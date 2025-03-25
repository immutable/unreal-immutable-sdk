#pragma once

#include "Modules/ModuleManager.h"

class FImmutableCoreModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};