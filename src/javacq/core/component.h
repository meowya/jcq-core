//
// Created by Sobte on 2019-12-04.
//
#pragma once

#include "javacq/core/common.h"

#include "javacq/core/context.h"

namespace jcq {
    /**
     * Lifecycle:
     *
     * +-------------------------------------------------+
     * | hook_initialize                                 |
     * |        +                                        |
     * |        | enabled by user                        |
     * |        v                                        |
     * | hook_start                                |
     * |   hook_enable+-----------+                      |
     * |        +                 | disabled by user     |
     * |        | coolq closed    v                      |
     * |        v            hook_disable                |
     * |   hook_disable           +                      |
     * |  hook_exit         | coolq closed         |
     * |                          v                      |
     * |                    hook_exit              |
     * +-------------------------------------------------+
     * This Code by Richard Chien
     * Details view：https://github.com/richardchien/coolq-http-api/blob/master/src/cqhttp/core/plugin.h
     */
    struct Component {
        Component() = default;

        virtual ~Component() = default;

        virtual std::string name() const = 0;

        virtual void hook_initialize(Context &ctx) { ctx.next(); }

        virtual void hook_enable(Context &ctx) { ctx.next(); }

        virtual void hook_disable(Context &ctx) { ctx.next(); }

        virtual void hook_start(Context &ctx) { ctx.next(); }

        virtual void hook_exit(Context &ctx) { ctx.next(); }

        virtual void hook_before_event(EventContext <cq::Event> &ctx) { ctx.next(); }

        virtual void hook_private_msg(EventContext <cq::PrivateMessageEvent> &ctx) { ctx.next(); }

        virtual void hook_group_msg(EventContext <cq::GroupMessageEvent> &ctx) { ctx.next(); }

        virtual void hook_discuss_msg(EventContext <cq::DiscussMessageEvent> &ctx) { ctx.next(); }

        virtual void hook_message_event(EventContext <cq::MessageEvent> &ctx) { ctx.next(); }

        virtual void hook_group_upload(EventContext <cq::GroupUploadEvent> &ctx) { ctx.next(); }

        virtual void hook_group_admin(EventContext <cq::GroupAdminEvent> &ctx) { ctx.next(); }

        virtual void hook_group_member_decrease(EventContext <cq::GroupMemberDecreaseEvent> &ctx) { ctx.next(); }

        virtual void hook_group_member_increase(EventContext <cq::GroupMemberIncreaseEvent> &ctx) { ctx.next(); }

        virtual void hook_group_ban(EventContext <cq::GroupBanEvent> &ctx) { ctx.next(); }

        virtual void hook_friend_add(EventContext <cq::FriendAddEvent> &ctx) { ctx.next(); }

        virtual void hook_notice_event(EventContext <cq::NoticeEvent> &ctx) { ctx.next(); }

        virtual void hook_friend_request(EventContext <cq::FriendRequestEvent> &ctx) { ctx.next(); }

        virtual void hook_group_request(EventContext <cq::GroupRequestEvent> &ctx) { ctx.next(); }

        virtual void hook_request_event(EventContext <cq::RequestEvent> &ctx) { ctx.next(); }

        virtual void hook_after_event(EventContext <cq::Event> &ctx) { ctx.next(); }

        virtual void hook_menu_event(MenuContext &ctx) { ctx.next(); }

        virtual bool good() const { return true; }
    };
} // namespace jcq