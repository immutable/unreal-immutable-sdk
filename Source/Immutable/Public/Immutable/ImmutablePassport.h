// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "JsonObjectConverter.h"
#include "Misc/EngineVersion.h"
#include "Runtime/Core/Public/HAL/Platform.h"
#include "UObject/Object.h"
#include "Immutable/ImtblJSConnector.h"
#include "Immutable/ImmutableDataTypes.h"
#include "Immutable/ImmutableRequests.h"

#include "ImmutablePassport.generated.h"


template <typename UStructType>
FString UStructToJsonString(const UStructType& InStruct)
{
	FString OutString;
	FJsonObjectConverter::UStructToJsonObjectString(InStruct, OutString, 0, 0, 0, nullptr, false);
	return OutString;
}

template <typename UStructType>
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

/**
 * Immutable sdk Passport object
 */
UCLASS()
class IMMUTABLE_API UImmutablePassport : public UObject
{
	GENERATED_BODY()
	friend class UImmutableSubsystem;

public:
	DECLARE_MULTICAST_DELEGATE(FOnPassportReadyDelegate);

	DECLARE_DELEGATE_OneParam(FImtblPassportResponseDelegate, FImmutablePassportResult);

#if PLATFORM_ANDROID
	void HandleDeepLink(FString DeepLink) const;
	void HandleCustomTabsDismissed(FString Url);
#elif PLATFORM_IOS | PLATFORM_MAC
	void HandleDeepLink(NSString* sDeepLink) const;
#endif

	void Initialize(const FImmutablePassportInitData& InitData, const FImtblPassportResponseDelegate& ResponseDelegate);

	void Connect(bool IsConnectImx, bool TryToRelogin, const FImtblPassportResponseDelegate& ResponseDelegate);
#if PLATFORM_ANDROID | PLATFORM_IOS | PLATFORM_MAC
	void ConnectPKCE(bool IsConnectImx, const FImtblPassportResponseDelegate& ResponseDelegate);
#endif

	void Logout(bool DoHardLogout, const FImtblPassportResponseDelegate& ResponseDelegate);

	/**
	 * Initializes the zkEVM provider.
	 * @param ResponseDelegate The response delegate of type
	 * FImtblPassportResponseDelegate to call on response from JS.
	 */
	void ConnectEvm(const FImtblPassportResponseDelegate& ResponseDelegate);

	/**
	 * This method attempts to authenticate the user and initialises their
	 * Passport wallet before returning an array of wallet addresses.
	 *
	 * This must be called before using other zkEVM functions.
	 * @param ResponseDelegate The response delegate of type
	 * FImtblPassportResponseDelegate to call on response from JS.
	 */
	void ZkEvmRequestAccounts(const FImtblPassportResponseDelegate& ResponseDelegate);

	/**
	 * Returns the balance of the account of given address in wei.
	 * @param Data The address and block number for the request
	 * @param ResponseDelegate The response delegate of type
	 * FImtblPassportResponseDelegate to call on response from JS.
	 */
	void ZkEvmGetBalance(const FImmutablePassportZkEvmGetBalanceData& Data, const FImtblPassportResponseDelegate& ResponseDelegate);

	/**
	 * Creates new message call transaction or a contract creation, if the data
	 * field contains code.
	 * @param Request The request data to perform the transaction.
	 * @param ResponseDelegate The response delegate of type
	 * FImtblPassportResponseDelegate to call on response from JS.
	 */
	void ZkEvmSendTransaction(const FImtblTransactionRequest& Request, const FImtblPassportResponseDelegate& ResponseDelegate);

	/**
	 * Retrieves the transaction information of a given transaction hash. This function uses the Ethereum JSON-RPC 
	 * <c>eth_getTransactionReceipt</c> method. Response will contain the receipt of the transaction or null if it is still processing.
	 * @param Request The request data(Hash) to perform the transaction.
	 * @param ResponseDelegate The response delegate of type
	 * FImtblPassportResponseDelegate to call on response from JS.
	 */
	void ZkEvmGetTransactionReceipt(const FZkEvmTransactionReceiptRequest& Request, const FImtblPassportResponseDelegate& ResponseDelegate);
	
	void GetIdToken(const FImtblPassportResponseDelegate& ResponseDelegate);
	void GetAccessToken(const FImtblPassportResponseDelegate& ResponseDelegate);
	void GetAddress(const FImtblPassportResponseDelegate& ResponseDelegate);
	void GetEmail(const FImtblPassportResponseDelegate& ResponseDelegate);

	/**
	 * Create a new imx transfer request.
	 * @param RequestData The transfer details structure of type
	 * FImxTransferRequest
	 * @param ResponseDelegate The response delegate of type
	 * FImtblPassportResponseDelegate to call on response from JS.
	 */
	void ImxTransfer(const FImxTransferRequest& RequestData, const FImtblPassportResponseDelegate& ResponseDelegate);

	/**
	 * Creates a new imx batch nft transfer request with the given transfer
	 * details.
	 * @param RequestData The transfer details structure of type
	 * FImxBatchNftTransferRequest
	 * @param ResponseDelegate The response delegate of type
	 * FImtblPassportResponseDelegate to call on response from JS.
	 */
	void ImxBatchNftTransfer(const FImxBatchNftTransferRequest& RequestData, const FImtblPassportResponseDelegate& ResponseDelegate);

	/**
	 *
	 * Checks if the user is registered off-chain.
	 * @param ResponseDelegate The response delegate of type
	 * FImtblPassportResponseDelegate to call on response from JS.
	 */
	void ImxIsRegisteredOffchain(const FImtblPassportResponseDelegate& ResponseDelegate);

