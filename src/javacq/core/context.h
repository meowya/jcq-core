//
// Created by Sobte on 2019-12-04.
//
#pragma once

#include "javacq/core/common.h"

#include "javacq/utils/jsonex.h"

namespace jcq {

    struct Context {
        using Next = std::function<void()>;

        /**
         * This is granted to be non-null during calling component' hook functions.
         */
        utils::JsonEx *config = nullptr;

        /**
         * Should be called by plugins' hook functions if they want to let other plugins play.
         */
        Next next = nullptr;
    };

    template <typename E>
    struct EventContext : Context {
        /**
         * The raw event object (as a constant).
         */
        const E &event;

        /**
         * The jsonified event data. It may be modified by component' hook functions.
         */
        json &data;

        EventContext(const E &event, json &data) : event(event), data(data) {
        }
    };

    struct MenuContext : Context {
        const std::string &name;

        /**
         * The jsonified event data. It may be modified by component' hook functions.
         */
        json &data;

        MenuContext(const std::string &name, json &data) : name(name), data(data) {
        }
    };

} // namespace jcq