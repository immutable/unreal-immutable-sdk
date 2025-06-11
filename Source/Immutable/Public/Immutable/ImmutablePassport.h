// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "JsonObjectConverter.h"
#include "Runtime/Core/Public/HAL/Platform.h"
#include "UObject/Object.h"
#include "Immutable/ImtblJSConnector.h"
#include "Immutable/ImmutableDataTypes.h"
#include "Immutable/ImmutableRequests.h"

#include "ImmutablePassport.generated.h"

/**
 * Converts a UStruct instance to a JSON string representation.
 *
 * @param InStruct 	The UStruct instance to convert.
 *
 * @return 			A JSON string representation of the input struct.
 */
template <typename UStructType>
FString UStructToJsonString(const UStructType& InStruct)
{
	FString OutString;
	FJsonObjectConverter::UStructToJsonObjectString(InStruct, OutString, 0, 0, 0, nullptr, false);
	return OutString;
}

/**
 * Converts a JSON object to a UStruct instance.
 *
 * @param JsonObject 	The JSON object to convert into a UStruct.
 *
 * @return 				An optional UStruct instance, or an empty optional if conversion fails.
 */
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
	/**
	 * Delegate used for when passport is ready.
	 */
	DECLARE_MULTICAST_DELEGATE(FOnPassportReadyDelegate);

	/**
	 * Delegate used for JavaScript callbacks.
	 */
	DECLARE_DELEGATE_OneParam(FImtblPassportResponseDelegate, FImmutablePassportResult);

#if PLATFORM_ANDROID
	/**
	 * Delegate used for handling the dismissal of the PKCE flow on Android.
	 */
	DECLARE_DELEGATE(FImtblPassportOnPKCEDismissedDelegate);
#endif

	/**
	 * Initialises passport. This sets up the Passport instance, configures the web browser, and waits for the ready signal.
	 *
	 * @param InitData 			Parameters to initialise the passport with.
	 * @param ResponseDelegate 	Callback delegate.
	 */
	void Initialize(const FImmutablePassportInitData& InitData, const FImtblPassportResponseDelegate& ResponseDelegate);

	/**
	 * Initialises passport. This sets up the Passport instance, configures the web browser, and waits for the ready signal.
	 * @details The functionality is the same with Initilize above except it obtains Passport initilization data from ApplicationConfig asset
	 * @see UApplicationConfig
	 *
	 * @param ResponseDelegate 	Callback delegate.
	 */
	void Initialize(const FImtblPassportResponseDelegate& ResponseDelegate);

#if PLATFORM_ANDROID | PLATFORM_IOS | PLATFORM_MAC | PLATFORM_WINDOWS
	/**
	 * Logs into Passport using Authorisation Code Flow with Proof Key for Code Exchange (PKCE)
	 *
	 * @param IsConnectImx 		If true, player will connect to Immutable X after logging in.
	 * 							Else, just perform the login without connecting to Immutable X.
	 * @param ResponseDelegate 	Callback delegate.
	 */
	void Connect(bool IsConnectImx, const FImtblPassportResponseDelegate& ResponseDelegate);
