//
// Created by Sobte on 2019-12-05.
//
#pragma once

#include "javacq/core/component.h"

namespace jcq::components {

    struct DefaultConfig : Component {
        std::string name() const override { return "default_config"; }

        void hook_start(Context &ctx) override;
    };

    static std::shared_ptr<DefaultConfig> default_config = std::make_shared<DefaultConfig>();

} // namespace jcq::components