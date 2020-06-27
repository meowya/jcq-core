//
// Created by Sobte on 2019-12-04.
//
#pragma once

#include "javacq/core/common.h"
#include "javacq/core/context.h"
#include "javacq/core/component.h"

namespace jcq {

    class Application {
    public:
        void on_initialize();

        void on_enable();

        void on_disable();

        void on_coolq_start();

        void on_coolq_exit();

        void on_before_event(const cq::Event &event, json &data) {
            iterate_hooks(&Component::hook_before_event, EventContext<cq::Event>(event, data));
        }

        void on_private_msg(const cq::PrivateMessageEvent &event, json &data) {
            iterate_hooks(&Component::hook_private_msg, EventContext<cq::PrivateMessageEvent>(event, data));
        }

        void on_group_msg(const cq::GroupMessageEvent &event, json &data) {
            iterate_hooks(&Component::hook_group_msg, EventContext<cq::GroupMessageEvent>(event, data));
        }

        void on_discuss_msg(const cq::DiscussMessageEvent &event, json &data) {
            iterate_hooks(&Component::hook_discuss_msg, EventContext<cq::DiscussMessageEvent>(event, data));
        }

        void on_message_event(const cq::MessageEvent &event, json &data) {
            iterate_hooks(&Component::hook_message_event, EventContext<cq::MessageEvent>(event, data));
        }

        void on_group_upload(const cq::GroupUploadEvent &event, json &data) {
            iterate_hooks(&Component::hook_group_upload, EventContext<cq::GroupUploadEvent>(event, data));
        }

        void on_group_admin(const cq::GroupAdminEvent &event, json &data) {
            iterate_hooks(&Component::hook_group_admin, EventContext<cq::GroupAdminEvent>(event, data));
        }

        void on_group_member_decrease(const cq::GroupMemberDecreaseEvent &event, json &data) {
            iterate_hooks(&Component::hook_group_member_decrease,
                          EventContext<cq::GroupMemberDecreaseEvent>(event, data));
        }

        void on_group_member_increase(const cq::GroupMemberIncreaseEvent &event, json &data) {
            iterate_hooks(&Component::hook_group_member_increase,
                          EventContext<cq::GroupMemberIncreaseEvent>(event, data));
        }

        void on_group_ban(const cq::GroupBanEvent &event, json &data) {
            iterate_hooks(&Component::hook_group_ban, EventContext<cq::GroupBanEvent>(event, data));
        }

        void on_friend_add(const cq::FriendAddEvent &event, json &data) {
            iterate_hooks(&Component::hook_friend_add, EventContext<cq::FriendAddEvent>(event, data));
        }

        void on_notice_event(const cq::NoticeEvent &event, json &data) {
            iterate_hooks(&Component::hook_notice_event, EventContext<cq::NoticeEvent>(event, data));
        }

        void on_friend_request(const cq::FriendRequestEvent &event, json &data) {
            iterate_hooks(&Component::hook_friend_request, EventContext<cq::FriendRequestEvent>(event, data));
        }

        void on_group_request(const cq::GroupRequestEvent &event, json &data) {
            iterate_hooks(&Component::hook_group_request, EventContext<cq::GroupRequestEvent>(event, data));
        }

        void on_request_event(const cq::RequestEvent &event, json &data) {
            iterate_hooks(&Component::hook_request_event, EventContext<cq::RequestEvent>(event, data));
        }

        void on_after_event(const cq::Event &event, json &data) {
            iterate_hooks(&Component::hook_after_event, EventContext<cq::Event>(event, data));
        }

        void on_menu_event(const std::string &name, json &data) {
            iterate_hooks(&Component::hook_menu_event, MenuContext(name, data));
        }

        bool initialized() const { return initialized_; }

        bool enabled() const { return enabled_; }

        std::map<std::string, bool> components_good() const {
            std::map<std::string, bool> results;
            for (const auto &p : components_) {
                results[p->name()] = p->good();
            }
            return results;
        }

        bool good() const {
            auto p = components_good();
            return initialized() && enabled()
                   && std::all_of(p.cbegin(), p.cend(), [](const auto &kv) { return kv.second; });
        }

        utils::JsonEx &config() { return config_; }

    private:
        std::vector<std::shared_ptr<Component>> components_;

        bool initialized_ = false;
        bool enabled_ = false;

        utils::JsonEx config_;

        template<typename HookFunc, typename Ctx>
        void iterate_hooks(const HookFunc hook_func, Ctx ctx) {
            ctx.config = &config_;

            auto it = components_.begin();
            Context::Next next = [&] {
                if (it == components_.end()) {
                    return;
                }

                ctx.next = next;
                (**it++.*hook_func)(ctx);
            };
            next();
        }

        friend void use(const std::shared_ptr<Component> component);
    };

} // namespace jcq