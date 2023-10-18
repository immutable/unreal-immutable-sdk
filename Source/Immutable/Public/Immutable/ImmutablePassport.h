// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Immutable/ImtblJSConnector.h"
#include "JsonObjectConverter.h"
#include "Misc/EngineVersion.h"
#include "Runtime/Core/Public/HAL/Platform.h"
#include "UObject/Object.h"
// clang-format off
#include "ImmutablePassport.generated.h"
// clang-format on

struct FImtblJSResponse;

namespace ImmutablePassportAction {
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
#if PLATFORM_ANDROID | PLATFORM_IOS | PLATFORM_MAC
const FString GetPKCEAuthUrl = TEXT("getPKCEAuthUrl");
const FString ConnectPKCE = TEXT("connectPKCE");
#endif
const FString GetAddress = TEXT("getAddress");
const FString GetEmail = TEXT("getEmail");
const FString ImxTransfer = TEXT("imxTransfer");
const FString ImxBatchNftTransfer = TEXT("imxBatchNftTransfer");
const FString EnvSandbox = TEXT("sandbox");
const FString EnvProduction = TEXT("production");
} // namespace ImmutablePassportAction

template <typename UStructType>
FString UStructToJsonString(const UStructType &InStruct) {
  FString OutString;
  FJsonObjectConverter::UStructToJsonObjectString(InStruct, OutString, 0, 0, 0,
                                                  nullptr, false);
  return OutString;
}

template <typename UStructType>
TOptional<UStructType>
JsonObjectToUStruct(const TSharedPtr<FJsonObject> &JsonObject) {
  if (!JsonObject.IsValid()) {
    return TOptional<UStructType>();
  }

  // Parse the JSON
  UStructType StructInstance;
  if (!FJsonObjectConverter::JsonObjectToUStruct(JsonObject.ToSharedRef(),
                                                 &StructInstance, 0, 0)) {
    IMTBL_ERR(
        "Could not parse response from JavaScript into the expected Ustruct")
    return TOptional<UStructType>();
  }
  return StructInstance;
}

USTRUCT()
struct FImmutablePassportResult {
  GENERATED_BODY()

  UPROPERTY()
  bool Success = false;
  UPROPERTY()
  FString Message;

  FImtblJSResponse Response;
};

USTRUCT()
struct FImmutableEngineVersionData {
  GENERATED_BODY()

  UPROPERTY()
  FString engine = TEXT("unreal");

  // cannot have spaces
  UPROPERTY()
  FString engineVersion =
      FEngineVersion::Current().ToString().Replace(TEXT(" "), TEXT("_"));

  // cannot have spaces
  UPROPERTY()
  FString platform = FString(FPlatformProperties::IniPlatformName())
                         .Replace(TEXT(" "), TEXT("_"));

  // cannot have spaces
  UPROPERTY()
  FString platformVersion =
      FPlatformMisc::GetOSVersion().Replace(TEXT(" "), TEXT("_"));
};

USTRUCT()
struct FImmutablePassportInitData {
  GENERATED_BODY()

  UPROPERTY()
  FString clientId;

  UPROPERTY()
  FString redirectUri;

  UPROPERTY()
  FString environment = ImmutablePassportAction::EnvSandbox;

  UPROPERTY()
  FImmutableEngineVersionData engineVersion;

  FString ToJsonString() const;
};

USTRUCT()
struct FImmutablePassportTokenData {
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
struct FImmutablePassportConnectData {
  GENERATED_BODY()

  UPROPERTY()
  FString code;
  UPROPERTY()
  FString deviceCode;
  UPROPERTY()
  FString url;
  UPROPERTY()
  float interval = 0;

  static TOptional<FImmutablePassportConnectData>
  FromJsonString(const FString &JsonObjectString);
};

USTRUCT()
struct FImmutablePassportZkEvmRequestAccountsData {
  GENERATED_BODY()

  UPROPERTY()
  TArray<FString> accounts;

  FString ToJsonString() const;
  static TOptional<FImmutablePassportZkEvmRequestAccountsData>
  FromJsonString(const FString &JsonObjectString);
  static TOptional<FImmutablePassportZkEvmRequestAccountsData>
  FromJsonObject(const TSharedPtr<FJsonObject> &JsonObject);
};

USTRUCT()
struct FImmutablePassportZkEvmGetBalanceData {
  GENERATED_BODY()

  UPROPERTY()
  FString address;

  UPROPERTY()
  FString blockNumberOrTag = "latest";

  FString ToJsonString() const;
};

USTRUCT(BlueprintType)
struct FImtblAccessListItem {
  GENERATED_BODY()

  UPROPERTY(BlueprintReadWrite, EditAnywhere)
  FString address;

  UPROPERTY(BlueprintReadWrite, EditAnywhere)
  TArray<FString> storageKeys;
};

/**
 * Key Value wrappers for converting to JSON
 */
USTRUCT()
struct FStringCustomData {
  GENERATED_BODY()

