//
// Created by Sobte on 2019-12-05.
//
#include "core.h"

using namespace std;

namespace jcq {

    Application app;

    static vector<function<void()>> callback_initializers;

    static bool add_callback_initializer(const function<void()> &initializer) {
        callback_initializers.emplace_back(initializer);
        return true;
    }

    void init() {
        for (const auto &initializer : callback_initializers) {
            initializer();
        }
    }

/**
 * Generate lifecycle callbacks.
 */
#define LIFECYCLE(Name)            \
    static void _on_##Name() {     \
        app.on_##Name();           \
    }                              \
    static bool _dummy_on_##Name = \
        add_callback_initializer([]() { cq::_##Name##_callbacks().emplace_back(_on_##Name); })

    /*
    Replacement:

    static void _on_initialize() {
        app.on_initialize();
    }

    static bool _dummy_on_initialize = add_callback_initializer([]() {
        cq::_initialize_callbacks().emplace_back(_on_initialize);
    });

    ...
    */

    LIFECYCLE(initialize);

    LIFECYCLE(enable);

    LIFECYCLE(disable);

    LIFECYCLE(coolq_start);

    LIFECYCLE(coolq_exit);

/**
 * Generate event callbacks.
 */
#define EVENT(Name, ...)                                                                        \
    static void _on_##Name(__VA_ARGS__);                                                        \
    static bool _dummy_##Name##_event =                                                         \
        add_callback_initializer([]() { cq::_##Name##_callbacks().emplace_back(_on_##Name); }); \
    static void _on_##Name(__VA_ARGS__)

    EVENT(private_message, const cq::PrivateMessageEvent &e) {
        json data = e;
        app.on_before_event(e, data);
        app.on_private_msg(e, data);
        app.on_message_event(e, data);
        app.on_after_event(e, data);
    }

    EVENT(group_message, const cq::GroupMessageEvent &e) {
        json data = e;
        app.on_before_event(e, data);
        app.on_group_msg(e, data);
        app.on_message_event(e, data);
        app.on_after_event(e, data);
    }

    EVENT(discuss_message, const cq::DiscussMessageEvent &e) {
        json data = e;
        app.on_before_event(e, data);
        app.on_discuss_msg(e, data);
        app.on_message_event(e, data);
        app.on_after_event(e, data);
    }

    EVENT(group_upload, const cq::GroupUploadEvent &e) {
        json data = e;
        app.on_before_event(e, data);
        app.on_group_upload(e, data);
        app.on_notice_event(e, data);
        app.on_after_event(e, data);
    }

    EVENT(group_admin, const cq::GroupAdminEvent &e) {
        json data = e;
        app.on_before_event(e, data);
        app.on_group_admin(e, data);
        app.on_notice_event(e, data);
        app.on_after_event(e, data);
    }

    EVENT(group_member_decrease, const cq::GroupMemberDecreaseEvent &e) {
        json data = e;
        app.on_before_event(e, data);
        app.on_group_member_decrease(e, data);
        app.on_notice_event(e, data);
        app.on_after_event(e, data);
    }

    EVENT(group_member_increase, const cq::GroupMemberIncreaseEvent &e) {
        json data = e;
        app.on_before_event(e, data);
        app.on_group_member_increase(e, data);
        app.on_notice_event(e, data);
        app.on_after_event(e, data);
    }

    EVENT(group_ban, const cq::GroupBanEvent &e) {
        json data = e;
        app.on_before_event(e, data);
        app.on_group_ban(e, data);
        app.on_notice_event(e, data);
        app.on_after_event(e, data);
    }

    EVENT(friend_add, const cq::FriendAddEvent &e) {
        json data = e;
        app.on_before_event(e, data);
        app.on_friend_add(e, data);
        app.on_notice_event(e, data);
        app.on_after_event(e, data);
    }

    EVENT(friend_request, const cq::FriendRequestEvent &e) {
        json data = e;
        app.on_before_event(e, data);
        app.on_friend_request(e, data);
        app.on_request_event(e, data);
        app.on_after_event(e, data);
    }

    EVENT(group_request, const cq::GroupRequestEvent &e) {
        json data = e;
        app.on_before_event(e, data);
        app.on_group_request(e, data);
        app.on_request_event(e, data);
        app.on_after_event(e, data);
    }

} // namespace jcq