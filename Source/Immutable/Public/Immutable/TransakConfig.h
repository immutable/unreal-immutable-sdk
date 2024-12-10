#pragma once
#include "ImmutableEnums.h"

#include "TransakConfig.generated.h"


UENUM()
enum class ETransakEnvironment : uint8
{
	Sandbox,
	Production,
};

/**
 * @class UTransakConfig
 * @brief Configuration settings for Passport and various APIs.
 * @details This class stores configuration settings such as URLs, chain names, contract addresses, 
 * client IDs, and environment settings for the zkEVM API, Orderbook API, and Passport.
 */
UCLASS(Abstract, Blueprintable, ClassGroup = Immutable)
class UTransakConfig : public UObject
{
	GENERATED_BODY()

public:
	FString GetURL() const
	{
		switch (Environment)
		{
		case ETransakEnvironment::Production:
			return TEXT("https://global.transak.com/");
		default:
		case ETransakEnvironment::Sandbox:
			return TEXT("https://global-stg.transak.com/");
		}
	}

	FString GetAPIKey() const
	{
		switch (Environment)
		{
		case ETransakEnvironment::Production:
			return TEXT("ad1bca70-d917-4628-bb0f-5609537498bc");
		default:
		case ETransakEnvironment::Sandbox:
			return TEXT("d14b44fb-0f84-4db5-affb-e044040d724b");
		}
	}

	const FString& GetNetwork()
	{
		return Network;
	}

	const FString& GetDefaultFiatCurrency()
	{
		return DefaultFiatCurrency;
	}

	const FString& GetDefaultFiatAmount()
	{
		return DefaultFiatAmount;
	}

	const FString& GetDefaultCryptoCurrency()
	{
		return DefaultCryptoCurrency;
	}

	const FString& GetDefaultPaymentMethod()
	{
		return DefaultPaymentMethod;
	}

	const TArray<FString>& GetDisablePaymentMethods()
	{
		return DisablePaymentMethods;
	}

	bool IsAutoFillUserData()
	{
		return bIsAutoFillUserData;
	}

	bool DisableWalletAddressForm()
	{
		return bDisableWalletAddressForm;
	}

	const TArray<FString>& GetCryptoCurrencyList()
	{
		return CryptoCurrencyList;
	}

	const FLinearColor& GetThemeColor()
	{
		return ThemeColor;
	}

protected:
	// UPROPERTY(EditDefaultsOnly, Category = "Transak")
	// FString URL;
	//
	// UPROPERTY(EditDefaultsOnly, Category = "Transak")
	// FString APIKey;

	UPROPERTY(EditDefaultsOnly, Category = "General")
	ETransakEnvironment Environment = ETransakEnvironment::Sandbox;

	UPROPERTY(EditDefaultsOnly, Category = "Fiat")
	FString DefaultPaymentMethod;

	UPROPERTY(EditDefaultsOnly, Category = "Fiat")
	FString DefaultFiatCurrency;

	UPROPERTY(EditDefaultsOnly, Category = "Fiat")
	FString DefaultFiatAmount;

	UPROPERTY(EditDefaultsOnly, Category = "Fiat")
	FString DefaultCryptoCurrency;
	
	UPROPERTY(EditDefaultsOnly, Category = "Transak")
	FString Network;

	UPROPERTY(EditDefaultsOnly, Category = "Fiat")
	TArray<FString> DisablePaymentMethods;

	UPROPERTY(EditDefaultsOnly, Category = "User")
	bool bIsAutoFillUserData = true;

	UPROPERTY(EditDefaultsOnly, Category = "Fiat")
	bool bDisableWalletAddressForm = true;

	UPROPERTY(EditDefaultsOnly, Category = "Fiat")
	TArray<FString> CryptoCurrencyList;

	UPROPERTY(EditDefaultsOnly, Category = "Theme")
	FLinearColor ThemeColor;
};
