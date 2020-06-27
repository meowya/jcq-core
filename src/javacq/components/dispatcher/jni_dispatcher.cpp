//
// Created by Sobte on 2020-01-13.
//

#include "jni_dispatcher.h"

#include "javacq/core/java.h"

#include "javacq/utils/jniex.h"
#include "javacq/utils/options.h"

using namespace std;
using namespace jcq::java;
using namespace jcq::exception;
using namespace jcq::utils::dir;

namespace jcq::components {

    static const auto TAG = "JNI";
    static jclass app_cls = nullptr;
    static jobject app_obj = nullptr;

    void JNIDispatcher::hook_start(Context &ctx) {
        try {
            string jre_path = ctx.config->get_string("jre.path");
            if (jre_path.empty()) {
                throw RuntimeError(u8"未指定Jre路径，请配置正确的Jre路径，并重启软件");
            }
            jni::__init(jre_path); // load jvm dll
            jni::create_java_vm(utils::java::get_vm_options("JCQ"), JNI_VERSION_1_8); // load jvm
            loaded_ = true;
        } catch (Exception &error) {
            logging::error(TAG, error.what());
        }
        ctx.next();
    }

    void JNIDispatcher::hook_exit(Context &ctx) {
        if (loaded_) {
            try {
                const auto env = utils::jni::JNIEnvEx();
                // call jcq start and enable
                const auto app_exit = env.get_method_id(app_cls, "exit", "()I");
                env.raw->CallIntMethod(app_obj, app_exit);
                jni::destroy_java_vm();
                loaded_ = false;
            } catch (Exception &error) {
                logging::error(TAG, error.what());
            }
        }
        ctx.next();
    }

    void JNIDispatcher::hook_enable(Context &ctx) {
        try {
            const auto env = utils::jni::JNIEnvEx();
            if (loaded_ && !started_) {
                const auto main_class = env.get_class(ctx.config->get_string("main-class"));
                const auto super_class = env.get_class("org.meowy.cqp.jcq.event.JcqListener");
                if (!env.raw->IsAssignableFrom(main_class, super_class)) {
                    throw RuntimeError(ctx.config->get_string("main-class")
                                       + u8" 不是合规的JCQ启动类，建议关闭自定义，使用JCQ自带启动类");
                }
                // TODO 将初始构造函数的目录移除
                const auto main_init = env.raw->GetMethodID(main_class, "<init>", "(Ljava/lang/String;I)V");
                if (!main_init) {
                    throw RuntimeError(
                        ctx.config->get_string("main-class")
                        + u8" 不是合规的JCQ启动类，未包含指定构造函数，建议关闭自定义，使用JCQ自带启动类");
                }
                const auto main_obj =
                    env.raw->NewObject(main_class, main_init, string_to_java(cq::get_app_directory()), cq::_ac());
                if (env.raw->ExceptionCheck() || !main_obj) {
                    throw RuntimeError(ctx.config->get_string("main-class")
                                       + u8" 启动类初始化异常，建议关闭自定义，使用JCQ自带启动类");
                }
                app_cls = main_class;
                app_obj = main_obj;

                // call jcq start
                const auto app_start = env.get_method_id(app_cls, "startup", "()I");
                env.raw->CallIntMethod(app_obj, app_start);

                started_ = true;
            }
            // call jcq enable
            const auto app_enable = env.get_method_id(app_cls, "enable", "()I");
            env.raw->CallIntMethod(app_obj, app_enable);
        } catch (Exception &error) {
            logging::error(TAG, error.what());
        }
        ctx.next();
    }

    void JNIDispatcher::hook_disable(Context &ctx) {
        if (loaded_) {
            try {
                const auto env = utils::jni::JNIEnvEx();
                const auto app_disable = env.get_method_id(app_cls, "disable", "()I");
                env.raw->CallIntMethod(app_obj, app_disable);
                started_ = false;
            } catch (Exception &error) {
                logging::error(TAG, error.what());
            }
        }
        ctx.next();
    }

    void JNIDispatcher::hook_before_event(EventContext<cq::Event> &ctx) {
        if (started_) {
            try {
                jni::attach_current_thread();
            } catch (Exception &error) {
                logging::error(TAG, error.what());
            }
        }
        ctx.next();
    }

