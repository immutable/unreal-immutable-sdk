// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Immutable/ImtblJSConnector.h"
#include "UObject/Object.h"
#include "ImmutablePassport.generated.h"


struct FImtblJSResponse;


namespace ImmutablePassportAction
{
    const FString Initialize = TEXT("init");
    const FString CheckStoredCredentials = TEXT("checkStoredCredentials");
    const FString ConnectWithCredentials = TEXT("connectWithCredentials");
    const FString Logout = TEXT("logout");
    const FString Connect = TEXT("connect");
    const FString ConnectEvm = TEXT("connectEvm");
    const FString ZkEvmRequestAccounts = TEXT("zkEvmRequestAccounts");
    const FString ZkEvmGetBalance = TEXT("zkEvmGetBalance");
    const FString ZkEvmSendTransaction = TEXT("zkEvmSendTransaction");
    const FString ConfirmCode = TEXT("confirmCode");
    const FString GetAddress = TEXT("getAddress");
    const FString GetEmail = TEXT("getEmail");
    const FString EnvSandbox = TEXT("sandbox");
    const FString EnvProduction = TEXT("production");
}


USTRUCT()
struct FImmutablePassportResult
{
    GENERATED_BODY()

    UPROPERTY()
    bool Success = false;
    UPROPERTY()
    FString Message;
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
    FString environment = ImmutablePassportAction::EnvSandbox;

    FString ToJsonString() const;
};


USTRUCT()
struct FImmutablePassportTokenData
{
    GENERATED_BODY()

    UPROPERTY()
    FString accessToken;
    UPROPERTY()
    FString refreshToken;
    UPROPERTY()
    FString idToken;
    UPROPERTY()
    FString tokenType;
    UPROPERTY()
    int64 expiresIn = 0;

    FString ToJsonString() const;
    static TOptional<FImmutablePassportTokenData> FromJsonObject(const TSharedPtr<FJsonObject>& JsonObject);
};


USTRUCT()
struct FImmutablePassportConnectData
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
    
    FString ToJsonString() const;
    static TOptional<FImmutablePassportConnectData> FromJsonString(const FString& JsonObjectString);
    static TOptional<FImmutablePassportConnectData> FromJsonObject(const TSharedPtr<FJsonObject>& JsonObject);
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

USTRUCT(BlueprintType)
struct FImtblAccessListItem
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FString address;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    TArray<FString> storageKeys;

    FString ToJsonString() const
    {
        FString OutString;
        FJsonObjectConverter::UStructToJsonObjectString<FImtblAccessListItem>(*this, OutString, 0, 0, 0, nullptr, false);
        return OutString;
    }
};

/**
 * Key Value wrappers for converting to JSON
 */
USTRUCT()
struct FStringCustomData
{
    GENERATED_BODY()

    UPROPERTY()
    FString key;

    UPROPERTY()
    FString value;
};

USTRUCT()
struct FInt64CustomData
{
    GENERATED_BODY()

    UPROPERTY()
    FString key;

    UPROPERTY()
    int64 value;
};

USTRUCT()
struct FFloatCustomData
{
    GENERATED_BODY()

    UPROPERTY()
    FString key;

    UPROPERTY()
    float value;
};

USTRUCT()
struct FBoolCustomData
{
    GENERATED_BODY()

    UPROPERTY()
    FString key;

    UPROPERTY()
    bool value;
};

UENUM(BlueprintType)
enum EImtblCustomDataType { String, Int64, Float, Bool };

/**
 * Blueprint doesn't support any sort of generics or polymorphism. To workaround this select the primitive type for
 * this custom data item and set the corresponding value. This will later be mapped to the proper API structure.
 */
USTRUCT(BlueprintType)
struct FImtblCustomData
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FString key;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FString stringValue;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    int64 intValue;
    
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    float floatValue;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    bool boolValue;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    TEnumAsByte<EImtblCustomDataType> type;

    /**
     * Convert from blueprint structure to the expected API data structure
     */
    TSharedPtr<FJsonObject> ToJsonObject() const
    {
        switch (type)  {
        case String:
            return FJsonObjectConverter::UStructToJsonObject<FStringCustomData>({ key, stringValue });
        case Int64:
            return FJsonObjectConverter::UStructToJsonObject<FInt64CustomData>({ key, intValue });
        case Float:
            return FJsonObjectConverter::UStructToJsonObject<FFloatCustomData>({ key, floatValue });
        case Bool:
            return FJsonObjectConverter::UStructToJsonObject<FBoolCustomData>({ key, boolValue });
        default:;
        }
        return {};
    }
};

