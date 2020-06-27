//
// Created by Sobte on 2019-12-05.
//
#pragma once

#include "javacq/core/application.h"
#include "javacq/core/common.h"
#include "javacq/core/component.h"

namespace jcq {

    extern Application app;

    /**
     * Initialize the module.
     * This will assign the event handlers of "jcq" module to "cqsdk" module.
     */
    void init();

    inline void use(const std::shared_ptr<Component> component) {
        app.components_.push_back(component);
    }

} // namespace jcq