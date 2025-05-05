#include "Immutable/Windows/ImmutablePKCEWindows.h"

#include "HAL/Runnable.h"
#include "HAL/RunnableThread.h"
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

// Static storage for registry watchers by protocol
static TMap<FString, TUniquePtr<FRegistryWatcherRunnable>> GRunnableMap;

// Helper function to get Windows error message from error code
static FString GetWindowsErrorMessage(DWORD ErrorCode)
{
	LPWSTR MessageBuffer = nullptr;
	DWORD Size = FormatMessageW
	(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr, ErrorCode, 0, (LPWSTR)&MessageBuffer, 0, nullptr
	);

	FString ErrorMessage;

	if (MessageBuffer)
	{
		ErrorMessage = FString(Size, MessageBuffer).TrimEnd();
		LocalFree(MessageBuffer);
	}
	else
	{
		ErrorMessage = FString::Printf(TEXT("Unknown error code: %d"), ErrorCode);
	}

	return ErrorMessage;
}

// Helper function to safely clean up a registry watcher thread
static void CleanupThreadForProtocol(const FString& Protocol)
{
	if (GRunnableMap.Contains(Protocol))
	{
		// Remove from map - unique ptr will handle deletion and stopping the thread
		GRunnableMap.Remove(Protocol);
	}
}

FRegistryWatcherRunnable::FRegistryWatcherRunnable(const FString& InRegistryKeyPath, TWeakObjectPtr<UImmutablePKCEData> InPKCEData):
	RegistryKeyPath(InRegistryKeyPath),
	PKCEData(InPKCEData)
{
	// Create Windows event objects for notification and termination
	EventHandle = CreateEvent(NULL, false, false, NULL);
	TerminateEventHandle = CreateEvent(NULL, true, false, NULL);
}

FRegistryWatcherRunnable::~FRegistryWatcherRunnable()
{
	// Stop the thread if running
	FRegistryWatcherRunnable::Stop();

	// Wait for thread to complete if it exists
	if (Thread)
	{
		Thread->WaitForCompletion();
		Thread.Reset();
	}

	// Close any open registry key
	if (DeepLinkKeyHandle)
	{
		RegCloseKey(DeepLinkKeyHandle);
		DeepLinkKeyHandle = nullptr;
	}

	// Close event handles
	if (EventHandle)
	{
		CloseHandle(EventHandle);
		EventHandle = nullptr;
	}

	if (TerminateEventHandle)
	{
		CloseHandle(TerminateEventHandle);
		TerminateEventHandle = nullptr;
	}
}

bool FRegistryWatcherRunnable::Init()
{
	// Validate event handles were created successfully
	if (!EventHandle || !TerminateEventHandle)
	{
		IMTBL_ERR("Failed to create event handles for registry watcher");
		return false;
	}

	return true;
}

bool FRegistryWatcherRunnable::StartThread(const FString& ProtocolName)
{
	if (Thread)
	{
		IMTBL_WARN("Thread already started for registry watcher");
		return true;
	}

	// Create the thread that will run this runnable
	Thread = TUniquePtr<FRunnableThread>
	(
		FRunnableThread::Create
		(
			this,
			*FString::Printf(TEXT("RegistryWatcher_%s"), *ProtocolName),
			0, // Use the current thread's stack size
			TPri_Normal
		)
	);

	if (!Thread)
	{
		IMTBL_ERR("Failed to create registry watcher thread for protocol: %s", *ProtocolName);
		return false;
	}

	return true;
}

