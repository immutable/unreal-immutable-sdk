#pragma once


/** A wrapper struct around various Immutable namespace utility and support methods. */
struct IMMUTABLE_API FImmutableUtilities
{
public:
	/**
	 * Loads the JavaScript game bridge file from the Web folder and retrieves its content as a string. The JavaScript game bridge facilitates two-way communication between the Immutable Unreal SDK and the Immutable TypeScript SDK.
	 * 
	 * @param GameBridge	An FString reference where the JavaScript content will be stored.
	 * @return True if the game bridge content was sucessfully retrieved. Otherwise, false.
	 */
	static bool LoadGameBridge(FString& GameBridge);
};
