#include "Immutable/ImmutableUtilities.h"

#include "Immutable/Misc/ImtblLogging.h"
#include "Interfaces/IPluginManager.h"
#include "Misc/FileHelper.h"


bool FImmutableUtilities::LoadGameBridge(FString& GameBridge)
{
	const FString JavaScriptFilePath = IPluginManager::Get().FindPlugin(TEXT("Immutable"))->GetBaseDir() / TEXT("Web/index.js");

	IMTBL_LOG("Trying to locate game-bridge file")
	if (FFileHelper::LoadFileToString(GameBridge, *JavaScriptFilePath, FFileHelper::EHashOptions::EnableVerify))
	{
		IMTBL_LOG("Successfully loaded game-bridge file")
		return true;
	}
	IMTBL_ERR("Failed to load game-bridge file")

	return false;
}
