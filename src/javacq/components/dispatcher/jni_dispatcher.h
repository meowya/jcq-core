//
// Created by Sobte on 2020-01-13.
//
#pragma once

#include <atomic>

#include "javacq/core/component.h"

namespace jcq::components {

    struct JNIDispatcher : Component {
        std::string name() const override { return "jni_adapter"; }

        void hook_start(Context &ctx) override;

        void hook_exit(Context &ctx) override;

        void hook_enable(Context &ctx) override;

        void hook_disable(Context &ctx) override;

        void hook_before_event(EventContext<cq::Event> &ctx) override;

        void hook_private_msg(EventContext<cq::PrivateMessageEvent> &ctx) override;

        void hook_group_msg(EventContext<cq::GroupMessageEvent> &ctx) override;

        void hook_discuss_msg(EventContext<cq::DiscussMessageEvent> &ctx) override;

        void hook_group_upload(EventContext<cq::GroupUploadEvent> &ctx) override;

        void hook_group_admin(EventContext<cq::GroupAdminEvent> &ctx) override;

        void hook_group_member_decrease(EventContext<cq::GroupMemberDecreaseEvent> &ctx) override;

        void hook_group_member_increase(EventContext<cq::GroupMemberIncreaseEvent> &ctx) override;

        void hook_group_ban(EventContext<cq::GroupBanEvent> &ctx) override;

        void hook_friend_add(EventContext<cq::FriendAddEvent> &ctx) override;

        void hook_friend_request(EventContext<cq::FriendRequestEvent> &ctx) override;

        void hook_group_request(EventContext<cq::GroupRequestEvent> &ctx) override;

        void hook_after_event(EventContext<cq::Event> &ctx) override;

        void hook_menu_event(MenuContext &ctx) override;

        bool good() const override { return started_ && loaded_; }

    private:
        std::atomic_bool started_ = false;
        std::atomic_bool loaded_ = false;
    };

    static std::shared_ptr<JNIDispatcher> jni_dispatcher = std::make_shared<JNIDispatcher>();

} // namespace jcq::components