  UPROPERTY()
  FString key;

  UPROPERTY()
  FString value;
};

USTRUCT()
struct FInt64CustomData {
  GENERATED_BODY()

  UPROPERTY()
  FString key;

  UPROPERTY()
  int64 value;
};

USTRUCT()
struct FFloatCustomData {
  GENERATED_BODY()

  UPROPERTY()
  FString key;

  UPROPERTY()
  float value;
};

USTRUCT()
struct FBoolCustomData {
  GENERATED_BODY()

  UPROPERTY()
  FString key;

  UPROPERTY()
  bool value;
};

UENUM(BlueprintType)
enum EImtblCustomDataType { String, Int64, Float, Bool };

/**
 * Blueprint doesn't support any sort of generics or polymorphism. To workaround
 * this select the primitive type for this custom data item and set the
 * corresponding value. This will later be mapped to the proper API structure.
 */
USTRUCT(BlueprintType)
struct FImtblCustomData {
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
  TSharedPtr<FJsonObject> ToJsonObject() const {
    switch (type) {
    case String:
      return FJsonObjectConverter::UStructToJsonObject<FStringCustomData>(
          {key, stringValue});
    case Int64:
      return FJsonObjectConverter::UStructToJsonObject<FInt64CustomData>(
          {key, intValue});
    case Float:
      return FJsonObjectConverter::UStructToJsonObject<FFloatCustomData>(
          {key, floatValue});
    case Bool:
      return FJsonObjectConverter::UStructToJsonObject<FBoolCustomData>(
          {key, boolValue});
    default: {
    }
    }
    return {};
  }
};

USTRUCT(BlueprintType)
struct FImtblTransactionRequest {
  GENERATED_BODY()

  UPROPERTY(BlueprintReadWrite, EditAnywhere)
  FString to;

  UPROPERTY(BlueprintReadWrite, EditAnywhere)
  FString data = "0x";

  UPROPERTY(BlueprintReadWrite, EditAnywhere)
  FString value;
};

USTRUCT()
struct FImmutablePassportCodeConfirmRequestData {
  GENERATED_BODY()

  UPROPERTY()
  FString deviceCode;
  UPROPERTY()
  float interval = 5;
  UPROPERTY()
  float timeoutMs = 15 * 60 * 1000;
};

USTRUCT()
struct FImxTransferRequest {
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
struct FImxTransferResponse {
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
struct FNftTransferDetails {
  GENERATED_BODY()

  UPROPERTY(BlueprintReadWrite)
  FString receiver;

  UPROPERTY(BlueprintReadWrite)
  FString tokenId;

  UPROPERTY(BlueprintReadWrite)
  FString tokenAddress;
};

USTRUCT()
struct FImxBatchNftTransferRequest {
  GENERATED_BODY()

  UPROPERTY()
  TArray<FNftTransferDetails> nftTransferDetails;

  FString ToJsonString() const;
};

USTRUCT()
struct FImxBatchNftTransferResponse {
  GENERATED_BODY()

  UPROPERTY()
  TArray<int> transferIds;
};

USTRUCT()
struct FImmutablePassportConnectPKCEData {
  GENERATED_BODY()

  UPROPERTY()
  FString authorizationCode;

  UPROPERTY()
  FString state;
};

#if PLATFORM_ANDROID | PLATFORM_IOS | PLATFORM_MAC
DECLARE_DELEGATE_OneParam(FImtblPassportHandleDeepLinkDelegate, FString);
FImtblPassportHandleDeepLinkDelegate OnHandleDeepLink;
#endif

/**
 *
 */
UCLASS()
class IMMUTABLE_API UImmutablePassport : public UObject {
  GENERATED_BODY()
  friend class UImmutableSubsystem;

public:
  DECLARE_MULTICAST_DELEGATE(FOnPassportReadyDelegate);

  DECLARE_DELEGATE_OneParam(FImtblPassportResponseDelegate,
                            FImmutablePassportResult);

#if PLATFORM_ANDROID
  static void HandleDeepLink(FString DeepLink);
#elif PLATFORM_IOS | PLATFORM_MAC
  static void HandleDeepLink(NSString *sDeepLink);
#endif

  void Initialize(const FImmutablePassportInitData &InitData,
                  const FImtblPassportResponseDelegate &ResponseDelegate);
  void CheckStoredCredentials(
      const FImtblPassportResponseDelegate &ResponseDelegate);
  void ConnectSilent(const FImtblPassportResponseDelegate &ResponseDelegate);
  void Logout(const FImtblPassportResponseDelegate &ResponseDelegate);
  void Connect(const FImtblPassportResponseDelegate &ResponseDelegate);

#if PLATFORM_ANDROID | PLATFORM_IOS | PLATFORM_MAC
  void ConnectPKCE(const FImtblPassportResponseDelegate &ResponseDelegate);
#endif

