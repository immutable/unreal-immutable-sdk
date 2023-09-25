// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Immutable/ImtblJSConnector.h"
#include "UObject/Object.h"
#include "JsonObjectConverter.h"
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
    const FString ConfirmCode = TEXT("confirmCode");
#if PLATFORM_ANDROID | PLATFORM_IOS
    const FString GetPKCEAuthUrl = TEXT("getPKCEAuthUrl");
    const FString ConnectPKCE = TEXT("connectPKCE");
#endif
    const FString GetAddress = TEXT("getAddress");
    const FString GetEmail = TEXT("getEmail");
    const FString Transfer = TEXT("imxTransfer");
    const FString BatchNftTransfer = TEXT("imxBatchNftTransfer");
    const FString EnvSandbox = TEXT("sandbox");
    const FString EnvProduction = TEXT("production");
}

template<typename UStructType>
FString UStructToJsonString(const UStructType& InStruct)
{
    FString OutString;
    FJsonObjectConverter::UStructToJsonObjectString(InStruct, OutString, 0, 0, 0, nullptr, false);
    return OutString;
}

template<typename UStructType>
TOptional<UStructType> JsonObjectToUStruct(const TSharedPtr<FJsonObject>& JsonObject)
{
    if (!JsonObject.IsValid())
    {
        return TOptional<UStructType>();
    }

    // Parse the JSON
    UStructType StructInstance;
    if (!FJsonObjectConverter::JsonObjectToUStruct(JsonObject.ToSharedRef(), &StructInstance, 0, 0))
    {
        IMTBL_ERR("Could not parse response from JavaScript into the expected Ustruct")
        return TOptional<UStructType>();
    }
    return StructInstance;
}

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

    static TOptional<FImmutablePassportConnectData> FromJsonString(const FString& JsonObjectString);
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
struct FImxTransferRequest
{
    GENERATED_BODY()

    UPROPERTY()
    FString receiver;

    UPROPERTY()
    FString type;

    UPROPERTY()
    FString amount;

    UPROPERTY()
    FString tokenId;

    UPROPERTY()
    FString tokenAddress;

    FString ToJsonString() const;
};

USTRUCT()
struct FImxTransferResponse
{
    GENERATED_BODY()

    UPROPERTY()
    FString sentSignature;

    UPROPERTY()
    FString status;

    UPROPERTY()
    float time;

    UPROPERTY()
    unsigned transferId;
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

USTRUCT()
struct FImxBatchNftTransferRequest
{
    GENERATED_BODY()

    UPROPERTY()
    TArray<FNftTransferDetails> nftTransferDetails;

    FString ToJsonString() const;
};

USTRUCT()
struct FImxBatchNftTransferResponse
{
    GENERATED_BODY()

    UPROPERTY()
    TArray<int> transferIds;
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


#if PLATFORM_ANDROID | PLATFORM_IOS
DECLARE_DELEGATE_OneParam(FImtblPassportHandleDeepLinkDelegate, FString);
FImtblPassportHandleDeepLinkDelegate OnHandleDeepLink;
#endif

/**
 *
 */
UCLASS()
class IMMUTABLE_API UImmutablePassport : public UObject
{
    GENERATED_BODY()
    friend class UImmutableSubsystem;

public:
    DECLARE_MULTICAST_DELEGATE(FOnPassportReadyDelegate);
    
    DECLARE_DELEGATE_OneParam(FImtblPassportResponseDelegate, FImmutablePassportResult);

#if PLATFORM_IOS
    static void HandleDeepLink(NSString* sDeepLink);
#endif

    void Initialize(const FImmutablePassportInitData& InitData, const FImtblPassportResponseDelegate& ResponseDelegate);
    void CheckStoredCredentials(const FImtblPassportResponseDelegate& ResponseDelegate);
    void ConnectSilent(const FImtblPassportResponseDelegate& ResponseDelegate);
    void Logout(const FImtblPassportResponseDelegate& ResponseDelegate);
    void Connect(const FImtblPassportResponseDelegate& ResponseDelegate);

#if PLATFORM_ANDROID | PLATFORM_IOS
    void ConnectPKCE(const FImtblPassportResponseDelegate& ResponseDelegate);
#endif

    void ConnectEvm(const FImtblPassportResponseDelegate& ResponseDelegate);
    void ZkEvmRequestAccounts(const FImtblPassportResponseDelegate& ResponseDelegate);
    void ZkEvmGetBalance(const FImmutablePassportZkEvmGetBalanceData& Data, const FImtblPassportResponseDelegate& ResponseDelegate);

    void GetAddress(const FImtblPassportResponseDelegate& ResponseDelegate);
    void GetEmail(const FImtblPassportResponseDelegate& ResponseDelegate);

    /**
    * Create a new transfer request.
    * @param RequestData The transfer details structure of type FImxTransferRequest
    * @param ResponseDelegate The response delegate of type FImtblPassportResponseDelegate to call on response from JS.
    */
    void Transfer(const FImxTransferRequest& RequestData, const FImtblPassportResponseDelegate& ResponseDelegate);

    /**
    * Creates a new batch nft transfer request with the given transfer details.
    * @param RequestData The transfer details structure of type FImxBatchNftTransferRequest
    * @param ResponseDelegate The response delegate of type FImtblPassportResponseDelegate to call on response from JS.
    */
    void BatchNftTransfer(const FImxBatchNftTransferRequest& RequestData, const FImtblPassportResponseDelegate& ResponseDelegate);

protected:
    void Setup(TWeakObjectPtr<class UImtblJSConnector> Connector);

private:
    bool bIsInitialized = false;
    bool bIsLoggedIn = false;

    TWeakObjectPtr<UImtblJSConnector> JSConnector;
    FImmutablePassportInitData InitData;
    FDelegateHandle BridgeReadyHandle;
    TMap<FString, FImtblPassportResponseDelegate> ResponseDelegates;
#if PLATFORM_ANDROID | PLATFORM_IOS
    // Since the second part of PKCE is triggered by deep links, saving the response delegate here so it's easier to get
    FImtblPassportResponseDelegate PKCEResponseDelegate;
#endif

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
    void OnConfirmCodeResponse(FImtblJSResponse Response);
#if PLATFORM_ANDROID | PLATFORM_IOS
    void OnGetPKCEAuthUrlResponse(FImtblJSResponse Response);
    void OnConnectPKCEResponse(FImtblJSResponse Response);
#endif
    void OnGetAddressResponse(FImtblJSResponse Response);
    void OnGetEmailResponse(FImtblJSResponse Response);
    void OnTransferResponse(FImtblJSResponse Response);
    void OnBatchNftTransferResponse(FImtblJSResponse Response);

    void LogAndIgnoreResponse(FImtblJSResponse Response);

#if PLATFORM_ANDROID | PLATFORM_IOS
    void OnDeepLinkActivated(FString DeepLink);
    void CompletePKCEFlow(FString Url);
#endif
};
