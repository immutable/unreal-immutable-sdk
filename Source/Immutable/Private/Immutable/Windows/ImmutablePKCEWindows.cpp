#include "Immutable/Windows/ImmutablePKCEWindows.h"

#include "Interfaces/IPluginManager.h"

#include "Windows/AllowWindowsPlatformTypes.h"
THIRD_PARTY_INCLUDES_START
#include <winreg.h>
THIRD_PARTY_INCLUDES_END
#include "Windows/HideWindowsPlatformTypes.h"

#include "Immutable/ImmutableDataTypes.h"
#include "Immutable/Misc/ImtblLogging.h"

// Registry constants
static const TCHAR* DeepLinkRegistryValueName = TEXT("DeepLink");

UImmutablePKCEData* UImmutablePKCEWindows::Initialise(const FImmutablePassportInitData& PassportInitData)
{
	// Extract the protocol scheme from the redirect URI
	FString Protocol;
	if (!ExtractProtocolFromUri(PassportInitData.redirectUri, Protocol))
	{
		return nullptr;
	}

	IMTBL_LOG("Attempting to register protocol handler for scheme: %s", *Protocol);

	// Register the custom protocol handler in Windows registry
	if (!RegisterProtocolHandler(Protocol))
	{
		IMTBL_ERR("Failed to register protocol handler for %s", *Protocol);
		return nullptr;
	}

	IMTBL_LOG("Registered protocol handler for %s", *Protocol);

	UImmutablePKCEData* PKCEData = NewObject<UImmutablePKCEData>();

	PKCEData->PassportInitData = PassportInitData;

	// Setup periodic check for deep links
	if (GEngine)
	{
		FTickerDelegate TickDelegate = FTickerDelegate::CreateWeakLambda(PKCEData, [PKCEData](float DeltaTime)
		{
			return UImmutablePKCEWindows::CheckDeepLinkTick(PKCEData, DeltaTime);
		});
		PKCEData->TickDelegateHandle = FTSTicker::GetCoreTicker().AddTicker(TickDelegate, 1.0f);
	}

	return PKCEData;
}