#endif

	/**
	 * Logs the user out of Passport.
	 *
	 * @param DoHardLogout 		If true, clears sessions and any stored credentials from both the SDK/plugin and the browser.
	 * 							Else, clears session and any stored credentials from the SDK only, browser session and stored credentials are cleared when session expires.
	 * @param ResponseDelegate 	Callback delegate.
	 */
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
	* Similar to ZkEvmSendTransaction. Creates new message call transaction or a contract creation, if the data field contains code, 
	* and signs it using the account specified in from. Additionally, it also resolves to the TransactionReceiptResponse once the transaction has 
	* been included in the chain for confirms blocks. If confirms is 0, and the transaction has not been mined, null is returned.
	* The callback responce data may contain the receipt of the transaction or null if it is still processing.
	* @param Request The request data to perform the transaction.
	* @param ResponseDelegate The response delegate of type
	* FImtblPassportResponseDelegate to call on response from JS.
	*/
	void ZkEvmSendTransactionWithConfirmation(const FImtblTransactionRequest& Request, const FImtblPassportResponseDelegate& ResponseDelegate);

	/**
	 * Retrieves the transaction information of a given transaction hash. This function uses the Ethereum JSON-RPC 
	 * <c>eth_getTransactionReceipt</c> method. Response will contain the receipt of the transaction or null if it is still processing.
	 * @param Request The request data(Hash) to perform the transaction.
	 * @param ResponseDelegate The response delegate of type
	 * FImtblPassportResponseDelegate to call on response from JS.
	 */
	void ZkEvmGetTransactionReceipt(const FZkEvmTransactionReceiptRequest& Request, const FImtblPassportResponseDelegate& ResponseDelegate);

	/**
	 * Generate a signature for a typed data V4 object
	 * Signs the EIP-712 structured message in JSON string format using the logged-in Passport account.
	 * @see https://eips.ethereum.org/EIPS/eip-712
	 * @param RequestJsonString The EIP-712 structured data in JSON string format
	 * @param ResponseDelegate The response delegate of type
	 * FImtblPassportResponseDelegate to call on response from JS.
	 */
	void ZkEvmSignTypedDataV4(const FString& RequestJsonString, const FImtblPassportResponseDelegate& ResponseDelegate);

	/**
	 * Gets the currently saved ID token without verifying its validity. 
	 *
	 * @param ResponseDelegate Callback delegate.
	 */
	void GetIdToken(const FImtblPassportResponseDelegate& ResponseDelegate);

	/**
	 * Gets the currently saved access token without verifying its validity. 
	 *
	 * @param ResponseDelegate Callback delegate.
	 */
	void GetAccessToken(const FImtblPassportResponseDelegate& ResponseDelegate);

	/**
	 * Gets the wallet address of the logged in user. 
	 *
	 * @param ResponseDelegate Callback delegate.
	 */
	void GetAddress(const FImtblPassportResponseDelegate& ResponseDelegate);

	/**
	 * Retrieves the email address of the user whose credentials are currently stored. 
	 *
	 * @param ResponseDelegate Callback delegate.
	 */
	void GetEmail(const FImtblPassportResponseDelegate& ResponseDelegate);

	/**
	 * Gets the list of external wallets the user has linked to their Passport account via the  
	 *
	 * @param ResponseDelegate Callback delegate.
	 */
	void GetLinkedAddresses(const FImtblPassportResponseDelegate& ResponseDelegate);

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

	/**
	 * Retrieves the "result" as string field from Response.JsonObject.
	 *
	 * @param Response 	The response to use to retrieve the "result" as string field.
	 *
	 * @return 			The "result" as string field from Response.JsonObject if Response.JsonObject is valid, otherwise, an empty string.
	 */
	static FString GetResponseResultAsString(const FImtblJSResponse& Response);

	/**
	 * Retrieves the "result" as bool field from Response.JsonObject.
	 *
	 * @param Response 	The response to use to retrieve the "result" as bool field.
	 *
	 * @return 			True if Response.JsonObject is valid, otherwise, false.
	 */
	static bool GetResponseResultAsBool(const FImtblJSResponse& Response);

	/**
	 * Retrieves the "result" as array field from Response.JsonObject and extracting them into an array of FString.
	 *
	 * @param Response 	The response to use to retrieve and extract.
	 *
	 * @return 			An array of FString extracted from the "result" field if Response.JsonObject is valid, otherwise, an empty array.
	 */
	static TArray<FString> GetResponseResultAsStringArray(const FImtblJSResponse& Response);

#if PLATFORM_ANDROID | PLATFORM_WINDOWS
	/**
	 * Handle deep linking. This is called from platform-specific code.
	 *
	 * @param DeepLink The deep link URL to process.
	 */
	void HandleDeepLink(FString DeepLink) const;
