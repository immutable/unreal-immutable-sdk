#pragma once

#include "JsonObjectConverter.h"
#include "Immutable/ImtblJSMessages.h"
#include "Immutable/ImmutableNames.h"

#include "ImmutableDataTypes.generated.h"


USTRUCT()
struct FImmutableEngineVersionData
{
	GENERATED_BODY()

	UPROPERTY()
	FString engine = TEXT("unreal");

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

USTRUCT()
struct FImmutablePassportInitData
{
	GENERATED_BODY()

	UPROPERTY()
	FString clientId;

	UPROPERTY()
	FString redirectUri;

	UPROPERTY()
	FString logoutRedirectUri;

	UPROPERTY()
	FString environment = ImmutablePassportAction::EnvSandbox;

	UPROPERTY()
	FImmutableEngineVersionData engineVersion;

	FString ToJsonString() const;
};

USTRUCT()
struct FImmutablePassportInitDeviceFlowData
{
	GENERATED_BODY()

	UPROPERTY()
	FString code;
	
	UPROPERTY()
	FString deviceCode;
	
	UPROPERTY()
	FString url;
	
	UPROPERTY()
	float interval = 0;

	static TOptional<FImmutablePassportInitDeviceFlowData> FromJsonString(const FString& JsonObjectString);
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
struct FImmutablePassportZkEvmRequestAccountsData
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<FString> accounts;

	FString ToJsonString() const;
	static TOptional<FImmutablePassportZkEvmRequestAccountsData> FromJsonString(const FString& JsonObjectString);
	static TOptional<FImmutablePassportZkEvmRequestAccountsData> FromJsonObject(const TSharedPtr<FJsonObject>& JsonObject);
};

USTRUCT()
struct FImmutablePassportZkEvmGetBalanceData
{
	GENERATED_BODY()

	UPROPERTY()
	FString address;

	UPROPERTY()
	FString blockNumberOrTag = "latest";

	FString ToJsonString() const;
};

USTRUCT()
struct FImmutablePassportCodeConfirmRequestData
{
	GENERATED_BODY()

	UPROPERTY()
	FString deviceCode;
	
	UPROPERTY()
	float interval = 5;

	UPROPERTY()
	float timeoutMs = 15 * 60 * 1000;
};

USTRUCT()
struct FImmutablePassportConnectPKCEData
{
	GENERATED_BODY()

	UPROPERTY()
	FString authorizationCode;

	UPROPERTY()
	FString state;
};

USTRUCT()
struct FImmutablePassportResult
{
	GENERATED_BODY()

	UPROPERTY()
	bool Success = false;
	
	UPROPERTY()
	FString Message;

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