USTRUCT(BlueprintType)
struct FImtblTransactionRequest
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FString to;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FString from;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FString nonce;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FString gasLimit;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FString gasPrice;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FString data = "0x";

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FString value;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    int64 chainId;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    int64 type;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    TArray<FImtblAccessListItem> accessList;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FString maxPriorityFeePerGas;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FString maxFeePerGas;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    TArray<FImtblCustomData> customData;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    bool ccipReadEnabled;

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
    
    FString ToJsonString() const;
};


/**
 * 
 */
UCLASS()
class IMMUTABLE_API UImmutablePassport : public UObject
{
    GENERATED_BODY()
    friend class UImmutableSubsystem;

public:
    DECLARE_MULTICAST_DELEGATE(FOnPassportReadyDelegate)
    
    DECLARE_DELEGATE_OneParam(FImtblPassportResponseDelegate, FImmutablePassportResult)

    void Initialize(const FImmutablePassportInitData& InitData, const FImtblPassportResponseDelegate& ResponseDelegate);
    void CheckStoredCredentials(const FImtblPassportResponseDelegate& ResponseDelegate);
    void ConnectSilent(const FImtblPassportResponseDelegate& ResponseDelegate);
    void Logout(const FImtblPassportResponseDelegate& ResponseDelegate);
    void Connect(const FImtblPassportResponseDelegate& ResponseDelegate);
    void ConnectEvm(const FImtblPassportResponseDelegate& ResponseDelegate);
    void ZkEvmRequestAccounts(const FImtblPassportResponseDelegate& ResponseDelegate);
    void ZkEvmGetBalance(const FImmutablePassportZkEvmGetBalanceData& Data, const FImtblPassportResponseDelegate& ResponseDelegate);
    void ZkEvmSendTransaction(const FImtblTransactionRequest& Request, const FImtblPassportResponseDelegate& ResponseDelegate);
    void GetAddress(const FImtblPassportResponseDelegate& ResponseDelegate);
    void GetEmail(const FImtblPassportResponseDelegate& ResponseDelegate);

protected:
    void Setup(TWeakObjectPtr<class UImtblJSConnector> Connector);
    
private:
    bool bIsInitialized = false;
    bool bIsLoggedIn = false;
    
    TWeakObjectPtr<UImtblJSConnector> JSConnector;
    FImmutablePassportInitData InitData;
    FDelegateHandle BridgeReadyHandle;
    TMap<FString, FImtblPassportResponseDelegate> ResponseDelegates;

    // Ensures that Passport has been initialized before calling JS
    bool CheckIsInitialized(const FString& Action, const FImtblPassportResponseDelegate& ResponseDelegate) const;
    // Calls JS with the given Action and Data, and registers the given ResponseDelegate to be called when JS responds
    void CallJS(const FString& Action, const FString& Data, const FImtblPassportResponseDelegate& ClientResponseDelegate, const FImtblJSResponseDelegate& HandleJSResponse, const bool bCheckInitialized = true);
    // Pulls the ResponseDelegate from the ResponseDelegates map and returns it
    TOptional<FImtblPassportResponseDelegate> GetResponseDelegate(const FImtblJSResponse& Response);
    void ConfirmCode(const FString& DeviceCode, const float Interval, const FImtblPassportResponseDelegate& ResponseDelegate);

    void OnInitializeResponse(FImtblJSResponse Response);
    void OnCheckStoredCredentialsResponse(FImtblJSResponse Response);
    void OnConnectSilentResponse(FImtblJSResponse Response);
    void OnConnectWithCredentialsResponse(FImtblJSResponse Response);
    void OnLogoutResponse(FImtblJSResponse Response);
    void OnConnectResponse(FImtblJSResponse Response);
    void OnConnectEvmResponse(FImtblJSResponse Response);
    void OnZkEvmRequestAccountsResponse(FImtblJSResponse Response);
    void OnZkEvmGetBalanceResponse(FImtblJSResponse Response);
    void OnZkEvmSendTransactionResponse(FImtblJSResponse Response);
    void OnConfirmCodeResponse(FImtblJSResponse Response);
    void OnGetAddressResponse(FImtblJSResponse Response);
    void OnGetEmailResponse(FImtblJSResponse Response);
    
    void LogAndIgnoreResponse(FImtblJSResponse Response);
};