    void JNIDispatcher::hook_private_msg(EventContext<cq::PrivateMessageEvent> &ctx) {
        if (started_) {
            try {
                const auto env = utils::jni::JNIEnvEx();
                const auto app_message_event = env.get_method_id(app_cls, "privateMsg", "(IIJLjava/lang/String;I)I");
                env.raw->CallIntMethod(app_obj,
                                       app_message_event,
                                       ctx.event.sub_type,
                                       ctx.event.message_id,
                                       ctx.event.user_id,
                                       string_to_java(ctx.event.message),
                                       ctx.event.font);
            } catch (Exception &error) {
                logging::error(TAG, error.what());
            }
        }
        ctx.next();
    }

    void JNIDispatcher::hook_group_msg(EventContext<cq::GroupMessageEvent> &ctx) {
        if (started_) {
            try {
                const auto env = utils::jni::JNIEnvEx();
                const auto app_message_event =
                    env.get_method_id(app_cls, "groupMsg", "(IIJJLjava/lang/String;Ljava/lang/String;I)I");
                env.raw->CallIntMethod(app_obj,
                                       app_message_event,
                                       ctx.event.sub_type,
                                       ctx.event.message_id,
                                       ctx.event.group_id,
                                       ctx.event.user_id,
                                       string_to_java(ctx.event.anonymous.base64),
                                       string_to_java(ctx.event.message),
                                       ctx.event.font);
            } catch (Exception &error) {
                logging::error(TAG, error.what());
            }
        }
        ctx.next();
    }

    void JNIDispatcher::hook_discuss_msg(EventContext<cq::DiscussMessageEvent> &ctx) {
        if (started_) {
            try {
                const auto env = utils::jni::JNIEnvEx();
                const auto app_message_event = env.get_method_id(app_cls, "discussMsg", "(IIJJLjava/lang/String;I)I");
                env.raw->CallIntMethod(app_obj,
                                       app_message_event,
                                       ctx.event.sub_type,
                                       ctx.event.message_id,
                                       ctx.event.discuss_id,
                                       ctx.event.user_id,
                                       string_to_java(ctx.event.message),
                                       ctx.event.font);
            } catch (Exception &error) {
                logging::error(TAG, error.what());
            }
        }
        ctx.next();
    }

    void JNIDispatcher::hook_group_upload(EventContext<cq::GroupUploadEvent> &ctx) {
        if (started_) {
            try {
                const auto env = utils::jni::JNIEnvEx();
                const auto app_notice_event = env.get_method_id(app_cls, "groupUpload", "(IIJJLjava/lang/String;)I");
                env.raw->CallIntMethod(app_obj,
                                       app_notice_event,
                                       ctx.event.sub_type,
                                       ctx.event.time,
                                       ctx.event.group_id,
                                       ctx.event.user_id,
                                       string_to_java(ctx.event.file.base64));
            } catch (Exception &error) {
                logging::error(TAG, error.what());
            }
        }
        ctx.next();
    }

    void JNIDispatcher::hook_group_admin(EventContext<cq::GroupAdminEvent> &ctx) {
        if (started_) {
            try {
                const auto env = utils::jni::JNIEnvEx();
                const auto app_notice_event = env.get_method_id(app_cls, "groupAdmin", "(IIJJ)I");
                env.raw->CallIntMethod(app_obj,
                                       app_notice_event,
                                       ctx.event.sub_type,
                                       ctx.event.time,
                                       ctx.event.group_id,
                                       ctx.event.user_id);
            } catch (Exception &error) {
                logging::error(TAG, error.what());
            }
        }
        ctx.next();
    }

    void JNIDispatcher::hook_group_member_decrease(EventContext<cq::GroupMemberDecreaseEvent> &ctx) {
        if (started_) {
            try {
                const auto env = utils::jni::JNIEnvEx();
                const auto app_notice_event = env.get_method_id(app_cls, "groupMemberDecrease", "(IIJJJ)I");
                env.raw->CallIntMethod(app_obj,
                                       app_notice_event,
                                       ctx.event.sub_type,
                                       ctx.event.time,
                                       ctx.event.group_id,
                                       ctx.event.user_id,
                                       ctx.event.operator_id);
            } catch (Exception &error) {
                logging::error(TAG, error.what());
            }
        }
        ctx.next();
    }