bool UImmutablePKCEWindows::RegisterProtocolHandler(const FString& Protocol)
{
	// Create a PowerShell script that will handle incoming protocol activations
	FString PowerShellFilePath = GetPowerShellScriptPath(Protocol);
	FString LogFilePath = GetLogFilePath(Protocol);

	auto Cleanup = [&](bool bDeletePowerShellScript, bool bDeleteRegistryKey)
	{
		if (bDeletePowerShellScript)
		{
			IFileManager::Get().Delete(*PowerShellFilePath);
		}

		if (bDeleteRegistryKey)
		{
			FString KeyPath = GetProtocolRegistryKeyPath(Protocol);
			RegDeleteTreeW(HKEY_CURRENT_USER, *KeyPath);
		}
	};

	// Script that logs incoming URL and stores it in registry for the game to find
	FString PsContent =
		TEXT("$url = $args -join ' '\n")
		TEXT("$url | Out-File -FilePath \"") + LogFilePath + TEXT("\"\n")
		TEXT("New-Item -Path \"HKCU:\\Software\\") + Protocol + TEXT("\" -Force | Out-Null\n")
		TEXT("Set-ItemProperty -Path \"HKCU:\\Software\\") + Protocol + TEXT("\" -Name \"") + DeepLinkRegistryValueName + TEXT("\" -Value $url\n");

	if (!FFileHelper::SaveStringToFile(PsContent, *PowerShellFilePath, FFileHelper::EEncodingOptions::ForceUTF8WithoutBOM))
	{
		IMTBL_ERR("Failed to create PowerShell script at %s", *PowerShellFilePath);
		return false;
	}

	FString CommandLine = FString::Printf(TEXT("powershell.exe -ExecutionPolicy Bypass -NoProfile -File \"%s\" %%1"), *PowerShellFilePath);
	FString KeyPath = GetProtocolRegistryKeyPath(Protocol);

	// Create main protocol handler registry key
	{
		HKEY KeyHandle;
		if (RegCreateKeyExW(HKEY_CURRENT_USER, *KeyPath, 0, nullptr, 0, KEY_WRITE, nullptr, &KeyHandle, nullptr) != ERROR_SUCCESS)
		{
			IMTBL_ERR("Failed to create registry key for protocol handler");
			Cleanup(true, false);
			return false;
		}

		FString DefaultValue = FString::Printf(TEXT("%s"), *Protocol);

		bool bSuccess =
			RegSetValueExW(KeyHandle, nullptr, 0, REG_SZ, (const BYTE*)*DefaultValue, (DefaultValue.Len() + 1) * sizeof(TCHAR)) == ERROR_SUCCESS &&
			RegSetValueExW(KeyHandle, TEXT("URL Protocol"), 0, REG_SZ, (const BYTE*)TEXT(""), sizeof(TCHAR)) == ERROR_SUCCESS;

		RegCloseKey(KeyHandle);

		if (!bSuccess)
		{
			IMTBL_ERR("Failed to set registry values for protocol handler");
			Cleanup(true, true);
			return false;
		}
	}

	// Setup shell key with empty default value
	{
		FString ShellKeyPath = FString::Printf(TEXT("%s\\shell"), *KeyPath);
		HKEY ShellKeyHandle;
		if (RegCreateKeyExW(HKEY_CURRENT_USER, *ShellKeyPath, 0, nullptr, 0, KEY_WRITE, nullptr, &ShellKeyHandle, nullptr) != ERROR_SUCCESS)
		{
			IMTBL_ERR("Failed to create shell registry key");
			Cleanup(true, true);
			return false;
		}

		FString EmptyValue = TEXT(" "); // Arbitrary value so that it will use the root for the deep link prompt
		bool bSuccess = RegSetValueExW(ShellKeyHandle, nullptr, 0, REG_SZ, (const BYTE*)*EmptyValue, (EmptyValue.Len() + 1) * sizeof(TCHAR)) == ERROR_SUCCESS;

		RegCloseKey(ShellKeyHandle);

		if (!bSuccess)
		{
			IMTBL_ERR("Failed to set shell registry value");
			Cleanup(true, true);
			return false;
		}
	}

	// Setup command to execute when protocol is activated in registry
	{
		FString CommandKeyPath = FString::Printf(TEXT("%s\\shell\\open\\command"), *KeyPath);
		HKEY CommandKeyHandle;
		if (RegCreateKeyExW(HKEY_CURRENT_USER, *CommandKeyPath, 0, nullptr, 0, KEY_WRITE, nullptr, &CommandKeyHandle, nullptr) != ERROR_SUCCESS)
		{
			IMTBL_ERR("Failed to create command registry key");
			Cleanup(true, true);
			return false;
		}

		bool bSuccess = RegSetValueExW(CommandKeyHandle, nullptr, 0, REG_SZ, (const BYTE*)*CommandLine, (CommandLine.Len() + 1) * sizeof(TCHAR)) == ERROR_SUCCESS;

		RegCloseKey(CommandKeyHandle);

		if (!bSuccess)
		{
			IMTBL_ERR("Failed to set command registry value");
			Cleanup(true, true);
			return false;
		}
	}

	return true;
}