#endif
#if PLATFORM_IOS | PLATFORM_MAC
	/**
	 * Handle deep linking. This is called from iOS/Mac native code.
	 *
	 * @param DeepLink	The deep link URL, passed from the iOS/Mac. This string contains the deep link data to be processed.
	 */
	void HandleDeepLink(NSString* sDeepLink) const;
#endif

#if PLATFORM_ANDROID
	/**
	 * Handle the dismissal of custom tabs.
	 *
	 * @param Url The URL associated with the custom tab that was dismissed. 
	 */
	void HandleCustomTabsDismissed(FString Url);
#endif

protected:
#if PLATFORM_ANDROID
	/*
	 * Delegate used for handling the dismissal of the PKCE flow on Android.
	 */
	DECLARE_DELEGATE(FImtblPassportOnPKCEDismissedDelegate);
#endif

#if PLATFORM_ANDROID | PLATFORM_IOS | PLATFORM_MAC | PLATFORM_WINDOWS
	/** Delegate for handling deep link activation. */
	FImmutableDeepLinkMulticastDelegate OnHandleDeepLink;
	// Since the second part of PKCE is triggered by deep links, saving the
	// response delegate here so it's easier to get
	FImtblPassportResponseDelegate PKCEResponseDelegate;
	/** Delegate for handling PCKE logout. */
	FImtblPassportResponseDelegate PKCELogoutResponseDelegate;
#endif

	/**
	 * Calls JavaScript function to the connected browser with specified parameters.
	 *
	 * @param Action 					The name of the JavaScript action to be called.
	 * @param Data 						The data to be passed to the JavaScript action as FString.
	 * @param ClientResponseDelegate 	Delegate to handle the response from the client.
	 * @param HandleJSResponse 			Delegate to handle the response from the JavaScript function.
	 * @param bCheckInitialized 		(Optional) If true, check if the passport is initialised. Else, initialised checks are skipped.
	 */
	void CallJS(const FString& Action, const FString& Data, const FImtblPassportResponseDelegate& ClientResponseDelegate, const FImtblJSResponseDelegate& HandleJSResponse, const bool bCheckInitialized = true);

	/**
	 * Sets up passport with the JavaScript connector
	 *
	 * @param Connector A weak pointer to the JavaScript Connector. If the connector is invalid set up will be aborted.
	 */
	void Setup(TWeakObjectPtr<class UImtblJSConnector> Connector);



	/**
	 * Checks if Passport has been initialised before allowing an action to proceed.
	 *
	 * @param Action 			The name of the JavaScript action to be called. Used for logging purposes.
	 * @param ResponseDelegate 	Delegate to handle the response if the passport is not initialised.
	 *
	 * @return 					True if Passport is initialised, otherwise, false.
	 */
	bool CheckIsInitialized(const FString& Action, const FImtblPassportResponseDelegate& ResponseDelegate) const;

	/**
	 * Retrieves the response delegate associated with a given JavaScript response from ResponseDelegates TMap.
	 *
	 * @param Response 	The Javascript response object containing the request Id.
	 *
	 * @return 			A TOptional containing the response delegate if found, otherwise, an empty TOptional.
	 */
	TOptional<FImtblPassportResponseDelegate> GetResponseDelegate(const FImtblJSResponse& Response);



	/**
	 * Common callback that handles the responses from game bridge
	 *
	 * @param Response The JavaScript response object containing the result of the callback.
	 */
	void OnBridgeCallbackResponse(FImtblJSResponse Response);

	/**
	 * Callback from init (passport).
	 *
	 * @param Response The JavaScript response object containing the result of the callback.
	 */
	void OnInitializeResponse(FImtblJSResponse Response);



	/**
	 * Callback from logout.
	 *
	 * @param Response The JavaScript response object containing the result of the callback.
	 */
	void OnLogoutResponse(FImtblJSResponse Response);



	// mobile platform callbacks