    void JNIDispatcher::hook_group_member_increase(EventContext<cq::GroupMemberIncreaseEvent> &ctx) {
        if (started_) {
            try {
                const auto env = utils::jni::JNIEnvEx();
                const auto app_notice_event = env.get_method_id(app_cls, "groupMemberIncrease", "(IIJJJ)I");
                env.raw->CallIntMethod(app_obj,
                                       app_notice_event,
                                       ctx.event.sub_type,
                                       ctx.event.time,
                                       ctx.event.group_id,
                                       ctx.event.user_id,
                                       ctx.event.operator_id);
            } catch (Exception &error) {
                logging::error(TAG, error.what());
            }
        }
        ctx.next();
    }

    void JNIDispatcher::hook_group_ban(EventContext<cq::GroupBanEvent> &ctx) {
        if (started_) {
            try {
                const auto env = utils::jni::JNIEnvEx();
                const auto app_notice_event = env.get_method_id(app_cls, "groupBan", "(IIJJJJ)I");
                env.raw->CallIntMethod(app_obj,
                                       app_notice_event,
                                       ctx.event.sub_type,
                                       ctx.event.time,
                                       ctx.event.group_id,
                                       ctx.event.user_id,
                                       ctx.event.operator_id,
                                       ctx.event.duration);
            } catch (Exception &error) {
                logging::error(TAG, error.what());
            }
        }
        ctx.next();
    }

    void JNIDispatcher::hook_friend_add(EventContext<cq::FriendAddEvent> &ctx) {
        if (started_) {
            try {
                const auto env = utils::jni::JNIEnvEx();
                const auto app_notice_event = env.get_method_id(app_cls, "friendAdd", "(IIJ)I");
                env.raw->CallIntMethod(
                    app_obj, app_notice_event, ctx.event.sub_type, ctx.event.time, ctx.event.user_id);
            } catch (Exception &error) {
                logging::error(TAG, error.what());
            }
        }
        ctx.next();
    }

    void JNIDispatcher::hook_friend_request(EventContext<cq::FriendRequestEvent> &ctx) {
        if (started_) {
            try {
                const auto env = utils::jni::JNIEnvEx();
                const auto app_request_event =
                    env.get_method_id(app_cls, "requestAddFriend", "(IIJLjava/lang/String;Ljava/lang/String;)I");
                env.raw->CallIntMethod(app_obj,
                                       app_request_event,
                                       ctx.event.sub_type,
                                       ctx.event.time,
                                       ctx.event.user_id,
                                       string_to_java(ctx.event.comment),
                                       string_to_java(ctx.event.flag));
            } catch (Exception &error) {
                logging::error(TAG, error.what());
            }
        }
        ctx.next();
    }

    void JNIDispatcher::hook_group_request(EventContext<cq::GroupRequestEvent> &ctx) {
        if (started_) {
            try {
                const auto env = utils::jni::JNIEnvEx();
                const auto app_request_event =
                    env.get_method_id(app_cls, "requestAddGroup", "(IIJJLjava/lang/String;Ljava/lang/String;)I");
                env.raw->CallIntMethod(app_obj,
                                       app_request_event,
                                       ctx.event.sub_type,
                                       ctx.event.time,
                                       ctx.event.group_id,
                                       ctx.event.user_id,
                                       string_to_java(ctx.event.comment),
                                       string_to_java(ctx.event.flag));
            } catch (Exception &error) {
                logging::error(TAG, error.what());
            }
        }
        ctx.next();
    }

    void JNIDispatcher::hook_after_event(EventContext<cq::Event> &ctx) {
        if (started_) {
            try {
                jni::detach_current_thread();
            } catch (Exception &error) {
                logging::error(TAG, error.what());
            }
        }
        ctx.next();
    }

    void JNIDispatcher::hook_menu_event(MenuContext &ctx) {
        if (started_ && ctx.name == "manage") {
            try {
                const auto env = utils::jni::JNIEnvEx();
                const auto app_menu_event = env.get_method_id(app_cls, "manage", "()I");
                env.raw->CallVoidMethod(app_obj, app_menu_event);
            } catch (Exception &error) {
                logging::error(TAG, error.what());
            }
        }
        ctx.next();
    }

} // namespace jcq::components