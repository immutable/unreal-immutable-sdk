#pragma once

#include "ImmutableEnums.h"
#include "ImmutableNames.h"

#include "ApplicationConfig.generated.h"

/**
 * @class UApplicationConfig
 * @brief Configuration settings for Passport and various APIs.
 * @details This class stores configuration settings such as client IDs and environment settings for Passport.
 */
UCLASS(Abstract, Blueprintable, ClassGroup = Immutable)
class IMMUTABLE_API UApplicationConfig : public UObject
{
	GENERATED_BODY()

public:
	/**
	 * Retrieves the Client ID used for Passport initialization.
	 *
	 * @return A constant reference to an FString containing the Client ID.
	 */
	const FString& GetClientID()
	{
		return ClientID;	
	}

	/**
	 * Retrieves the environment configuration used for Passport initialization as FString .
	 *
	 * @return A constant reference to an FString representing the environment.
	 */
	const FString& GetEnvironmentString() const
	{
		switch (Environment)
		{
		case EImmutableEnvironment::Production:
			return ImmutablePassportEnvironmentConstants::EnvironmentProduction;
		default:
		case EImmutableEnvironment::Sandbox:
			return ImmutablePassportEnvironmentConstants::EnvironmentProduction;
		}
	}

	/**
	 * Retrieves the URL where the browser will redirect after successful authentication.
	 * @note This is only used for Android, iOS, and macOS.
	 *
	 * @return A constant reference to an FString containing the redirect URL.
	 */
	const FString& GetRedirectURL()
	{
		return RedirectURL;
	}

	/**
	 * Retrieves the URL used for logging out.
	 *
	 * @return A constant reference to an FString containing the logout URL.
	 */
	const FString& GetLogoutURL()
	{
		return LogoutURL;
	}

protected:
	/** Passport Client ID. */
	UPROPERTY(EditDefaultsOnly, Category = "Passport")
	FString ClientID;

	/**
	 * Environment used to initialize passport. Ex. sandbox or production.
	 * @note The default environment is set to the Sandbox environment.
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Passport")
	EImmutableEnvironment Environment = EImmutableEnvironment::Sandbox;

	/**
	 * (Android, iOS, and macOS only)
	 * The URL where the browser will redirect after successful authentication.
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Passport")
	FString RedirectURL;

	/** The URL where the browser will redirect after logout is complete. */
	UPROPERTY(EditDefaultsOnly, Category = "Passport")
	FString LogoutURL;
	
};