uint32 FRegistryWatcherRunnable::Run()
{
	bool bKeyOpened = false;

	HANDLE WaitHandles[2] = {EventHandle, TerminateEventHandle};

	// Lambda to close registry key and reset flag
	auto CloseRegistryKey = [&]()
	{
		if (DeepLinkKeyHandle)
		{
			RegCloseKey(DeepLinkKeyHandle);
			DeepLinkKeyHandle = nullptr;
		}
		bKeyOpened = false;
	};

	while (bShouldRun)
	{
		// Open the registry key if not already open
		if (!bKeyOpened)
		{
			// Try to open existing key first
			LONG OpenResult = RegOpenKeyExW(HKEY_CURRENT_USER, *RegistryKeyPath, 0, KEY_READ | KEY_WRITE | KEY_NOTIFY, &DeepLinkKeyHandle);
			if (OpenResult != ERROR_SUCCESS)
			{
				// Create the key if it doesn't exist
				LONG CreateResult = RegCreateKeyExW(HKEY_CURRENT_USER, *RegistryKeyPath, 0, nullptr, 0, KEY_READ | KEY_WRITE | KEY_NOTIFY, nullptr, &DeepLinkKeyHandle, nullptr);
				if (CreateResult != ERROR_SUCCESS)
				{
					// Failed to open or create the key, log error and retry after delay
					IMTBL_ERR("Failed to open or create registry key: %s (Error: %d - %s)", *RegistryKeyPath, CreateResult, *GetWindowsErrorMessage(CreateResult));
					FPlatformProcess::Sleep(1.0f);
					continue;
				}
			}
			bKeyOpened = true;

			// Set up registry change notification
			LONG NotifyResult = RegNotifyChangeKeyValue(DeepLinkKeyHandle, false, REG_NOTIFY_CHANGE_LAST_SET, EventHandle, true);
			if (NotifyResult != ERROR_SUCCESS)
			{
				CloseRegistryKey();
				FPlatformProcess::Sleep(1.0f);
				continue;
			}
		}

		// Wait for the registry change event or termination event
		DWORD WaitResult = WaitForMultipleObjects(2, WaitHandles, false, INFINITE);

		if (WaitResult == WAIT_OBJECT_0) // Registry change event
		{
			// Event was signaled, check the registry for deep link
			if (DeepLinkKeyHandle && !PKCEData.IsStale() && bShouldRun)
			{
				// Use Game Thread to process the deeplink
				FFunctionGraphTask::CreateAndDispatchWhenReady([this]()
				{
					if (!PKCEData.IsStale())
					{
						UImmutablePKCEWindows::CheckDeepLinkRegistry(PKCEData.Get(), DeepLinkKeyHandle);
					}
				}, TStatId(), nullptr, ENamedThreads::GameThread);
			}

			// Reset notification for the next change
			if (bKeyOpened && bShouldRun)
			{
				LONG notifyResult = RegNotifyChangeKeyValue(DeepLinkKeyHandle, false, REG_NOTIFY_CHANGE_LAST_SET, EventHandle, true);
				if (notifyResult != ERROR_SUCCESS)
				{
					CloseRegistryKey();
				}
			}
		}
		else if (WaitResult == WAIT_OBJECT_0 + 1) // Termination event
		{
			IMTBL_LOG("Registry watcher received termination signal");
			break;
		}
		else if (WaitResult == WAIT_FAILED)
		{
			IMTBL_ERR("WaitForMultipleObjects failed with error: %d - %s", GetLastError(), *GetWindowsErrorMessage(GetLastError()));
			if (bKeyOpened)
			{
				CloseRegistryKey();
			}
			FPlatformProcess::Sleep(0.5f);
		}
		else
		{
			// Any other result is unexpected
			IMTBL_WARN("Unexpected result from WaitForMultipleObjects: %d", WaitResult);
			if (bKeyOpened)
			{
				CloseRegistryKey();
			}
			FPlatformProcess::Sleep(0.5f);
		}
	}

	return 0;
}

void FRegistryWatcherRunnable::Stop()
{
	bShouldRun = false;

	// Signal termination event to wake up the thread
	if (TerminateEventHandle)
	{
		SetEvent(TerminateEventHandle);
	}
}

void FRegistryWatcherRunnable::Exit()
{
	// Clean up when thread exits
	if (DeepLinkKeyHandle)
	{
		RegCloseKey(DeepLinkKeyHandle);
		DeepLinkKeyHandle = nullptr;
	}
}

