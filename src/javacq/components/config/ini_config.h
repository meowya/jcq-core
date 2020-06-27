//
// Created by Sobte on 2019-12-06.
//
#pragma once

#include "javacq/core/component.h"

namespace jcq::components {

    struct IniConfig : Component {
        std::string name() const override { return "ini_config"; }

        void hook_start(Context &ctx) override;
    };

    static std::shared_ptr<IniConfig> ini_config = std::make_shared<IniConfig>();

} // namespace jcq::components
