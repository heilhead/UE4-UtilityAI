﻿#pragma once

#include "Modules/ModuleInterface.h"

class FUtilityAIEditorModule : public IModuleInterface {
public:
    virtual void
    StartupModule() override;

    virtual void
    ShutdownModule() override;
};