UImmutablePKCEData* UImmutablePKCEWindows::Initialise(const FImmutablePassportInitData& PassportInitData)
{
	// Extract the protocol scheme from the redirect URI
	FString Protocol;
	if (!ExtractProtocolFromUri(PassportInitData.redirectUri, Protocol))
	{
		IMTBL_ERR("Failed to extract protocol from redirect URI: %s", *PassportInitData.redirectUri);
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

	// Setup registry watcher thread for deep links
	FString RegistryKeyPath = GetDeepLinkRegistryKeyPath(Protocol);

	// Clean up any existing thread for this protocol
	CleanupThreadForProtocol(Protocol);

	// Create new runnable and start its thread
	TWeakObjectPtr<UImmutablePKCEData> WeakPKCEData(PKCEData);
	TUniquePtr<FRegistryWatcherRunnable> Runnable = MakeUnique<FRegistryWatcherRunnable>(RegistryKeyPath, WeakPKCEData);

	if (!Runnable->Init())
	{
		IMTBL_ERR("Failed to initialize registry watcher for protocol: %s", *Protocol);
		return PKCEData;
	}

	if (!Runnable->StartThread(Protocol))
	{
		IMTBL_ERR("Failed to start registry watcher thread for protocol: %s", *Protocol);
		return PKCEData;
	}

	// Store the runnable in the map
	GRunnableMap.Add(Protocol, MoveTemp(Runnable));

	return PKCEData;
}

bool UImmutablePKCEWindows::RegisterProtocolHandler(const FString& Protocol)
{
	if (Protocol.IsEmpty())
	{
		IMTBL_ERR("Cannot register protocol handler: Protocol is empty");
		return false;
	}

	// Create a PowerShell script that will handle incoming protocol activations
	FString PowerShellFilePath = GetPowerShellScriptPath(Protocol);
	FString LogFilePath = GetLogFilePath(Protocol);

	auto CleanupResources = [&](bool bDeletePowerShellScript, bool bDeleteRegistryKey)
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
		LONG Result = RegCreateKeyExW(HKEY_CURRENT_USER, *KeyPath, 0, nullptr, 0, KEY_WRITE, nullptr, &KeyHandle, nullptr);
		if (Result != ERROR_SUCCESS)
		{
			IMTBL_ERR("Failed to create registry key for protocol handler (Error: %d - %s)", Result, *GetWindowsErrorMessage(Result));
			CleanupResources(true, false);
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
			CleanupResources(true, true);
			return false;
		}
	}

	// Setup shell key with empty default value
	{
		FString ShellKeyPath = FString::Printf(TEXT("%s\\shell"), *KeyPath);
		HKEY ShellKeyHandle;
		LONG Result = RegCreateKeyExW(HKEY_CURRENT_USER, *ShellKeyPath, 0, nullptr, 0, KEY_WRITE, nullptr, &ShellKeyHandle, nullptr);
		if (Result != ERROR_SUCCESS)
		{
			IMTBL_ERR("Failed to create shell registry key (Error: %d - %s)", Result, *GetWindowsErrorMessage(Result));
			CleanupResources(true, true);
			return false;
		}

		FString EmptyValue = TEXT(" "); // Arbitrary value so that it will use the root for the deep link prompt
		LONG SetResult = RegSetValueExW(ShellKeyHandle, nullptr, 0, REG_SZ, (const BYTE*)*EmptyValue, (EmptyValue.Len() + 1) * sizeof(TCHAR));
		bool bSuccess = SetResult == ERROR_SUCCESS;

		RegCloseKey(ShellKeyHandle);

		if (!bSuccess)
		{
			IMTBL_ERR("Failed to set shell registry value (Error: %d - %s)", SetResult, *GetWindowsErrorMessage(SetResult));
			CleanupResources(true, true);
			return false;
		}
	}

	// Setup command to execute when protocol is activated in registry
	{
		FString CommandKeyPath = FString::Printf(TEXT("%s\\shell\\open\\command"), *KeyPath);
		HKEY CommandKeyHandle;
		LONG Result = RegCreateKeyExW(HKEY_CURRENT_USER, *CommandKeyPath, 0, nullptr, 0, KEY_WRITE, nullptr, &CommandKeyHandle, nullptr);
		if (Result != ERROR_SUCCESS)
		{
			IMTBL_ERR("Failed to create command registry key (Error: %d - %s)", Result, *GetWindowsErrorMessage(Result));
			CleanupResources(true, true);
			return false;
		}

		LONG SetResult = RegSetValueExW(CommandKeyHandle, nullptr, 0, REG_SZ, (const BYTE*)*CommandLine, (CommandLine.Len() + 1) * sizeof(TCHAR));
		bool bSuccess = SetResult == ERROR_SUCCESS;

		RegCloseKey(CommandKeyHandle);

		if (!bSuccess)
		{
			IMTBL_ERR("Failed to set command registry value (Error: %d - %s)", SetResult, *GetWindowsErrorMessage(SetResult));
			CleanupResources(true, true);
			return false;
		}
	}

	return true;
}

