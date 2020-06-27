//
// Created by Sobte on 2019-12-06.
//
#pragma once

#include "javacq/core/component.h"

namespace jcq::components {

    struct DefaultResources : Component {
        std::string name() const override { return "default_resources"; }

        void hook_start(Context &ctx) override;
    };

    static std::shared_ptr<DefaultResources> default_resources = std::make_shared<DefaultResources>();

} // namespace jcq::components