	/**
	 * Register the user to Immutable X if they are not already registered
	 * @param ResponseDelegate The response delegate of type
	 * FImtblPassportResponseDelegate to call on response from JS.
	 */
	void ImxRegisterOffchain(const FImtblPassportResponseDelegate& ResponseDelegate);

	/**
	 * Checks if the user's credentials have been stored
	 * @param ResponseDelegate The response delegate of type
	 * FImtblPassportResponseDelegate to call on response from JS.
	 */
	void HasStoredCredentials(const FImtblPassportResponseDelegate& ResponseDelegate);

protected:
	void Setup(TWeakObjectPtr<class UImtblJSConnector> Connector);
	void ReinstateConnection(FImtblJSResponse Response);

#if PLATFORM_ANDROID
	DECLARE_DELEGATE(FImtblPassportOnPKCEDismissedDelegate);
  
	FImtblPassportOnPKCEDismissedDelegate OnPKCEDismissed;
#endif

	TWeakObjectPtr<UImtblJSConnector> JSConnector;
	FImmutablePassportInitData InitData;
	FDelegateHandle BridgeReadyHandle;
	TMap<FString, FImtblPassportResponseDelegate> ResponseDelegates;
#if PLATFORM_ANDROID | PLATFORM_IOS | PLATFORM_MAC
	DECLARE_DELEGATE_OneParam(FImtblPassportHandleDeepLinkDelegate, FString);

	FImtblPassportHandleDeepLinkDelegate OnHandleDeepLink;
	// Since the second part of PKCE is triggered by deep links, saving the
	// response delegate here so it's easier to get
	FImtblPassportResponseDelegate PKCEResponseDelegate;
	FImtblPassportResponseDelegate PKCELogoutResponseDelegate;
	// bool IsPKCEConnected = false;
#endif

	// Ensures that Passport has been initialized before calling JS
	bool CheckIsInitialized(const FString& Action, const FImtblPassportResponseDelegate& ResponseDelegate) const;
	// Calls JS with the given Action and Data, and registers the given
	// ResponseDelegate to be called when JS responds
	void CallJS(const FString& Action, const FString& Data, const FImtblPassportResponseDelegate& ClientResponseDelegate, const FImtblJSResponseDelegate& HandleJSResponse, const bool bCheckInitialized = true);
	// Pulls the ResponseDelegate from the ResponseDelegates map and returns it
	TOptional<FImtblPassportResponseDelegate> GetResponseDelegate(const FImtblJSResponse& Response);
	void ConfirmCode(const FString& DeviceCode, const float Interval, const FImtblPassportResponseDelegate& ResponseDelegate);

	void OnInitializeResponse(FImtblJSResponse Response);

	void OnInitDeviceFlowResponse(FImtblJSResponse Response);

	void OnLogoutResponse(FImtblJSResponse Response);
	void OnConnectResponse(FImtblJSResponse Response);
	void OnConnectSilentResponse(FImtblJSResponse Response);
	void OnConnectEvmResponse(FImtblJSResponse Response);
	void OnZkEvmRequestAccountsResponse(FImtblJSResponse Response);
	void OnZkEvmGetBalanceResponse(FImtblJSResponse Response);
	void OnZkEvmSendTransactionResponse(FImtblJSResponse Response);
	void OnZkEvmGetTransactionReceiptResponse(FImtblJSResponse Response);
	void OnConfirmCodeResponse(FImtblJSResponse Response);
#if PLATFORM_ANDROID | PLATFORM_IOS | PLATFORM_MAC
	void OnGetPKCEAuthUrlResponse(FImtblJSResponse Response);
	void OnConnectPKCEResponse(FImtblJSResponse Response);
#endif
	void OnGetIdTokenResponse(FImtblJSResponse Response);
	void OnGetAccessTokenResponse(FImtblJSResponse Response);
	void OnGetAddressResponse(FImtblJSResponse Response);
	void OnGetEmailResponse(FImtblJSResponse Response);
	void OnTransferResponse(FImtblJSResponse Response);
	void OnBatchNftTransferResponse(FImtblJSResponse Response);
	void OnImxIsRegisteredOffchain(FImtblJSResponse Response);
	void OnImxRegisterOffchain(FImtblJSResponse Response);

	void LogAndIgnoreResponse(FImtblJSResponse Response);

#if PLATFORM_ANDROID | PLATFORM_IOS | PLATFORM_MAC
	void OnDeepLinkActivated(FString DeepLink);
	void CompleteLoginPKCEFlow(FString Url);
#endif

#if PLATFORM_ANDROID
	void HandleOnLoginPKCEDismissed();
	void CallJniStaticVoidMethod(JNIEnv* Env, const jclass Class, jmethodID Method, ...);
	void LaunchAndroidUrl(FString Url);
#endif

	void SetStateFlags(uint8 StateIn);
	void ResetStateFlags(uint8 StateIn);
	bool IsStateFlagsSet(uint8 StateIn) const;

private:
	void SavePassportSettings();
	void LoadPassportSettings();

private:
	enum EImmutablePassportStateFlags : uint8
	{
		IPS_NONE = 0,
		IPS_CONNECTING = 1 << 0,
		IPS_CONNECTED = 1 << 1,
		IPS_IMX = 1 << 2, // if set player used "connect" instead of "login"
		IPS_PKCE = 1 << 3,
		IPS_COMPLETING_PKCE = 1 << 4,
		IPS_INITIALIZED = 1 << 5,
		IPS_HARDLOGOUT = 1 << 6
	};

	uint8 StateFlags = IPS_NONE;

	UPROPERTY()
	class UImmutableAnalytics* Analytics = nullptr;

};