bool UImmutablePKCEWindows::CheckDeepLinkRegistry(UImmutablePKCEData* PKCEData, HKEY DeepLinkKeyHandle)
{
	if (!PKCEData || !DeepLinkKeyHandle)
	{
		return false;
	}

	DWORD ValueType;
	DWORD DataSize = 0;

	// First query to get size of the registry value data
	LONG QueryResult = RegQueryValueExW(DeepLinkKeyHandle, DeepLinkRegistryValueName, nullptr, &ValueType, nullptr, &DataSize);
	if (QueryResult == ERROR_SUCCESS && ValueType == REG_SZ && DataSize > 0)
	{
		TArray<TCHAR> Buffer;
		Buffer.SetNumZeroed(DataSize / sizeof(TCHAR) + 1);

		// Second query to get actual data content
		LONG DataResult = RegQueryValueExW(DeepLinkKeyHandle, DeepLinkRegistryValueName, nullptr, nullptr, (BYTE*)Buffer.GetData(), &DataSize);
		if (DataResult == ERROR_SUCCESS)
		{
			FString DeepLink = Buffer.GetData();

			if (!DeepLink.IsEmpty())
			{
				IMTBL_LOG("Found deep link in registry: %s", *DeepLink);

				// Clear the registry value to avoid processing the same deep link multiple times
				FString EmptyValue = TEXT("");
				RegSetValueExW(DeepLinkKeyHandle, DeepLinkRegistryValueName, 0, REG_SZ, (const BYTE*)*EmptyValue, (EmptyValue.Len() + 1) * sizeof(TCHAR));

				HandleDeepLink(PKCEData, DeepLink);
				return true;
			}
		}
		else
		{
			IMTBL_WARN("Failed to read deep link registry value data (Error: %d - %s)", DataResult, *GetWindowsErrorMessage(DataResult));
		}
	}
	else if (QueryResult != ERROR_FILE_NOT_FOUND)
	{
		// Only log error if it's not due to missing value (which is normal)
		IMTBL_WARN("Failed to query deep link registry value size (Error: %d - %s)", QueryResult, *GetWindowsErrorMessage(QueryResult));
	}

	return false;
}

