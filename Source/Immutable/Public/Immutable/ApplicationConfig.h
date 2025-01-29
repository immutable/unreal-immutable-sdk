#pragma once

#include "ImmutableEnums.h"
#include "ImmutableNames.h"

#include "ApplicationConfig.generated.h"

/**
 * @class UApplicationConfig
 * @brief Configuration settings for Passport and various APIs.
 * @details This class stores configuration settings such as URLs, chain names, contract addresses, 
 * client IDs, and environment settings for the zkEVM API, Orderbook API, and Passport.
 */
UCLASS(Abstract, Blueprintable, ClassGroup = Immutable)
class IMMUTABLE_API UApplicationConfig : public UObject
{
	GENERATED_BODY()

public:
	/**
	 * Retrieves URL for the zkEVM API.
	 *
	 * @return A constant reference to an FString containing the name of the chain.
	 */
	const FString& GetzkEVMAPIURL()
	{
		return zkEVMAPIURL;
	}
	
	/**
	 * Retrieves the name of the chain used to pass to the zkEVM API.
	 *
	 * @return A constant reference to an FString containing the name of the chain.
	 */
	const FString& GetzkEVMAPIChainName()
	{
		return zkEVMAPIChainName;
	}

	/**
	 * Retrieves URL for the Orderbook API.
	 *
	 * @return A constant reference to an FString containing the name of the chain.
	 */
	const FString& GetOrderbookAPIURL()
	{
		return OrederbookAPIURL;
	}
	
	/**
	 * Retrieves the name of the chain used to pass to the Orderbook API.
	 *
	 * @return A constant reference to an FString containing the name of the chain.
	 */
	const FString& GetOrderbookAPIChainName()
	{
		return OrderbookAPIChainName;
	}

	/**
	 * @brief Retrieves the cryptocurrency contract address associated with the user's wallet balance.
	 *
	 * @return A string representing the contract address.
	 */
	const FString& GetTokenBalanceContractAddress()
	{
		return TokenBalanceContractAddress;
	} 

	/**
	 * Retrieves the list of NFT contracts used in the APIs' queries.
	 *
	 * @return A constant reference to an array of strings representing the contracts.
	 */
	const TArray<FString>& GetNFTContractAddresses()
	{
		return NFTContractAddress;
	}

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
	/** The URL for the zkEVM API. */
	UPROPERTY(EditDefaultsOnly, Category = "zkEVM API")
	FString zkEVMAPIURL;

	/** The name of the API chain used by the zkEVM API. */
	UPROPERTY(EditDefaultsOnly, Category = "zkEVM API")
	FString zkEVMAPIChainName;

	/** The URL for the Orderbook API. */
	UPROPERTY(EditDefaultsOnly, Category = "Orderbook API")
	FString OrederbookAPIURL;

	/** The name of the API chain used by Orderbook API. */
	UPROPERTY(EditDefaultsOnly, Category = "Orderbook API")
	FString OrderbookAPIChainName;
	
	/** The address of the cryptocurrency contract in the blockchain. */
	UPROPERTY(EditDefaultsOnly, Category = "Contracts")
	FString TokenBalanceContractAddress;

	/** An array of NFT contract addresses used for searching NFTs in the marketplace or displaying them in the player's inventory. */
	UPROPERTY(EditDefaultsOnly, Category = "Contracts")
	TArray<FString> NFTContractAddress;

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
