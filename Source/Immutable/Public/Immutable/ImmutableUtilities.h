#pragma once


/** A wrapper struct around various Immutable namespace utility and support methods. */
struct IMMUTABLE_API FImmutableUtilities
{
public:
	/**
	 * Loads game bridge which is javascript with Immutable functionality. The game bridge is located in the Web folder. 
	 * 
	 * @param GameBridge	Output the game bridge javascript.
	 * @return True if the game bridge was loaded and false otherwise.
	 */
	static bool LoadGameBridge(FString& GameBridge);
};