bool UImmutablePKCEWindows::CheckDeepLinkTick(UImmutablePKCEData* PKCEData, float DeltaTime)
{
	// Extract protocol from the redirect URI
	FString Protocol;
	if (!ExtractProtocolFromUri(PKCEData->PassportInitData.redirectUri, Protocol))
	{
		return true; // Continue ticking even if we can't extract the protocol
	}

	FString RegistryKeyPath = GetDeepLinkRegistryKeyPath(Protocol);

	// Check if there's a deep link URL stored in the registry
	HKEY DeepLinkKeyHandle;
	if (RegOpenKeyExW(HKEY_CURRENT_USER, *RegistryKeyPath, 0, KEY_READ | KEY_WRITE, &DeepLinkKeyHandle) == ERROR_SUCCESS)
	{
		DWORD Type;
		DWORD DataSize = 0;

		// First query to get size of the registry value data
		if (RegQueryValueExW(DeepLinkKeyHandle, DeepLinkRegistryValueName, nullptr, &Type, nullptr, &DataSize) == ERROR_SUCCESS && Type == REG_SZ && DataSize > 0)
		{
			TArray<TCHAR> Buffer;
			Buffer.SetNumZeroed(DataSize / sizeof(TCHAR) + 1);

			// Second query to get actual data content
			if (RegQueryValueExW(DeepLinkKeyHandle, DeepLinkRegistryValueName, nullptr, nullptr, (BYTE*)Buffer.GetData(), &DataSize) == ERROR_SUCCESS)
			{
				FString DeepLink = Buffer.GetData();

				if (!DeepLink.IsEmpty())
				{
					IMTBL_LOG("Found deep link in registry: %s", *DeepLink);

					// Clear the registry value to avoid processing the same deep link multiple times
					FString EmptyValue = TEXT("");
					RegSetValueExW(DeepLinkKeyHandle, DeepLinkRegistryValueName, 0, REG_SZ, (const BYTE*)*EmptyValue, (EmptyValue.Len() + 1) * sizeof(TCHAR));
					RegCloseKey(DeepLinkKeyHandle);

					HandleDeepLink(PKCEData, DeepLink);

					return true;
				}
			}
		}

		RegCloseKey(DeepLinkKeyHandle);
	}
	else
	{
		// Create the registry key if it doesn't exist yet
		if (RegCreateKeyExW(HKEY_CURRENT_USER, *RegistryKeyPath, 0, nullptr, 0, KEY_WRITE, nullptr, &DeepLinkKeyHandle, nullptr) == ERROR_SUCCESS)
		{
			FString EmptyValue = TEXT("");
			RegSetValueExW(DeepLinkKeyHandle, DeepLinkRegistryValueName, 0, REG_SZ, (const BYTE*)*EmptyValue, (EmptyValue.Len() + 1) * sizeof(TCHAR));
			RegCloseKey(DeepLinkKeyHandle);
		}
	}

	return true;
}

void UImmutablePKCEWindows::HandleDeepLink(UImmutablePKCEData* PKCEData, const FString& DeepLink)
{
	// Clean up the deep link URL by removing quotes and extra whitespace
	FString CleanDeepLink = DeepLink.TrimStartAndEnd().Replace(TEXT("\""), TEXT(""));
	IMTBL_LOG("Handle Deep Link: %s", *CleanDeepLink);

	// Verify the deep link matches one of our expected redirect URIs before broadcasting
	if (CleanDeepLink.StartsWith(PKCEData->PassportInitData.redirectUri) || CleanDeepLink.StartsWith(PKCEData->PassportInitData.logoutRedirectUri))
	{
		PKCEData->DynamicMulticastDelegate_DeepLinkCallback.Broadcast(CleanDeepLink);
	}
	else
	{
		IMTBL_WARN("Deep link URL does not match expected redirect URIs: %s or %s", *PKCEData->PassportInitData.redirectUri, *PKCEData->PassportInitData.logoutRedirectUri);
	}
}

