#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"

#include "Immutable/ImmutableDataTypes.h"

#include "ImmutablePKCEWindows.generated.h"

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
	 * @return True if registration was successful
	 */
	UFUNCTION(BlueprintCallable, Category = "Immutable|PKCE|Windows")
	static bool RegisterProtocolHandler(const FString& Protocol);

	/**
	 * Periodic function to check for deep links in the registry
	 * Called by the ticker system to detect incoming authentication callbacks
	 * Monitors registry for URI requests and triggers appropriate delegates
	 * 
	 * @param PKCEData The PKCE data object containing callback information
	 * @param DeltaTime Time since last tick
	 * @return True to continue ticking, false to stop monitoring
	 */
	UFUNCTION(BlueprintCallable, Category = "Immutable|PKCE|Windows")
	static bool CheckDeepLinkTick(UImmutablePKCEData* PKCEData, float DeltaTime);

	/**
	 * Process a deep link URL and trigger the appropriate callback
	 * Parses the incoming URI and extracts authentication parameters
	 * 
	 * @param PKCEData The PKCE data object containing callback information
	 * @param DeepLink The deep link URL to handle with authorization code and state
	 */
	UFUNCTION(BlueprintCallable, Category = "Immutable|PKCE|Windows")
	static void HandleDeepLink(UImmutablePKCEData* PKCEData, const FString& DeepLink);

	/**
	 * Reset and clean up PKCE operation footprint
	 * 
	 * @param PKCEData The PKCE data object to clean up
	 */
	UFUNCTION(BlueprintCallable, Category = "Immutable|PKCE|Windows")
	static void Reset(UImmutablePKCEData* PKCEData);

protected:
	/**
	 * Get the PowerShell script file path for a protocol
	 * This script handles deep link registration and browser-to-app communication
	 *
	 * @param Protocol The protocol to get the script path for
	 * @return The full path to the PowerShell script file in the game's saved directory
	 */
	UFUNCTION(BlueprintCallable, Category = "Immutable|PKCE|Windows")
	static FString GetPowerShellScriptPath(const FString& Protocol);

	/**
	 * Get the log file path for a protocol
	 * Used to monitor and debug deep link communication
	 *
	 * @param Protocol The protocol to get the log path for
	 * @return The full path to the log file in the game's saved directory
	 */
	UFUNCTION(BlueprintCallable, Category = "Immutable|PKCE|Windows")
	static FString GetLogFilePath(const FString& Protocol);

	/**
	 * Creates and returns a protocol-specific directory for storing PKCE-related files
	 * The location varies based on build configuration:
	 *  - In shipping builds: Uses the system temp directory (%TEMP%)
	 *  - In non-shipping builds: Uses a plugin subdirectory in the project's saved folder
	 * 
	 * @param Protocol The protocol to create a directory for
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
	 * @return True if extraction was successful
	 */
	UFUNCTION(BlueprintCallable, Category = "Immutable|PKCE|Windows")
	static bool ExtractProtocolFromUri(const FString& Uri, FString& OutProtocol);
};