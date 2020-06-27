//
// Created by Sobte on 2019-12-05.
//
#pragma once

#include "javacq/core/common.h"

namespace cq {

    inline void to_json(json &j, const Sex &sex) {
        switch (sex) {
        case Sex::MALE:
            j = "male";
            break;
        case Sex::FEMALE:
            j = "female";
            break;
        default:
            j = "unknown";
            break;
        }
    }

    inline void to_json(json &j, const GroupRole &role) {
        switch (role) {
        case GroupRole::MEMBER:
            j = "member";
            break;
        case GroupRole::ADMIN:
            j = "admin";
            break;
        case GroupRole::OWNER:
            j = "owner";
            break;
        default:
            j = "unknown";
            break;
        }
    }

    inline void to_json(json &j, const User &u) {
        j = {
            {"user_id", u.user_id},
            {"nickname", u.nickname},
            {"sex", u.sex},
            {"age", u.age},
        };
    }

    inline void to_json(json &j, const Friend &f) {
        j = {
            {"user_id", f.user_id},
            {"nickname", f.nickname},
            {"remark", f.remark},
        };
    }

    inline void to_json(json &j, const Group &g) {
        j = {
            {"group_id", g.group_id},
            {"group_name", g.group_name},
            {"member_count", g.member_count},
            {"max_member_count", g.max_member_count},
        };
    }

    inline void to_json(json &j, const GroupMember &m) {
        j = {
            {"group_id", m.group_id},
            {"user_id", m.user_id},
            {"nickname", m.nickname},
            {"card", m.card},
            {"sex", m.sex},
            {"age", m.age},
            {"area", m.area},
            {"join_time", m.join_time},
            {"last_sent_time", m.last_sent_time},
            {"level", m.level},
            {"role", m.role},
            {"unfriendly", m.unfriendly},
            {"title", m.title},
            {"title_expire_time", m.title_expire_time},
            {"card_changeable", m.card_changeable},
        };
    }

    inline void to_json(json &j, const Anonymous &a) {
        j = {
            {"id", a.id},
            {"name", a.name},
            {"token", a.token},
        };
    }

    inline void from_json(const json &j, Anonymous &a) {
        a.id = j.at("id").get<int64_t>();
        a.name = j.at("name").get<std::string>();
        a.token = j.at("token").get<std::string>();
    }

    inline void to_json(json &j, const File &f) {
        j = {
            {"id", f.id},
            {"name", f.name},
            {"size", f.size},
            {"busid", f.busid},
        };
    }

} // namespace cq

namespace cq::message {

    inline void to_json(json &j, const MessageSegment &segment) {
        j = json{{"type", segment.type}, {"data", segment.data}};
    }

    inline void from_json(const json &j, MessageSegment &segment) {
        segment.type = j.at("type").get<std::string>();
        auto data = j.at("data");
        for (auto it = data.begin(); it != data.end(); ++it) {
            if (!it.value().is_string()) {
                it.value() = it.value().dump();
            }
        }
        segment.data = data.get<std::map<std::string, std::string>>();
    }

    inline void to_json(json &j, const Message &message) {
        j = message.segments();
    }

    inline void from_json(const json &j, Message &message) {
        if (j.is_array()) {
            message.segments() = j.get<std::remove_reference<decltype(message.segments())>::type>();
            message.reduce();
        } else if (j.is_object()) {
            message.push_back(j.get<MessageSegment>());
        } else {
            message = j.get<std::string>();
        }
    }

    inline void to_json(json &j, const MessageEvent::DetailType &type) {
        j = to_string(type);
    }

} // namespace cq::message

namespace cq {

    inline void to_json(json &j, const NoticeEvent::DetailType &type) {
        j = to_string(type);
    }

    inline void to_json(json &j, const RequestEvent::DetailType &type) {
        j = to_string(type);
    }

    inline void to_json(json &j, const UserEvent::Type &type) {
        j = to_string(type);
    }

    inline void to_json(json &j, const PrivateMessageEvent &e) {
        j = {
            {"post_type", e.type},
            {"detail_type", e.detail_type},
            {"sub_type", to_string(e.sub_type)},
            {"message_id", e.message_id},
            {"user_id", e.user_id},
            {"message", e.message},
            {"font", e.font},
        };
    }

    inline void to_json(json &j, const GroupMessageEvent &e) {
        j = {
            {"post_type", e.type},
            {"detail_type", e.detail_type},
            {"sub_type", to_string(e.sub_type)},
            {"message_id", e.message_id},
            {"group_id", e.group_id},
            {"user_id", e.user_id},
            {"anonymous", nullptr},
            {"message", e.message},
            {"font", e.font},
        };

        if (e.is_anonymous()) {
            j["anonymous"] = e.anonymous;
        }
    }

    inline void to_json(json &j, const DiscussMessageEvent &e) {
        j = {
            {"post_type", e.type},
            {"detail_type", e.detail_type},
            {"message_id", e.message_id},
            {"discuss_id", e.discuss_id},
            {"user_id", e.user_id},
            {"message", e.message},
            {"font", e.font},
        };
    }

    inline void to_json(json &j, const GroupUploadEvent &e) {
        j = {
            {"time", e.time},
            {"post_type", e.type},
            {"detail_type", e.detail_type},
            {"group_id", e.group_id},
            {"user_id", e.user_id},
            {"file", e.file},
        };
    }

    inline void to_json(json &j, const GroupAdminEvent &e) {
        j = {
            {"time", e.time},
            {"post_type", e.type},
            {"detail_type", e.detail_type},
            {"sub_type", to_string(e.sub_type)},
            {"group_id", e.group_id},
            {"user_id", e.user_id},
        };
    }

    inline void to_json(json &j, const GroupMemberDecreaseEvent &e) {
        j = {
            {"time", e.time},
            {"post_type", e.type},
            {"detail_type", e.detail_type},
            {"sub_type", to_string(e.sub_type)},
            {"group_id", e.group_id},
            {"operator_id", e.operator_id},
            {"user_id", e.user_id},
        };
    }

    inline void to_json(json &j, const GroupMemberIncreaseEvent &e) {
        j = {
            {"time", e.time},
            {"post_type", e.type},
            {"detail_type", e.detail_type},
            {"sub_type", to_string(e.sub_type)},
            {"group_id", e.group_id},
            {"operator_id", e.operator_id},
            {"user_id", e.user_id},
        };
    }

    inline void to_json(json &j, const GroupBanEvent &e) {
        j = {
            {"time", e.time},
            {"post_type", e.type},
            {"detail_type", e.detail_type},
            {"sub_type", to_string(e.sub_type)},
            {"group_id", e.group_id},
            {"operator_id", e.operator_id},
            {"user_id", e.user_id},
            {"duration", e.duration},
        };
    }

    inline void to_json(json &j, const FriendAddEvent &e) {
        j = {
            {"time", e.time},
            {"post_type", e.type},
            {"detail_type", e.detail_type},
            {"user_id", e.user_id},
        };
    }

    inline void to_json(json &j, const FriendRequestEvent &e) {
        j = {
            {"time", e.time},
            {"post_type", e.type},
            {"detail_type", e.detail_type},
            {"user_id", e.user_id},
            {"comment", e.comment},
            {"flag", e.flag},
        };
    }

    inline void to_json(json &j, const GroupRequestEvent &e) {
        j = {
            {"time", e.time},
            {"post_type", e.type},
            {"detail_type", e.detail_type},
            {"sub_type", to_string(e.sub_type)},
            {"group_id", e.group_id},
            {"user_id", e.user_id},
            {"comment", e.comment},
            {"flag", e.flag},
        };
    }

} // namespace cq