void UImmutablePKCEWindows::HandleDeepLink(UImmutablePKCEData* PKCEData, const FString& DeepLink)
{
	if (!PKCEData)
	{
		IMTBL_ERR("Cannot handle deep link: PKCEData is nullptr");
		return;
	}

	// Clean up the deep link URL by removing quotes and extra whitespace
	FString CleanDeepLink = DeepLink.TrimStartAndEnd().Replace(TEXT("\""), TEXT(""));
	IMTBL_LOG("Handle Deep Link: %s", *CleanDeepLink);

	// Verify the deep link matches one of our expected redirect URIs before broadcasting
	if (CleanDeepLink.StartsWith(PKCEData->PassportInitData.redirectUri) || CleanDeepLink.StartsWith(PKCEData->PassportInitData.logoutRedirectUri))
	{
		PKCEData->DynamicMulticastDelegate_DeepLinkCallback.Broadcast(CleanDeepLink);

		// After PKCE authentication completes, return focus to the game window
		if (GEngine && GEngine->GameViewport && GEngine->GameViewport->GetWindow().IsValid())
		{
			TSharedPtr<SWindow> GameWindow = GEngine->GameViewport->GetWindow();
			GameWindow->BringToFront();
			GameWindow->HACK_ForceToFront();

			IMTBL_LOG("Game window brought to foreground after authentication completed");
		}
		else
		{
			IMTBL_WARN("Could not bring game window to foreground: window is not valid");
		}
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

	// Extract the protocol scheme from the redirect URI
	FString Protocol;
	if (!ExtractProtocolFromUri(PKCEData->PassportInitData.redirectUri, Protocol))
	{
		IMTBL_WARN("Failed to extract protocol from URI during reset: %s", *PKCEData->PassportInitData.redirectUri);
		return;
	}

	// Clean up registry watcher thread
	CleanupThreadForProtocol(Protocol);

// In shipping builds, clean up protocol handler registry entries and temp files
// In non-shipping builds, skip cleanup for easier testing and debugging
#if UE_BUILD_SHIPPING
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
	LSTATUS DeleteResult = RegDeleteTreeW(HKEY_CURRENT_USER, *ProtocolKeyPath);
	if (DeleteResult == ERROR_SUCCESS)
	{
		IMTBL_LOG("Deleted protocol handler registry key: %s", *ProtocolKeyPath);
	}
	else if (DeleteResult != ERROR_FILE_NOT_FOUND)
	{
		// Only log error if key exists but couldn't be deleted
		IMTBL_WARN("Failed to delete protocol handler registry key: %s (Error code: %d - %s)", *ProtocolKeyPath, DeleteResult, *GetWindowsErrorMessage(DeleteResult));
	}

	// Delete the deep link registry key
	FString DeepLinkRegistryKeyPath = GetDeepLinkRegistryKeyPath(Protocol);
	DeleteResult = RegDeleteKeyW(HKEY_CURRENT_USER, *DeepLinkRegistryKeyPath);
	if (DeleteResult == ERROR_SUCCESS)
	{
		IMTBL_LOG("Deleted deep link registry key: %s", *DeepLinkRegistryKeyPath);
	}
	else if (DeleteResult != ERROR_FILE_NOT_FOUND)
	{
		// Only log error if key exists but couldn't be deleted
		IMTBL_WARN("Failed to delete deep link registry key: %s (Error code: %d - %s)", *DeepLinkRegistryKeyPath, DeleteResult, *GetWindowsErrorMessage(DeleteResult));
	}

	IMTBL_LOG("PKCE cleanup completed for protocol: %s", *Protocol);
#endif

	auto ResetPKCEData = [](UImmutablePKCEData* PKCEData)
	{
		PKCEData->DynamicMulticastDelegate_DeepLinkCallback.Clear();
		PKCEData->PassportInitData = {};
	};

	ResetPKCEData(PKCEData);
}

FString UImmutablePKCEWindows::GetProtocolDirectory(const FString& Protocol)
{
	if (Protocol.IsEmpty())
	{
		IMTBL_WARN("Empty protocol provided to GetProtocolDirectory, using default");
		return FPaths::ProjectSavedDir() / TEXT("Immutable") / TEXT("DefaultProtocol");
	}

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
		IMTBL_WARN("System TEMP directory not found, using project saved directory instead");
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
	FString ProtocolDir = FPaths::ConvertRelativePathToFull(BaseDir / Protocol);
	if (!FPlatformFileManager::Get().GetPlatformFile().DirectoryExists(*ProtocolDir))
	{
		if (!FPlatformFileManager::Get().GetPlatformFile().CreateDirectoryTree(*ProtocolDir))
		{
			IMTBL_WARN("Failed to create protocol directory: %s", *ProtocolDir);
		}
	}

	return ProtocolDir;
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
	if (Uri.IsEmpty())
	{
		IMTBL_ERR("Cannot extract protocol: URI is empty");
		return false;
	}

	FString Right;
	if (!Uri.Split(TEXT("://"), &OutProtocol, &Right))
	{
		IMTBL_ERR("Failed to extract protocol from URI: %s", *Uri);
		return false;
	}

	if (OutProtocol.IsEmpty())
	{
		IMTBL_ERR("Extracted protocol is empty from URI: %s", *Uri);
		return false;
	}

	return true;
}