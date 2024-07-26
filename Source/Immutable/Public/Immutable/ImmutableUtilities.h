#pragma once


/** A wrapper struct around various Immutable namespace utility and support methods. */
struct IMMUTABLE_API FImmutableUtilities
{
public:
	/**
	 * Loads the JavaScript game bridge file from the Web folder and retrieves its content as a string. The JavaScript game bridge facilitates two-way communication between the Immutable Unreal SDK and the Immutable TypeScript SDK.
	 * 
	 * @param GameBridge	Output the game bridge javascript.
	 * @return True if the game bridge was loaded and false otherwise.
	 */
	static bool LoadGameBridge(FString& GameBridge);
};
