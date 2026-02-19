#pragma once

#include "Misc/EngineVersion.h"

#include "Immutable/ImtblJSMessages.h"
#include "Immutable/ImmutableNames.h"

#include "ImmutableDataTypes.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FImmutableSimpleDynamicMulticastDelegate);

DECLARE_MULTICAST_DELEGATE_OneParam(FImmutableMessageMulticastDelegate, const FString& /* Message */);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FImmutableMessageDynamicMulticastDelegate, const FString&, Message);

DECLARE_MULTICAST_DELEGATE_OneParam(FImmutableDeepLinkMulticastDelegate, const FString& /** DeepLink */);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FImmutableDeepLinkDynamicMulticastDelegate, const FString&, DeepLink);

// This is the version of the Unreal Immutable SDK that is being used. This is not the version of the engine.
// This hardcoded value will be updated by a workflow during the release process.
#define ENGINE_SDK_VERSION TEXT("1.11.2")

/**
 * Enum representing marketing consent status for authentication
 */
UENUM(BlueprintType)
enum class EImmutableMarketingConsentStatus : uint8
{
	Opted_In,
	Unsubscribed,
	Subscribed
};

/**
 * Enum representing direct login methods for authentication providers
 */
UENUM(BlueprintType)
enum class EImmutableDirectLoginMethod : uint8
{
	Email,
	Google,
	Apple,
	Facebook
};

/**
 * Structure representing direct login options for authentication
 * Can be used for social login (google, apple, facebook) or email login
 */
USTRUCT(BlueprintType)
struct IMMUTABLE_API FImmutableDirectLoginOptions
{
	GENERATED_BODY()

	bool IsEmailValid() const;

	TSharedPtr<FJsonObject> ToJsonObject() const;

	/** Direct login method for authentication */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EImmutableDirectLoginMethod DirectLoginMethod = EImmutableDirectLoginMethod::Email;

	/** Email address for email-based authentication (only used when DirectLoginMethod is Email) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Email;

	/** Marketing consent status for authentication (defaults to opted in) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EImmutableMarketingConsentStatus MarketingConsentStatus = EImmutableMarketingConsentStatus::Opted_In;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ImPassportTraceId;
};

UCLASS()
class UImmutableDirectLoginOptionsStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	static bool FromJSResponse(const FImtblJSResponse& Response, FImmutableDirectLoginOptions& DirectLoginOptions);
};

USTRUCT()
struct FImmutableEngineVersionData
{
	GENERATED_BODY()

	UPROPERTY()
	FString engine = TEXT("unreal");

	UPROPERTY()
	FString engineSdkVersion = ENGINE_SDK_VERSION;

	// cannot have spaces
	UPROPERTY()
	FString engineVersion = FEngineVersion::Current().ToString().Replace(TEXT(" "), TEXT("_"));

	// cannot have spaces
	UPROPERTY()
	FString platform = FString(FPlatformProperties::IniPlatformName()).Replace(TEXT(" "), TEXT("_"));

	// cannot have spaces
	UPROPERTY()
	FString platformVersion = FPlatformMisc::GetOSVersion().Replace(TEXT(" "), TEXT("_"));

	// Information on device. Examples of expected results: Apple|Apple M3 Max, GenuineIntel|13th Gen Intel(R) Core(TM) i7-13700H, Apple|iPhone15&#44;4.
	UPROPERTY()
	FString deviceModel = FGenericPlatformMisc::GetDeviceMakeAndModel();
};

/**
 * Structure to hold initialisation data for the Immutable Passport.
 */
USTRUCT(BlueprintType)
struct IMMUTABLE_API FImmutablePassportInitData
{
	GENERATED_BODY()

	/** The Client ID. */
	UPROPERTY()
	FString clientId;

	/**
	 * (Android, iOS, and macOS only)
	 * The URL where the browser will redirect after successful authentication.
	 */
	UPROPERTY()
	FString redirectUri;

	/** The URL where the browser will redirect after logout is complete. */
	UPROPERTY()
	FString logoutRedirectUri;

	/** The environment to connect to.
	 * @note Default value is "sandbox"
	 */
	UPROPERTY()
	FString environment = ImmutablePassportEnvironmentConstants::EnvironmentSandbox;

	/**
	 * Whether silent logout is enabled.
	 * If true, logout silently (without popping up a new browser tab)
	 */
	UPROPERTY()
	bool isSilentLogout = false;

	/** Information about engine version */
	UPROPERTY()
	FImmutableEngineVersionData engineVersion;

