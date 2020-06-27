//
// Created by Sobte on 2019-12-04.
//

#include "application.h"

namespace jcq {
    void Application::on_initialize() {
        initialized_ = true;
        iterate_hooks(&Component::hook_initialize, Context());
    }

    void Application::on_enable() {
        enabled_ = true;

        iterate_hooks(&Component::hook_enable, Context());
    }

    void Application::on_disable() {
        enabled_ = false;

        iterate_hooks(&Component::hook_disable, Context());
    }

    void Application::on_coolq_start() {
        iterate_hooks(&Component::hook_start, Context());
    }

    void Application::on_coolq_exit() {
        if (enabled_) {
            // generate a fake "disable" event at exit
            // this leads to a lifecycle change, check components.h for the lifecycle graph
            on_disable();
        }
        iterate_hooks(&Component::hook_exit, Context());
    }

} // namespace jcq