  /**
   * Initializes the zkEVM provider.
   * @param ResponseDelegate The response delegate of type
   * FImtblPassportResponseDelegate to call on response from JS.
   */
  void ConnectEvm(const FImtblPassportResponseDelegate &ResponseDelegate);

  /**
   * This method attempts to authenticate the user and initialises their
   * Passport wallet before returning an array of wallet addresses.
   *
   * This must be called before using other zkEVM functions.
   * @param ResponseDelegate The response delegate of type
   * FImtblPassportResponseDelegate to call on response from JS.
   */
  void
  ZkEvmRequestAccounts(const FImtblPassportResponseDelegate &ResponseDelegate);

  /**
   * Returns the balance of the account of given address in wei.
   * @param Data The address and block number for the request
   * @param ResponseDelegate The response delegate of type
   * FImtblPassportResponseDelegate to call on response from JS.
   */
  void ZkEvmGetBalance(const FImmutablePassportZkEvmGetBalanceData &Data,
                       const FImtblPassportResponseDelegate &ResponseDelegate);

  /**
   * Creates new message call transaction or a contract creation, if the data
   * field contains code.
   * @param Request The request data to perform the transaction.
   * @param ResponseDelegate The response delegate of type
   * FImtblPassportResponseDelegate to call on response from JS.
   */
  void
  ZkEvmSendTransaction(const FImtblTransactionRequest &Request,
                       const FImtblPassportResponseDelegate &ResponseDelegate);
  void GetAddress(const FImtblPassportResponseDelegate &ResponseDelegate);
  void GetEmail(const FImtblPassportResponseDelegate &ResponseDelegate);

  /**
   * Create a new imx transfer request.
   * @param RequestData The transfer details structure of type
   * FImxTransferRequest
   * @param ResponseDelegate The response delegate of type
   * FImtblPassportResponseDelegate to call on response from JS.
   */
  void ImxTransfer(const FImxTransferRequest &RequestData,
                   const FImtblPassportResponseDelegate &ResponseDelegate);

  /**
   * Creates a new imx batch nft transfer request with the given transfer
   * details.
   * @param RequestData The transfer details structure of type
   * FImxBatchNftTransferRequest
   * @param ResponseDelegate The response delegate of type
   * FImtblPassportResponseDelegate to call on response from JS.
   */
  void
  ImxBatchNftTransfer(const FImxBatchNftTransferRequest &RequestData,
                      const FImtblPassportResponseDelegate &ResponseDelegate);

protected:
  void Setup(TWeakObjectPtr<class UImtblJSConnector> Connector);

private:
  bool bIsInitialized = false;
  bool bIsLoggedIn = false;

  TWeakObjectPtr<UImtblJSConnector> JSConnector;
  FImmutablePassportInitData InitData;
  FDelegateHandle BridgeReadyHandle;
  TMap<FString, FImtblPassportResponseDelegate> ResponseDelegates;
#if PLATFORM_ANDROID | PLATFORM_IOS | PLATFORM_MAC
  // Since the second part of PKCE is triggered by deep links, saving the
  // response delegate here so it's easier to get
  FImtblPassportResponseDelegate PKCEResponseDelegate;
#endif

  // Ensures that Passport has been initialized before calling JS
  bool CheckIsInitialized(
      const FString &Action,
      const FImtblPassportResponseDelegate &ResponseDelegate) const;
  // Calls JS with the given Action and Data, and registers the given
  // ResponseDelegate to be called when JS responds
  void CallJS(const FString &Action, const FString &Data,
              const FImtblPassportResponseDelegate &ClientResponseDelegate,
              const FImtblJSResponseDelegate &HandleJSResponse,
              const bool bCheckInitialized = true);
  // Pulls the ResponseDelegate from the ResponseDelegates map and returns it
  TOptional<FImtblPassportResponseDelegate>
  GetResponseDelegate(const FImtblJSResponse &Response);
  void ConfirmCode(const FString &DeviceCode, const float Interval,
                   const FImtblPassportResponseDelegate &ResponseDelegate);

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
#if PLATFORM_ANDROID | PLATFORM_IOS | PLATFORM_MAC
  void OnGetPKCEAuthUrlResponse(FImtblJSResponse Response);
  void OnConnectPKCEResponse(FImtblJSResponse Response);
#endif
  void OnGetAddressResponse(FImtblJSResponse Response);
  void OnGetEmailResponse(FImtblJSResponse Response);
  void OnTransferResponse(FImtblJSResponse Response);
  void OnBatchNftTransferResponse(FImtblJSResponse Response);

  void LogAndIgnoreResponse(FImtblJSResponse Response);

#if PLATFORM_ANDROID | PLATFORM_IOS | PLATFORM_MAC
  void OnDeepLinkActivated(FString DeepLink);
  void CompletePKCEFlow(FString Url);
#endif

#if PLATFORM_ANDROID
  void CallJniStaticVoidMethod(JNIEnv *Env, const jclass Class,
                               jmethodID Method, ...);
#endif
};