#if PLATFORM_ANDROID | PLATFORM_IOS | PLATFORM_MAC | PLATFORM_WINDOWS
	/**
	 * Callback from Get PKCE Auth URL.
	 *
	 * @param Response The JavaScript response object containing the result of the callback.
	 */
	void OnGetAuthUrlResponse(FImtblJSResponse Response);

	/*
	 * Callback from Connect PKCE.
	 *
	 * @param Response The JavaScript response object containing the result of the callback.
	 */
	void OnConnectResponse(FImtblJSResponse Response);

	/*
	 * Completes the PKCE login flow using the provided URL.
	 * 
	 * @param Url The URL containing the authorisation code and state.
	 */
	void CompleteLoginFlow(FString Url);
#endif

	/*
	 * Callback when deep link is activated.
	 *
	 * @param DeepLink The deep link URL that was activated. 
	 */
	UFUNCTION()
	void OnDeepLinkActivated(const FString& DeepLink);

#if PLATFORM_ANDROID
	/**
	 * Callback when Login PKCE is dismissed.
	 */
	void HandleOnLoginPKCEDismissed();

	/**
	 * Calls a static void method in Java using JNI.
	 *
	 * @param Env		The JNI (Java Native Interface) environment.
	 * @param Class		The Java class containing the method.
	 * @param Method	The method ID of the method to call.
	 * @param ...		Additional parameters to forward to the method to call.
	 */
	void CallJniStaticVoidMethod(JNIEnv* Env, const jclass Class, jmethodID Method, ...);

	/**
	 * Opens a specified URL on Android using Chrome Custom Tabs via JNI if Chrome is installed.
	 * Else, it launches the URL in any available browser.
	 *
	 * @param Url The URL to launch.
	 */
	void LaunchAndroidUrl(FString Url);
#endif

	/**
	 * Sets the specified state flags by applying a bitwise OR operation.
	 * @param StateIn The state flags to be set.
	 */
	void SetStateFlags(uint8 StateIn);

	/**
	 * Resets the specified state flags by applying a bitwise AND operation with the negated flags.
	 * @param StateIn The state flags to be reset.
	 */
	void ResetStateFlags(uint8 StateIn);

	/**
	 * Checks if the specified state flags are set. 
	 * @param StateIn The state flags to check.
	 * @return True if all StateIn flags are set, otherwise, false.
	 */
	bool IsStateFlagsSet(uint8 StateIn) const;

protected:
	/** Cached pointer to the JavaScript connector used for communicating JavaScript calls. */
	TWeakObjectPtr<UImtblJSConnector> JSConnector;
	/** Cached passport init data. */
	FImmutablePassportInitData InitData;
	FDelegateHandle BridgeReadyHandle;
	/** A map of JavaScript calls request Ids to their response callbacks. */
	TMap<FString, FImtblPassportResponseDelegate> ResponseDelegates;

#if PLATFORM_ANDROID
	/** Delegate called when the PKCE flow is dismissed. */
	FImtblPassportOnPKCEDismissedDelegate OnPKCEDismissed;
#endif

private:
	/**
	 * Saves the current Passport settings to save game object.
	 */
	void SavePassportSettings();

	/**
	 * Loads the passport settings from save game object.
	 */
	void LoadPassportSettings();

private:
	/**
	 * State flgas for Immutable Passport.
	 */
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

	/** Passport state flags. */
	uint8 StateFlags = IPS_NONE;

	/** Pointer to the analytics manager instance for tracking events and metrics. */
	UPROPERTY()
	class UImmutableAnalytics* Analytics = nullptr;

	/**
	 * PKCE data used for PKCE operations e.g. login and logout flows.
	 * When null, no PKCE operation is currently in progress.
	 * When non-null, an active PKCE operation is being processed.
	 */
	UPROPERTY(Transient)
	TObjectPtr<UImmutablePKCEData> PKCEData;
};