	/**
	 * Converts the FImmutablePassportInitData structure to a JSON string representation.
	 *
	 * @return 	A JSON string representation of the FImmutablePassportInitData structure.
	 * 			Returns an empty string if the conversion fails.
	 */
	FString ToJsonString() const;
};

USTRUCT()
struct FImtblUserProfile
{
	GENERATED_BODY()

	UPROPERTY()
	FString email;

	UPROPERTY()
	FString nickname;

	UPROPERTY()
	FString sub;
};

USTRUCT()
struct IMMUTABLE_API FImmutablePassportZkEvmRequestAccountsData
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<FString> accounts;

	FString ToJsonString() const;
	static TOptional<FImmutablePassportZkEvmRequestAccountsData> FromJsonString(const FString& JsonObjectString);
	static TOptional<FImmutablePassportZkEvmRequestAccountsData> FromJsonObject(const TSharedPtr<FJsonObject>& JsonObject);
};

USTRUCT()
struct IMMUTABLE_API FImmutablePassportZkEvmGetBalanceData
{
	GENERATED_BODY()

	UPROPERTY()
	FString address;

	UPROPERTY()
	FString blockNumberOrTag = "latest";

	FString ToJsonString() const;
};

USTRUCT()
struct FImmutablePassportConnectData
{
	GENERATED_BODY()

	UPROPERTY()
	FString authorizationCode;

	UPROPERTY()
	FString state;
};

/**
 * Structure to hold PKCE authentication URL request data
 */
USTRUCT()
struct IMMUTABLE_API FImmutableGetPKCEAuthUrlRequest
{
	GENERATED_BODY()

	/** Whether this is a ConnectImx operation (true) or just Login (false) */
	UPROPERTY()
	bool isConnectImx = false;

	/** Direct login options for authentication */
	UPROPERTY()
	FImmutableDirectLoginOptions directLoginOptions;
};

USTRUCT()
struct IMMUTABLE_API FImmutablePassportResult
{
	GENERATED_BODY()

	FString ToJsonString() const;

	/** Whether the response was successful. */
	UPROPERTY()
	bool Success = false;

	/** Error string for the response. */
	UPROPERTY()
	FString Error;

	/** Response payload. */
	UPROPERTY()
	FImtblJSResponse Response;
};

USTRUCT(BlueprintType)
struct FImtblAccessListItem
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString address;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FString> storageKeys;
};

USTRUCT(BlueprintType)
struct FNftTransferDetails
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FString receiver;

	UPROPERTY(BlueprintReadWrite)
	FString tokenId;

	UPROPERTY(BlueprintReadWrite)
	FString tokenAddress;
};

USTRUCT(BlueprintType)
struct IMMUTABLE_API FZkEvmTransactionReceiptLog
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString _type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString address;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString blockHash;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString blockNumber;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString data;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString index;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FString> topics;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString transactionHash;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString transactionIndex;
};

USTRUCT(BlueprintType)
struct IMMUTABLE_API FZkEvmTransactionReceipt
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString responseFor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString requestId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool success = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString blockHash;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString blockNumber;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString contractAddress;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString cumulativeGasUsed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString from;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString gasPrice;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString blobGasUsed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString blobGasPrice;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString gasUsed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString hash;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString index;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FZkEvmTransactionReceiptLog> logs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString logsBloom;

	// Either 1 (success) or 0 (failure) encoded as a hexadecimal.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString status;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString to;
};

/**
 * Data for PKCE deep linking
 */
UCLASS(BlueprintType, DisplayName = "Immutable PKCE Data")
class IMMUTABLE_API UImmutablePKCEData : public UObject
{
	GENERATED_BODY()

public:
	/** UObject: @Interface @Begin */
	virtual void BeginDestroy() override;
	/** UObject: @Interface @End */

	/**
	 * Reset and clean up PKCE operation footprint
	 * Automatically called during object destruction
	 */
	UFUNCTION(BlueprintCallable, Category = "Immutable|Passport")
	void Reset();

public:
	/** Passport initialization data */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FImmutablePassportInitData PassportInitData;

	/**
	 * Delegate triggered when a deep link callback is received from the browser
	 * Contains the complete URI with authorization code and state parameters
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintAssignable, DisplayName = "Deep Link Callback")
	FImmutableDeepLinkDynamicMulticastDelegate DynamicMulticastDelegate_DeepLinkCallback;

	/**
	 * Handle for the ticker delegate that periodically checks for incoming deep links
	 */
	FTSTicker::FDelegateHandle TickDelegateHandle;
};
