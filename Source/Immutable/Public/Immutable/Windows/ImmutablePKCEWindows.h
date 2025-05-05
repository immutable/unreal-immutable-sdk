#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"

#include "Windows/WindowsHWrapper.h"

#include "Immutable/ImmutableDataTypes.h"

#include "ImmutablePKCEWindows.generated.h"

/**
 * Self-contained registry watcher that creates and manages its own thread
 * Monitors the registry for deep link callbacks and processes them
 */
class FRegistryWatcherRunnable : public FRunnable
{
public:
	FRegistryWatcherRunnable(const FString& InRegistryKeyPath, TWeakObjectPtr<UImmutablePKCEData> InPKCEData);
	virtual ~FRegistryWatcherRunnable() override;

	/** FRunnable: @Interface @Begin */
	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Stop() override;
	virtual void Exit() override;
	/** FRunnable: @Interface @End */

	/**
	 * Starts the watcher thread
	 * @param ProtocolName Name of the protocol for thread identification
	 * @return True if thread was started successfully
	 */
	bool StartThread(const FString& ProtocolName);

private:
	FString RegistryKeyPath;
	TWeakObjectPtr<UImmutablePKCEData> PKCEData;
	volatile bool bShouldRun = true;

	HANDLE EventHandle;
	HANDLE TerminateEventHandle;
	HKEY DeepLinkKeyHandle = nullptr;

	// Thread that runs this runnable
	TUniquePtr<FRunnableThread> Thread;
};

/**
 * Windows platform-specific implementation for PKCE authentication flow
 * Handles custom URI protocol registration and deep link processing
 */
UCLASS()
class IMMUTABLE_API UImmutablePKCEWindows : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * Initialize the Windows deep linking functionality
	 * 
	 * @param PassportInitData The initialization data containing redirect URIs and client configuration
	 * @return Pointer to the initialized PKCE data object or nullptr if initialization failed
	 */
	UFUNCTION(BlueprintCallable, Category = "Immutable|PKCE|Windows")
	static UImmutablePKCEData* Initialise(const FImmutablePassportInitData& PassportInitData);

	/**
	 * Register protocol handler for custom URI scheme in Windows registry
	 * Creates necessary registry entries and PowerShell helper scripts
	 *
	 * @param Protocol The protocol to register (e.g., "immutable")
	 * @return True if registration was successful, false if failed due to invalid protocol or registry errors
	 */
	UFUNCTION(BlueprintCallable, Category = "Immutable|PKCE|Windows")
	static bool RegisterProtocolHandler(const FString& Protocol);

	/**
	 * Process a deep link URL and trigger the appropriate callback
	 * Parses the incoming URI and extracts authentication parameters
	 * 
	 * @param PKCEData The PKCE data object containing callback information; if nullptr, function will return early
	 * @param DeepLink The deep link URL to handle with authorization code and state
	 */
	UFUNCTION(BlueprintCallable, Category = "Immutable|PKCE|Windows")
	static void HandleDeepLink(UImmutablePKCEData* PKCEData, const FString& DeepLink);

	/**
	 * Check the registry for deep link values using a provided registry key handle
	 * Called by the registry watcher thread when registry changes are detected
	 * 
	 * @param PKCEData The PKCE data object containing callback information
	 * @param DeepLinkKeyHandle An open registry key handle with appropriate permissions
	 * @return True if a deep link was found and processed, false if invalid parameters or no deep link found
	 */
	static bool CheckDeepLinkRegistry(UImmutablePKCEData* PKCEData, HKEY DeepLinkKeyHandle);

	/**
	 * Reset and clean up PKCE operation footprint
	 * Stops any registry watcher threads and cleans up resources
	 * 
	 * @param PKCEData The PKCE data object to clean up; if nullptr, function will return early
	 */
	UFUNCTION(BlueprintCallable, Category = "Immutable|PKCE|Windows")
	static void Reset(UImmutablePKCEData* PKCEData);

protected:
	/**
	 * Get the PowerShell script file path for a protocol
	 * This script handles deep link registration and browser-to-app communication
	 *
	 * @param Protocol The protocol to get the script path for
	 * @return The full path to the PowerShell script file in the protocol-specific directory
	 */
	UFUNCTION(BlueprintCallable, Category = "Immutable|PKCE|Windows")
	static FString GetPowerShellScriptPath(const FString& Protocol);

	/**
	 * Get the log file path for a protocol
	 * Used to monitor and debug deep link communication
	 *
	 * @param Protocol The protocol to get the log path for
	 * @return The full path to the log file in the protocol-specific directory
	 */
	UFUNCTION(BlueprintCallable, Category = "Immutable|PKCE|Windows")
	static FString GetLogFilePath(const FString& Protocol);

	/**
	 * Creates and returns a protocol-specific directory for storing PKCE-related files
	 * The location varies based on build configuration:
	 *  - In shipping builds: Uses the system temp directory (%TEMP%)
	 *  - In non-shipping builds: Uses a plugin subdirectory in the project's saved folder
	 * 
	 * @param Protocol The protocol to create a directory for; if empty, returns a default path
	 * @return The full path to a directory dedicated to the specified protocol
	 */
	static FString GetProtocolDirectory(const FString& Protocol);

	/**
	 * Get the registry key path for protocol handler registration
	 * Windows uses these registry entries to route custom URI schemes
	 *
	 * @param Protocol The protocol to get the registry key path for
	 * @return The full Windows registry key path for protocol registration
	 */
	UFUNCTION(BlueprintCallable, Category = "Immutable|PKCE|Windows")
	static FString GetProtocolRegistryKeyPath(const FString& Protocol);

	/**
	 * Get the registry key path for deep link storage
	 * Used to store and retrieve incoming authentication callbacks
	 *
	 * @param Protocol The protocol to get the registry key path for
	 * @return The registry key path where deep links are temporarily stored
	 */
	UFUNCTION(BlueprintCallable, Category = "Immutable|PKCE|Windows")
	static FString GetDeepLinkRegistryKeyPath(const FString& Protocol);

	/**
	 * Extract the protocol scheme from a URI
	 * Parses URIs like "immutable://callback?code=abc123&state=xyz789" to extract just the protocol name "immutable"
	 * The extracted protocol is used to locate the corresponding registry entries and handlers
	 *
	 * @param Uri The URI to extract the protocol from
	 * @param OutProtocol The output parameter to store the extracted protocol
	 * @return True if extraction was successful, false if URI is empty or malformed
	 */
	UFUNCTION(BlueprintCallable, Category = "Immutable|PKCE|Windows")
	static bool ExtractProtocolFromUri(const FString& Uri, FString& OutProtocol);
};