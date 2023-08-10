// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ImtblJSConnector.h"
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
    const FString ConfirmCode = TEXT("confirmCode");
    const FString GetAddress = TEXT("getAddress");
    const FString GetEmail = TEXT("getEmail");
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

    void Initialize(const FString& ClientID, const FImtblPassportResponseDelegate& ResponseDelegate);
    void CheckStoredCredentials(const FImtblPassportResponseDelegate& ResponseDelegate);
    void ConnectSilent(const FImtblPassportResponseDelegate& ResponseDelegate);
    void Logout(const FImtblPassportResponseDelegate& ResponseDelegate);
    void Connect(const FImtblPassportResponseDelegate& ResponseDelegate);
    void GetAddress(const FImtblPassportResponseDelegate& ResponseDelegate);
    void GetEmail(const FImtblPassportResponseDelegate& ResponseDelegate);

protected:
    void Setup(TWeakObjectPtr<class UImtblJSConnector> Connector);
    
private:
    bool bIsInitialized = false;
    bool bIsLoggedIn = false;
    
    TWeakObjectPtr<UImtblJSConnector> JSConnector;
    FString ClientId;
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
    void OnConfirmCodeResponse(FImtblJSResponse Response);
    void OnGetAddressResponse(FImtblJSResponse Response);
    void OnGetEmailResponse(FImtblJSResponse Response);
    
    void LogAndIgnoreResponse(FImtblJSResponse Response);
};