void UImmutablePKCEWindows::Reset(UImmutablePKCEData* PKCEData)
{
	if (!PKCEData)
	{
		return;
	}

// In shipping builds, clean up protocol handler registry entries and temp files
// In non-shipping builds, skip cleanup for easier testing and debugging
#if UE_BUILD_SHIPPING
	// Extract the protocol scheme from the redirect URI
	FString Protocol;
	if (!ExtractProtocolFromUri(PKCEData->PassportInitData.redirectUri, Protocol))
	{
		return;
	}

	// Delete the PowerShell script file
	FString PowerShellFilePath = GetPowerShellScriptPath(Protocol);

	if (FPaths::FileExists(PowerShellFilePath))
	{
		if (IFileManager::Get().Delete(*PowerShellFilePath))
		{
			IMTBL_LOG("Deleted PowerShell script: %s", *PowerShellFilePath);
		}
		else
		{
			IMTBL_WARN("Failed to delete PowerShell script: %s", *PowerShellFilePath);
		}
	}

	// Delete the log file if it exists
	FString LogFilePath = GetLogFilePath(Protocol);
	if (FPaths::FileExists(LogFilePath))
	{
		if (IFileManager::Get().Delete(*LogFilePath))
		{
			IMTBL_LOG("Deleted log file: %s", *LogFilePath);
		}
		else
		{
			IMTBL_WARN("Failed to delete log file: %s", *LogFilePath);
		}
	}

	// Delete registry keys
	FString ProtocolKeyPath = GetProtocolRegistryKeyPath(Protocol);
	LSTATUS lResult = RegDeleteTreeW(HKEY_CURRENT_USER, *ProtocolKeyPath);
	if (lResult == ERROR_SUCCESS)
	{
		IMTBL_LOG("Deleted protocol handler registry key: %s", *ProtocolKeyPath);
	}
	else
	{
		IMTBL_WARN("Failed to delete protocol handler registry key: %s (Error code: %d)", *ProtocolKeyPath, lResult);
	}

	// Delete the deep link registry key
	FString DeepLinkRegistryKeyPath = GetDeepLinkRegistryKeyPath(Protocol);
	lResult = RegDeleteKeyW(HKEY_CURRENT_USER, *DeepLinkRegistryKeyPath);
	if (lResult == ERROR_SUCCESS)
	{
		IMTBL_LOG("Deleted deep link registry key: %s", *DeepLinkRegistryKeyPath);
	}
	else
	{
		IMTBL_WARN("Failed to delete deep link registry key: %s (Error code: %d)", *DeepLinkRegistryKeyPath, lResult);
	}

	IMTBL_LOG("PKCE cleanup completed for protocol: %s", *Protocol);
#endif

	auto ResetPKCEData = [](UImmutablePKCEData* PKCEData)
	{
		PKCEData->DynamicMulticastDelegate_DeepLinkCallback.Clear();

		if (GEngine)
		{
			FTSTicker::GetCoreTicker().RemoveTicker(PKCEData->TickDelegateHandle);
			PKCEData->TickDelegateHandle.Reset();
		}

		PKCEData->PassportInitData = {};
	};

	ResetPKCEData(PKCEData);
}

FString UImmutablePKCEWindows::GetProtocolDirectory(const FString& Protocol)
{
	FString BaseDir;

// In shipping builds, store files in system temp directory
// In non-shipping builds, store in project directory for easier testing and debugging
#if UE_BUILD_SHIPPING
	// Use system temp directory in shipping builds
	BaseDir = FPlatformMisc::GetEnvironmentVariable(TEXT("TEMP"));
	if (BaseDir.IsEmpty())
	{
		// Fallback if TEMP environment variable is not available
		BaseDir = FPaths::ProjectSavedDir();
	}
#else
	// In non-shipping builds, use saved directory for testing purposes
	FString PluginName = TEXT("Immutable"); // Fallback default

	if (TSharedPtr<IPlugin> Plugin = IPluginManager::Get().FindPlugin(TEXT("Immutable")))
	{
		PluginName = Plugin->GetName();
	}

	BaseDir = FPaths::ProjectSavedDir() / PluginName;
#endif

	// Ensure the directory exists
	if (!FPlatformFileManager::Get().GetPlatformFile().DirectoryExists(*BaseDir))
	{
		FPlatformFileManager::Get().GetPlatformFile().CreateDirectoryTree(*BaseDir);
	}

	return FPaths::ConvertRelativePathToFull(BaseDir / Protocol);
}

FString UImmutablePKCEWindows::GetPowerShellScriptPath(const FString& Protocol)
{
	return GetProtocolDirectory(Protocol) / TEXT("PKCE.ps1");
}

FString UImmutablePKCEWindows::GetLogFilePath(const FString& Protocol)
{
	return GetProtocolDirectory(Protocol) / TEXT("PKCE.log");
}

FString UImmutablePKCEWindows::GetProtocolRegistryKeyPath(const FString& Protocol)
{
	return FString::Printf(TEXT("Software\\Classes\\%s"), *Protocol);
}

FString UImmutablePKCEWindows::GetDeepLinkRegistryKeyPath(const FString& Protocol)
{
	return FString::Printf(TEXT("Software\\%s"), *Protocol);
}

bool UImmutablePKCEWindows::ExtractProtocolFromUri(const FString& Uri, FString& OutProtocol)
{
	FString Right;

	if (!Uri.Split(TEXT("://"), &OutProtocol, &Right))
	{
		IMTBL_ERR("Failed to extract protocol from URI: %s", *Uri);
		return false;
	}

	return true;
}