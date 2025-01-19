#pragma once

#include "OnRampWidgetConfig.generated.h"


UENUM(BlueprintType)
enum class ETransakEnvironment : uint8
{
	Sandbox,
	Production,
};

/**
 * @class UOnRampWidgetConfig
 * @brief Configuration settings for on ramp widget.
 */
UCLASS(Abstract, Blueprintable, ClassGroup = Immutable)
class IMMUTABLEMARKETPLACE_API UOnRampWidgetConfig : public UObject
{
	GENERATED_BODY()

public:
	/**
	 * Get the URL based on the current environment setting.
	 * @return The URL corresponding to the current environment setting.
	 */
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

	/**
	 * Get the API key based on the current environment.
	 * @return The API key corresponding to the current environment.
	 */
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

	/**
	 * @details More details could be found under the class parameter   
	 * @return Network as FString
	 */
	const FString& GetNetwork()
	{
		return Network;
	}

	/**
	 * @details More details could be found under the class parameter   
	 * @return DefaultFiatCurrency as FString
	 */
	const FString& GetDefaultFiatCurrency()
	{
		return DefaultFiatCurrency;
	}

	/**
	 * @details More details could be found under the class parameter   
	 * @return DefaultFiatAmount as FString
	 */
	const FString& GetDefaultFiatAmount()
	{
		return DefaultFiatAmount;
	}

	/**
	 * @details More details could be found under the class parameter   
	 * @return DefaultCryptoCurrency as FString
	 */
	const FString& GetDefaultCryptoCurrency()
	{
		return DefaultCryptoCurrency;
	}

	/**
	 * @details More details could be found under the class parameter   
	 * @return DefaultPaymentMethod as FString
	 */
	const FString& GetDefaultPaymentMethod()
	{
		return DefaultPaymentMethod;
	}

	/**
	 * @details More details could be found under the class parameter   
	 * @return DisablePaymentMethods as array of FString
	 */
	const TArray<FString>& GetDisablePaymentMethods()
	{
		return DisablePaymentMethods;
	}

	/**
	 * @details More details could be found under the class parameter   
	 * @return bIsAutoFillUserData as bool
	 */
	bool IsAutoFillUserData()
	{
		return bIsAutoFillUserData;
	}

	/**
	 * @details More details could be found under the class parameter   
	 * @return bDisableWalletAddressForm as bool
	 */
	bool DisableWalletAddressForm()
	{
		return bDisableWalletAddressForm;
	}

	/**
	 * @details More details could be found under the class parameter   
	 * @return CryptoCurrencyList as array of FString
	 */
	const TArray<FString>& GetCryptoCurrencyList()
	{
		return CryptoCurrencyList;
	}

	/**
	 * @details More details could be found under the class parameter   
	 * @return ThemeColor as FLinearColor
	 */
	const FLinearColor& GetThemeColor()
	{
		return ThemeColor;
	}

protected:
	/**
	 * Specifies the environment for transactions.
	 * @note The default environment is set to the Sandbox environment.
	 */
	UPROPERTY(EditDefaultsOnly, Category = "General")
	ETransakEnvironment Environment = ETransakEnvironment::Sandbox;

	/**
	* The default payment method you would prefer the customer to buy/sell with.
	* If you pass this param, the payment method will be selected by default and the customer can
	* also select another payment method.
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Fiat")
	FString DefaultPaymentMethod;

	/**
	 * The three letter code of the fiat currency your user will send/receive while buying/selling cryptocurrency.
	 * Users can change the fiat currency if this is passed. If the fiat currency is not supported by
	 * a specific product type (BUY/SELL) then the default widget will load with all the supported fiat
	 * currencies for that product type.
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Fiat")
	FString DefaultFiatCurrency;

	/**
	* An integer amount representing how much the customer wants to spend/receive.
	* Users can change the fiat amount if this is passed.
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Fiat")
	FString DefaultFiatAmount;

	/**
	* The default cryptocurrency you would prefer the customer to buy/sell.
	* If you pass this param, the currency will be selected by default, but the customer will
	* still be able to select another cryptocurrency. Please ensure that the currency code passed by
	* you is available for the specific product type (BUY/SELL).
	* If you pass a value that is not supported by BUY/SELL, then the default widget will load.
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Fiat")
	FString DefaultCryptoCurrency;
	
	/**
	* Crypto network that you would allow your customers to buy.
	* You can get the supporting networks by opening http://global.transak.com and then go to
	* cryptocurrencies select screen. Only the cryptocurrencies supported by this network for
	* the specific product type (BUY/SELL) will be shown in the widget.
	* If the network selected is not supported by a product type (BUY/SELL) then the default widget will
	* all supported networks will be shown.
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Blockchain")
	FString Network;

	/**
	 * A comma-separated list of payment methods you want to disable and hide from the customers.
	 * Refer here to the list of supported params for the payment method.
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Fiat")
	TArray<FString> DisablePaymentMethods;

	/**
	 * When true, then the email address will be auto-filled, but the screen will not be skipped.
	 * User can edit their email address, basic data like first name & the address.
	 * This parameter will be ignored if email or userData are not passed.
	 */
	UPROPERTY(EditDefaultsOnly, Category = "User")
	bool bIsAutoFillUserData = true;

	/**
	* When true, the customer will not be able to change the destination address of
	* where the cryptocurrency is sent to.
	*/
	UPROPERTY(EditDefaultsOnly, Category = "Fiat")
	bool bDisableWalletAddressForm = true;

	/**
	* A comma-separated list of cryptoCurrencies that you would allow your customers to buy/sell.
	* Only these crypto currencies will be shown in the widget. This will be a string of comma
	* separated values each of which will represent a valid cryptoCurrency code.
	* Please ensure that the crypto currency codes passed in the list are available for the specific
	* product type (BUY/SELL). If even one of the crypto currency codes in the list is supported by
	* the specific product type (BUY/SELL), then it will be honored, otherwise the default widget will
	* load for the product type for which none of the crypto currency codes are supported.
	*/
	UPROPERTY(EditDefaultsOnly, Category = "Fiat")
	TArray<FString> CryptoCurrencyList;

	/**
	* The theme color code for the widget main color. It is used for buttons,
	*/
	UPROPERTY(EditDefaultsOnly, Category = "Theme")
	FLinearColor ThemeColor;
};
