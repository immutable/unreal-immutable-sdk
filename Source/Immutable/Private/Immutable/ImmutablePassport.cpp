// Fill out your copyright notice in the Description page of Project Settings.

#include "Immutable/ImmutablePassport.h"

#include "Immutable/Misc/ImtblLogging.h"
#include "ImtblJSConnector.h"
#include "JsonObjectConverter.h"

#if PLATFORM_ANDROID | PLATFORM_IOS | PLATFORM_MAC
#include "GenericPlatform/GenericPlatformHttp.h"
#endif

#if PLATFORM_ANDROID
#include "Android/AndroidApplication.h"
#include "Android/ImmutableAndroidJNI.h"
#elif PLATFORM_IOS
#include "IOS/ImmutableIOS.h"
#elif PLATFORM_MAC
#include "GenericPlatform/GenericPlatformProcess.h"
#include "Mac/ImmutableMac.h"
#endif

FString FImmutablePassportInitData::ToJsonString() const {
  FString OutString;

  FJsonObjectWrapper Wrapper;
  Wrapper.JsonObject = MakeShared<FJsonObject>();
  FJsonObjectConverter::UStructToJsonObject(
      FImmutablePassportInitData::StaticStruct(), this,
      Wrapper.JsonObject.ToSharedRef(), 0, 0);

  if (!Wrapper.JsonObject.IsValid()) {
    IMTBL_ERR("Could not convert FImmutablePassportInitData to JSON")
    return "";
  }
  // Remove redirectUri field if it's empty so that the bridge doesn't try to
  // use it
  if (Wrapper.JsonObject->HasField("redirectUri") &&
      Wrapper.JsonObject->GetStringField("redirectUri").IsEmpty())
    Wrapper.JsonObject->RemoveField("redirectUri");
  Wrapper.JsonObjectToString(OutString);

  return OutString;
}

FString FImxBatchNftTransferRequest::ToJsonString() const {
  FString OutString;
  FJsonObjectWrapper Wrapper;
  Wrapper.JsonObject = MakeShared<FJsonObject>();
  FJsonObjectConverter::UStructToJsonObject(
      FImxBatchNftTransferRequest::StaticStruct(), this,
      Wrapper.JsonObject.ToSharedRef(), 0, 0);

  if (!Wrapper.JsonObject.IsValid()) {
    IMTBL_ERR("Could not convert FImxBatchNftTransferRequest to JSON")
    return "";
  }

  if (Wrapper.JsonObject->HasField("nftTransferDetails")) {
    const auto Writer =
        TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(
            &OutString);
    FJsonSerializer::Serialize(
        Wrapper.JsonObject->GetArrayField("nftTransferDetails"), Writer);
    IMTBL_LOG("FImxBatchNftTransferRequest Serialised: %s", *OutString);
    Writer->Close();
  }
  return OutString;
}

TOptional<FImmutablePassportConnectData>
FImmutablePassportConnectData::FromJsonString(const FString &JsonObjectString) {
  FImmutablePassportConnectData PassportConnect;
  if (!FJsonObjectConverter::JsonObjectStringToUStruct(
          JsonObjectString, &PassportConnect, 0, 0)) {
    IMTBL_WARN("Could not parse response from JavaScript into the expected "
               "Passport connect format")
    return TOptional<FImmutablePassportConnectData>();
  }
  return PassportConnect;
}

FString FImmutablePassportZkEvmRequestAccountsData::ToJsonString() const {
  FString OutString;
  FJsonObjectConverter::UStructToJsonObjectString(*this, OutString, 0, 0, 0,
                                                  nullptr, false);
  return OutString;
}

TOptional<FImmutablePassportZkEvmRequestAccountsData>
FImmutablePassportZkEvmRequestAccountsData::FromJsonString(
    const FString &JsonObjectString) {
  FImmutablePassportZkEvmRequestAccountsData RequestAccounts;
  if (!FJsonObjectConverter::JsonObjectStringToUStruct(
          JsonObjectString, &RequestAccounts, 0, 0)) {
    IMTBL_WARN("Could not parse response from JavaScript into the expected "
               "Passport ZkEvm request accounts format")
    return TOptional<FImmutablePassportZkEvmRequestAccountsData>();
  }
  return RequestAccounts;
}

TOptional<FImmutablePassportZkEvmRequestAccountsData>
FImmutablePassportZkEvmRequestAccountsData::FromJsonObject(
    const TSharedPtr<FJsonObject> &JsonObject) {
  if (!JsonObject.IsValid())
    return TOptional<FImmutablePassportZkEvmRequestAccountsData>();

  // Parse the JSON
  FImmutablePassportZkEvmRequestAccountsData RequestAccounts;
  if (!FJsonObjectConverter::JsonObjectToUStruct(JsonObject.ToSharedRef(),
                                                 &RequestAccounts, 0, 0)) {
    IMTBL_ERR("Could not parse response from JavaScript into the expected "
              "Passport ZkEvm request accounts format")
    return TOptional<FImmutablePassportZkEvmRequestAccountsData>();
  }
  return RequestAccounts;
}

FString FImmutablePassportZkEvmGetBalanceData::ToJsonString() const {
  FString OutString;

  FJsonObjectWrapper Wrapper;
  Wrapper.JsonObject = MakeShared<FJsonObject>();
  FJsonObjectConverter::UStructToJsonObject(
      FImmutablePassportZkEvmGetBalanceData::StaticStruct(), this,
      Wrapper.JsonObject.ToSharedRef(), 0, 0);

  if (!Wrapper.JsonObject.IsValid()) {
    IMTBL_ERR("Could not convert FImmutablePassportZkEvmGetBalanceData to JSON")
    return "";
  }
  Wrapper.JsonObjectToString(OutString);

  return OutString;
}

// @param Environment can be one of ImmutablePassportAction::EnvSandbox or
// ImmutablePassportAction::EnvProduction
void UImmutablePassport::Initialize(
    const FImmutablePassportInitData &Data,
    const FImtblPassportResponseDelegate &ResponseDelegate) {
  check(JSConnector.IsValid());

  InitData = Data;

  CallJS(ImmutablePassportAction::Initialize, InitData.ToJsonString(),
         ResponseDelegate,
         FImtblJSResponseDelegate::CreateUObject(
             this, &UImmutablePassport::OnInitializeResponse),
         false);
}

void UImmutablePassport::CheckStoredCredentials(
    const FImtblPassportResponseDelegate &ResponseDelegate) {
  CallJS(ImmutablePassportAction::CheckStoredCredentials, TEXT(""),
         ResponseDelegate,
         FImtblJSResponseDelegate::CreateUObject(
             this, &UImmutablePassport::OnCheckStoredCredentialsResponse));
}

void UImmutablePassport::ConnectSilent(
    const FImtblPassportResponseDelegate &ResponseDelegate) {
  CallJS(ImmutablePassportAction::CheckStoredCredentials, TEXT(""),
         ResponseDelegate,
         FImtblJSResponseDelegate::CreateUObject(
             this, &UImmutablePassport::OnConnectSilentResponse));
}

void UImmutablePassport::Logout(
    const FImtblPassportResponseDelegate &ResponseDelegate) {
  CallJS(ImmutablePassportAction::Logout, TEXT(""), ResponseDelegate,
         FImtblJSResponseDelegate::CreateUObject(
             this, &UImmutablePassport::OnLogoutResponse));
}

void UImmutablePassport::Connect(
    const FImtblPassportResponseDelegate &ResponseDelegate) {
  CallJS(ImmutablePassportAction::Connect, TEXT(""), ResponseDelegate,
         FImtblJSResponseDelegate::CreateUObject(
             this, &UImmutablePassport::OnConnectResponse));
}

void UImmutablePassport::ConnectEvm(
    const FImtblPassportResponseDelegate &ResponseDelegate) {
  CallJS(ImmutablePassportAction::ConnectEvm, TEXT(""), ResponseDelegate,
         FImtblJSResponseDelegate::CreateUObject(
             this, &UImmutablePassport::OnConnectEvmResponse));
}

void UImmutablePassport::ZkEvmRequestAccounts(
    const FImtblPassportResponseDelegate &ResponseDelegate) {
  CallJS(ImmutablePassportAction::ZkEvmRequestAccounts, TEXT(""),
         ResponseDelegate,
         FImtblJSResponseDelegate::CreateUObject(
             this, &UImmutablePassport::OnZkEvmRequestAccountsResponse));
}

void UImmutablePassport::ZkEvmGetBalance(
    const FImmutablePassportZkEvmGetBalanceData &Data,
    const FImtblPassportResponseDelegate &ResponseDelegate) {
  CallJS(ImmutablePassportAction::ZkEvmGetBalance, Data.ToJsonString(),
         ResponseDelegate,
         FImtblJSResponseDelegate::CreateUObject(
             this, &UImmutablePassport::OnZkEvmGetBalanceResponse));
}

void UImmutablePassport::ZkEvmSendTransaction(
    const FImtblTransactionRequest &Request,
    const FImtblPassportResponseDelegate &ResponseDelegate) {
  CallJS(ImmutablePassportAction::ZkEvmSendTransaction,
         UStructToJsonString(Request), ResponseDelegate,
         FImtblJSResponseDelegate::CreateUObject(
             this, &UImmutablePassport::OnZkEvmSendTransactionResponse));
}

void UImmutablePassport::ConfirmCode(
    const FString &DeviceCode, const float Interval,
    const FImtblPassportResponseDelegate &ResponseDelegate) {
  FImmutablePassportCodeConfirmRequestData Data;
  Data.deviceCode = DeviceCode;
  Data.interval = Interval;
  CallJS(ImmutablePassportAction::ConfirmCode, UStructToJsonString(Data),
         ResponseDelegate,
         FImtblJSResponseDelegate::CreateUObject(
             this, &UImmutablePassport::OnConfirmCodeResponse));
}

#if PLATFORM_ANDROID | PLATFORM_IOS | PLATFORM_MAC
void UImmutablePassport::ConnectPKCE(
    const FImtblPassportResponseDelegate &ResponseDelegate) {
#if PLATFORM_ANDROID
  completingPKCE = false;
#endif

  PKCEResponseDelegate = ResponseDelegate;
  CallJS(ImmutablePassportAction::GetPKCEAuthUrl, TEXT(""),
         PKCEResponseDelegate,
         FImtblJSResponseDelegate::CreateUObject(
             this, &UImmutablePassport::OnGetPKCEAuthUrlResponse));
}
#endif

void UImmutablePassport::GetAddress(
    const FImtblPassportResponseDelegate &ResponseDelegate) {
  CallJS(ImmutablePassportAction::GetAddress, TEXT(""), ResponseDelegate,
         FImtblJSResponseDelegate::CreateUObject(
             this, &UImmutablePassport::OnGetAddressResponse));
}

void UImmutablePassport::GetEmail(
    const FImtblPassportResponseDelegate &ResponseDelegate) {
  CallJS(ImmutablePassportAction::GetEmail, TEXT(""), ResponseDelegate,
         FImtblJSResponseDelegate::CreateUObject(
             this, &UImmutablePassport::OnGetEmailResponse));
}

void UImmutablePassport::ImxTransfer(
    const FImxTransferRequest &RequestData,
    const FImtblPassportResponseDelegate &ResponseDelegate) {
  IMTBL_LOG("Tranfer Request: %s", *UStructToJsonString(RequestData))
  CallJS(ImmutablePassportAction::ImxTransfer, UStructToJsonString(RequestData),
         ResponseDelegate,
         FImtblJSResponseDelegate::CreateUObject(
             this, &UImmutablePassport::OnTransferResponse));
}

void UImmutablePassport::ImxBatchNftTransfer(
    const FImxBatchNftTransferRequest &RequestData,
    const FImtblPassportResponseDelegate &ResponseDelegate) {
  CallJS(ImmutablePassportAction::ImxBatchNftTransfer,
         RequestData.ToJsonString(), ResponseDelegate,
         FImtblJSResponseDelegate::CreateUObject(
             this, &UImmutablePassport::OnBatchNftTransferResponse));
}

void UImmutablePassport::Setup(
    const TWeakObjectPtr<UImtblJSConnector> Connector) {
  IMTBL_LOG_FUNCSIG

  if (!Connector.IsValid()) {
    IMTBL_ERR("Invalid JSConnector passed to UImmutablePassport::Initialize")
    return;
  }

  JSConnector = Connector.Get();
}

bool UImmutablePassport::CheckIsInitialized(
    const FString &Action,
    const FImtblPassportResponseDelegate &ResponseDelegate) const {
  if (!bIsInitialized) {
    IMTBL_WARN("Attempting action '%s' before Passport is initialized", *Action)
    ResponseDelegate.ExecuteIfBound(
        FImmutablePassportResult{false, "Passport is not initialized"});
  }
  return bIsInitialized;
}

void UImmutablePassport::CallJS(
    const FString &Action, const FString &Data,
    const FImtblPassportResponseDelegate &ClientResponseDelegate,
    const FImtblJSResponseDelegate &HandleJSResponse,
    const bool bCheckInitialized /*= true*/) {
  if (bCheckInitialized && !CheckIsInitialized(Action, ClientResponseDelegate))
    return;

  check(JSConnector.IsValid());
  const FString Guid = JSConnector->CallJS(Action, Data, HandleJSResponse);
  ResponseDelegates.Add(Guid, ClientResponseDelegate);
}

TOptional<UImmutablePassport::FImtblPassportResponseDelegate>
UImmutablePassport::GetResponseDelegate(const FImtblJSResponse &Response) {
  FImtblPassportResponseDelegate ResponseDelegate;
  if (!ResponseDelegates.RemoveAndCopyValue(Response.requestId,
                                            ResponseDelegate)) {
    IMTBL_WARN("Couldn't find delegate for %s response", *Response.responseFor)
    return TOptional<FImtblPassportResponseDelegate>();
  }
  return ResponseDelegate;
}

void UImmutablePassport::OnInitializeResponse(FImtblJSResponse Response) {
  if (auto ResponseDelegate = GetResponseDelegate(Response)) {
    FString Msg;
    if (Response.success) {
      bIsInitialized = true;
      IMTBL_LOG("Passport initialization succeeded.")
    } else {
      IMTBL_ERR("Passport initialization failed.")
      Response.Error.IsSet()
          ? Msg = Response.Error->ToString()
          : Msg = Response.JsonObject->GetStringField(TEXT("error"));
    }

    ResponseDelegate->ExecuteIfBound(
        FImmutablePassportResult{Response.success, Msg, Response});
  }
}

void UImmutablePassport::OnCheckStoredCredentialsResponse(
    FImtblJSResponse Response) {
  if (auto ResponseDelegate = GetResponseDelegate(Response)) {
    // Extract the credentials
    auto Credentials =
        JsonObjectToUStruct<FImmutablePassportTokenData>(Response.JsonObject);

    if (!Response.success || !Credentials.IsSet() ||
        !Credentials->accessToken.Len()) {
      IMTBL_LOG("No stored credentials found.");
      FString Msg;
      Response.Error.IsSet()
          ? Msg = Response.Error->ToString()
          : Msg = Response.JsonObject->GetStringField(TEXT("error"));
      ResponseDelegate->ExecuteIfBound(
          FImmutablePassportResult{false, Msg, Response});
    } else {
      IMTBL_LOG("Stored credentials found.");
      ResponseDelegate->ExecuteIfBound(
          FImmutablePassportResult{true, UStructToJsonString(*Credentials)});
    }
  }
}

void UImmutablePassport::OnConnectSilentResponse(FImtblJSResponse Response) {
  if (auto ResponseDelegate = GetResponseDelegate(Response)) {
    // Extract the credentials
    auto Credentials =
        JsonObjectToUStruct<FImmutablePassportTokenData>(Response.JsonObject);

    if (!Response.success || !Credentials.IsSet() ||
        !Credentials->accessToken.Len()) {
      IMTBL_LOG("No stored credentials found.");
      FString Msg;
      Response.Error.IsSet()
          ? Msg = Response.Error->ToString()
          : Msg = Response.JsonObject->GetStringField(TEXT("error"));
      ResponseDelegate->ExecuteIfBound(
          FImmutablePassportResult{false, Msg, Response});
      return;
    }

    IMTBL_LOG("Stored credentials found; attemping login.");

    CallJS(ImmutablePassportAction::ConnectWithCredentials,
           UStructToJsonString(*Credentials), ResponseDelegate.GetValue(),
           FImtblJSResponseDelegate::CreateUObject(
               this, &UImmutablePassport::OnConnectWithCredentialsResponse));
  }
}

void UImmutablePassport::OnConnectWithCredentialsResponse(
    FImtblJSResponse Response) {
  if (auto ResponseDelegate = GetResponseDelegate(Response)) {
    FString Msg;
    if (Response.success) {
      IMTBL_LOG("Connect with credentials succeeded.");
      bIsLoggedIn = true;
    } else {
      IMTBL_LOG("Connect with credentials failed.");
      Response.Error.IsSet()
          ? Msg = Response.Error->ToString()
          : Msg = Response.JsonObject->GetStringField(TEXT("error"));
      // Send log out action to Passport and move on, logging and ignoring the
      // response
      JSConnector->CallJS(ImmutablePassportAction::Logout, TEXT(""),
                          FImtblJSResponseDelegate::CreateUObject(
                              this, &UImmutablePassport::LogAndIgnoreResponse));
    }
    ResponseDelegate->ExecuteIfBound(
        FImmutablePassportResult{Response.success, Msg, Response});
  }
}

void UImmutablePassport::OnLogoutResponse(FImtblJSResponse Response) {
  if (auto ResponseDelegate = GetResponseDelegate(Response)) {
    FString Msg;
    if (Response.success) {
      IMTBL_LOG("Logged out.")
      bIsLoggedIn = false;
    } else {
      IMTBL_ERR("Error logging out.")
      Response.Error.IsSet()
          ? Msg = Response.Error->ToString()
          : Msg = Response.JsonObject->GetStringField(TEXT("error"));
    }
    ResponseDelegate->ExecuteIfBound(
        FImmutablePassportResult{Response.success, Msg, Response});
  }
}

void UImmutablePassport::OnConnectResponse(FImtblJSResponse Response) {
  if (auto ResponseDelegate = GetResponseDelegate(Response)) {
    const auto ConnectData =
        JsonObjectToUStruct<FImmutablePassportConnectData>(Response.JsonObject);
    if (!Response.success || !ConnectData || !ConnectData->code.Len()) {
      FString Msg;
      IMTBL_WARN("Connect attempt failed.");
      Response.Error.IsSet()
          ? Msg = Response.Error->ToString()
          : Msg = Response.JsonObject->GetStringField(TEXT("error"));
      ResponseDelegate->ExecuteIfBound(
          FImmutablePassportResult{false, Msg, Response});
      return;
    }
    FString Err;
    FPlatformProcess::LaunchURL(*ConnectData->url, nullptr, &Err);
    if (Err.Len()) {
      FString Msg = "Failed to connect to Browser: " + Err;
      IMTBL_ERR("%s", *Msg);
      ResponseDelegate->ExecuteIfBound(
          FImmutablePassportResult{false, Msg, Response});
      return;
    }
    ConfirmCode(ConnectData->deviceCode, ConnectData->interval,
                ResponseDelegate.GetValue());
  }
}

void UImmutablePassport::OnConnectEvmResponse(FImtblJSResponse Response) {
  if (auto ResponseDelegate = GetResponseDelegate(Response)) {
    FString Msg;
    if (Response.success) {
      IMTBL_LOG("Connected to Evm.")
    } else {
      IMTBL_WARN("Error connecting to Evm.")
      Response.Error.IsSet()
          ? Msg = Response.Error->ToString()
          : Msg = Response.JsonObject->GetStringField(TEXT("error"));
    }
    ResponseDelegate->ExecuteIfBound(
        FImmutablePassportResult{Response.success, Msg});
  }
}

#if PLATFORM_ANDROID | PLATFORM_IOS | PLATFORM_MAC
void UImmutablePassport::OnGetPKCEAuthUrlResponse(FImtblJSResponse Response) {
  if (PKCEResponseDelegate.IsBound()) {
    FString Msg;
    bool bSuccess = true;
    if (!Response.success ||
        !Response.JsonObject->HasTypedField<EJson::String>(TEXT("result"))) {
      IMTBL_LOG("Could not get PKCE auth URL from Passport.");
    } else {
      // Handle deeplink calls
      OnHandleDeepLink = FImtblPassportHandleDeepLinkDelegate::CreateUObject(
          this, &UImmutablePassport::OnDeepLinkActivated);

      FString Err;
      Msg = Response.JsonObject->GetStringField(TEXT("result"))
                .Replace(TEXT(" "), TEXT("+"));
#if PLATFORM_ANDROID
      OnPKCEDismissed = FImtblPassportOnPKCEDismissedDelegate::CreateUObject(
          this, &UImmutablePassport::HandleOnPKCEDismissed);

      JNIEnv *Env = FAndroidApplication::GetJavaEnv();
      if (Env) {
        jstring jurl = Env->NewStringUTF(TCHAR_TO_UTF8(*Msg));
        jclass jimmutableAndroidClass = FAndroidApplication::FindJavaClass(
            "com/immutable/unreal/ImmutableAndroid");
        static jmethodID jlaunchUrl = FJavaWrapper::FindStaticMethod(
            Env, jimmutableAndroidClass, "launchUrl",
            "(Landroid/app/Activity;Ljava/lang/String;)V", false);
        CallJniStaticVoidMethod(Env, jimmutableAndroidClass, jlaunchUrl,
                                FJavaWrapper::GameActivityThis, jurl);
      }
#elif PLATFORM_IOS
      [[ImmutableIOS instance] launchUrl:TCHAR_TO_ANSI(*Msg)];
#elif PLATFORM_MAC
      [[ImmutableMac instance] launchUrl:TCHAR_TO_ANSI(*Msg)
                          forRedirectUri:TCHAR_TO_ANSI(*InitData.redirectUri)];
#endif

      if (Err.Len()) {
        Msg = "Failed to connect to Browser: " + Err;
        IMTBL_ERR("%s", *Msg);
        bSuccess = false;
      } else {
        return;
      }
    }
    PKCEResponseDelegate.ExecuteIfBound(
        FImmutablePassportResult{bSuccess, Msg});
    PKCEResponseDelegate = nullptr;
  } else {
    IMTBL_ERR("Unable to return a response for Connect PKCE");
  }
}

void UImmutablePassport::OnConnectPKCEResponse(FImtblJSResponse Response) {
  if (PKCEResponseDelegate.IsBound()) {
    FString Msg;
    if (Response.success) {
      IMTBL_LOG("Successfully connected via PKCE")
      bIsLoggedIn = true;
    } else {
      IMTBL_WARN("Connect PKCE attempt failed.");
      Response.Error.IsSet()
          ? Msg = Response.Error->ToString()
          : Msg = Response.JsonObject->GetStringField(TEXT("error"));
    }
    PKCEResponseDelegate.ExecuteIfBound(
        FImmutablePassportResult{Response.success, Msg});
    PKCEResponseDelegate = nullptr;
  } else {
    IMTBL_ERR("Unable to return a response for Connect PKCE");
  }
#if PLATFORM_ANDROID
  completingPKCE = false;
#endif
}
#endif

void UImmutablePassport::OnGetAddressResponse(FImtblJSResponse Response) {
  if (auto ResponseDelegate = GetResponseDelegate(Response)) {
    auto ConnectData =
        JsonObjectToUStruct<FImmutablePassportConnectData>(Response.JsonObject);

    FString Msg;
    bool bSuccess = true;
    if (!Response.success ||
        !Response.JsonObject->HasTypedField<EJson::String>(TEXT("result"))) {
      IMTBL_WARN("Could not fetch address from Passport.");
      Response.Error.IsSet()
          ? Msg = Response.Error->ToString()
          : Msg = Response.JsonObject->GetStringField(TEXT("error"));
      bSuccess = false;
    } else {
      Msg = Response.JsonObject->GetStringField(TEXT("result"));
    }
    ResponseDelegate->ExecuteIfBound(
        FImmutablePassportResult{bSuccess, Msg, Response});
  }
}

void UImmutablePassport::OnZkEvmRequestAccountsResponse(
    FImtblJSResponse Response) {
  if (auto ResponseDelegate = GetResponseDelegate(Response)) {
    const auto RequestAccountstData =
        FImmutablePassportZkEvmRequestAccountsData::FromJsonObject(
            Response.JsonObject);
    FString Msg;
    bool bSuccess = true;
    if (!Response.success ||
        !Response.JsonObject->HasTypedField<EJson::Array>(TEXT("accounts"))) {
      IMTBL_WARN("Error requesting zkevm accounts.")
      Response.Error.IsSet()
          ? Msg = Response.Error->ToString()
          : Msg = Response.JsonObject->GetStringField(TEXT("error"));
      bSuccess = false;
    } else {
      const auto size = RequestAccountstData->accounts.Num();
      for (int32 Index = 0; Index != size; ++Index) {
        Msg += RequestAccountstData->accounts[Index];
        if (Index < size - 1) {
          Msg += TEXT(",");
        }
      }
    }
    ResponseDelegate->ExecuteIfBound(FImmutablePassportResult{bSuccess, Msg});
  }
}

void UImmutablePassport::OnZkEvmGetBalanceResponse(FImtblJSResponse Response) {
  if (auto ResponseDelegate = GetResponseDelegate(Response)) {
    FString Msg;
    bool bSuccess = true;
    if (!Response.success ||
        !Response.JsonObject->HasTypedField<EJson::String>(TEXT("result"))) {
      IMTBL_WARN("Could not get balance.");
      Response.Error.IsSet()
          ? Msg = Response.Error->ToString()
          : Msg = Response.JsonObject->GetStringField(TEXT("error"));
      bSuccess = false;
    } else {
      Msg = Response.JsonObject->GetStringField(TEXT("result"));
    }
    ResponseDelegate->ExecuteIfBound(
        FImmutablePassportResult{bSuccess, Msg, Response});
  }
}

void UImmutablePassport::OnZkEvmSendTransactionResponse(
    FImtblJSResponse Response) {
  if (auto ResponseDelegate = GetResponseDelegate(Response)) {
    FString Msg;
    bool bSuccess = true;
    if (!Response.success ||
        !Response.JsonObject->HasTypedField<EJson::String>(TEXT("result"))) {
      IMTBL_WARN("Could not fetch send transaction.");
      Response.Error.IsSet()
          ? Msg = Response.Error->ToString()
          : Msg = Response.JsonObject->GetStringField(TEXT("error"));
      bSuccess = false;
    } else {
      Msg = Response.JsonObject->GetStringField(TEXT("result"));
    }
    ResponseDelegate->ExecuteIfBound(FImmutablePassportResult{bSuccess, Msg});
  }
}

void UImmutablePassport::OnConfirmCodeResponse(FImtblJSResponse Response) {
  if (auto ResponseDelegate = GetResponseDelegate(Response)) {
    FString Msg;
    if (Response.success) {
      IMTBL_LOG("Log in code confirmed.")
      bIsLoggedIn = true;
    } else {
      IMTBL_WARN("Login code not confirmed.")
      Response.Error.IsSet()
          ? Msg = Response.Error->ToString()
          : Msg = Response.JsonObject->GetStringField(TEXT("error"));
    }
    ResponseDelegate->ExecuteIfBound(
        FImmutablePassportResult{Response.success, Msg, Response});
  }
}

void UImmutablePassport::OnGetEmailResponse(FImtblJSResponse Response) {
  if (auto ResponseDelegate = GetResponseDelegate(Response)) {
    auto ConnectData =
        JsonObjectToUStruct<FImmutablePassportConnectData>(Response.JsonObject);

    FString Msg;
    bool bSuccess = true;
    if (!Response.success ||
        !Response.JsonObject->HasTypedField<EJson::String>(TEXT("result"))) {
      IMTBL_WARN("Connect attempt failed.");
      Response.Error.IsSet()
          ? Msg = Response.Error->ToString()
          : Msg = Response.JsonObject->GetStringField(TEXT("error"));
      bSuccess = false;
    } else {
      Msg = Response.JsonObject->GetStringField(TEXT("result"));
    }
    ResponseDelegate->ExecuteIfBound(
        FImmutablePassportResult{bSuccess, Msg, Response});
  }
}

void UImmutablePassport::OnTransferResponse(FImtblJSResponse Response) {
  if (auto ResponseDelegate = GetResponseDelegate(Response)) {
    auto TransferResponse =
        JsonObjectToUStruct<FImxTransferResponse>(Response.JsonObject);

    FString Msg;
    bool bSuccess = true;
    if (!Response.success) {
      IMTBL_WARN("ImxTransfer failed.");
      Response.Error.IsSet()
          ? Msg = Response.Error->ToString()
          : Msg = Response.JsonObject->GetStringField(TEXT("error"));
      bSuccess = false;
    } else {
      Msg = Response.JsonObject->GetStringField(TEXT("status"));
    }
    ResponseDelegate->ExecuteIfBound(
        FImmutablePassportResult{bSuccess, Msg, Response});
  }
}

void UImmutablePassport::OnBatchNftTransferResponse(FImtblJSResponse Response) {
  if (auto ResponseDelegate = GetResponseDelegate(Response)) {
    auto BatchNftTransferResponse =
        JsonObjectToUStruct<FImxBatchNftTransferResponse>(Response.JsonObject);

    FString Msg;
    bool bSuccess = true;
    if (!Response.success ||
        !Response.JsonObject->HasTypedField<EJson::Object>(TEXT("result"))) {
      IMTBL_WARN("ImxBatchNftTransfer failed.");
      Response.Error.IsSet()
          ? Msg = Response.Error->ToString()
          : Msg = Response.JsonObject->GetStringField(TEXT("error"));
      bSuccess = false;
    } else {
      Msg = UStructToJsonString(*BatchNftTransferResponse);
    }
    ResponseDelegate->ExecuteIfBound(
        FImmutablePassportResult{bSuccess, Msg, Response});
  }
}

void UImmutablePassport::LogAndIgnoreResponse(FImtblJSResponse Response) {
  if (Response.success && !Response.Error) {
    IMTBL_LOG("Received success response from Passport for action %s",
              *Response.responseFor);
  } else {
    FString Msg;
    Response.Error.IsSet()
        ? Msg = Response.Error->ToString()
        : Msg = Response.JsonObject->GetStringField(TEXT("error"));
    IMTBL_WARN("Received error response from Passport for action %s -- %s",
               *Response.responseFor, *Msg);
  }
}

#if PLATFORM_ANDROID | PLATFORM_IOS | PLATFORM_MAC
void UImmutablePassport::OnDeepLinkActivated(FString DeepLink) {
  IMTBL_LOG("On Deep Link Activated: %s", *DeepLink);
  OnHandleDeepLink = nullptr;

  if (DeepLink.StartsWith(InitData.redirectUri)) {
    CompletePKCEFlow(DeepLink);
  }
}

void UImmutablePassport::CompletePKCEFlow(FString Url) {
#if PLATFORM_ANDROID
  completingPKCE = true;
#endif

  // Get code and state from deeplink URL
  TOptional<FString> Code, State;
  FString Endpoint, Params;
  Url.Split(TEXT("?"), &Endpoint, &Params);
  TArray<FString> ParamsArray;
  Params.ParseIntoArray(ParamsArray, TEXT("&"));
  for (FString Param : ParamsArray) {
    FString Key, Value;
    if (Param.StartsWith("code")) {
      Param.Split(TEXT("="), &Key, &Value);
      Code = Value;
    } else if (Param.StartsWith("state")) {
      Param.Split(TEXT("="), &Key, &Value);
      State = Value;
    }
  }

  if (!Code.IsSet() || !State.IsSet()) {
    const FString ErrorMsg =
        "Uri was missing state and/or code. Please call ConnectPKCE() again";
    IMTBL_ERR("%s", *ErrorMsg);
    PKCEResponseDelegate.ExecuteIfBound(
        FImmutablePassportResult{false, ErrorMsg});
    PKCEResponseDelegate = nullptr;
#if PLATFORM_ANDROID
    completingPKCE = false;
#endif
  } else {
    FImmutablePassportConnectPKCEData Data =
        FImmutablePassportConnectPKCEData{Code.GetValue(), State.GetValue()};
    CallJS(ImmutablePassportAction::ConnectPKCE, UStructToJsonString(Data),
           PKCEResponseDelegate,
           FImtblJSResponseDelegate::CreateUObject(
               this, &UImmutablePassport::OnConnectPKCEResponse));
  }
}
#endif

#if PLATFORM_ANDROID | PLATFORM_IOS | PLATFORM_MAC
#if PLATFORM_ANDROID
// Called from Android JNI
void UImmutablePassport::HandleDeepLink(FString DeepLink) {
#elif PLATFORM_IOS | PLATFORM_MAC
// Called from iOS Objective C
void UImmutablePassport::HandleDeepLink(NSString *sDeepLink) {
  FString DeepLink = FString(UTF8_TO_TCHAR([sDeepLink UTF8String]));
  IMTBL_LOG("Handle Deep Link: %s", *DeepLink);
#endif

  if (!OnHandleDeepLink.ExecuteIfBound(DeepLink)) {
    IMTBL_WARN("OnHandleDeepLink delegate was not called");
  }
}
#endif

#if PLATFORM_ANDROID
void UImmutablePassport::HandleOnPKCEDismissed() {
  IMTBL_LOG("Handle On PKCE Dismissed");
  FFunctionGraphTask::CreateAndDispatchWhenReady(
      [=]() {
        OnPKCEDismissed = nullptr;
        if (!completingPKCE) {
          // User hasn't entered all required details (e.g. email address) into
          // Passport yet
          IMTBL_LOG("PKCE dismissed before completing the flow");
          if (!PKCEResponseDelegate.ExecuteIfBound(
                  FImmutablePassportResult{false, "Cancelled"})) {
            IMTBL_WARN("PKCEResponseDelegate delegate was not called");
          }
          PKCEResponseDelegate = nullptr;
        } else {
          IMTBL_LOG("PKCE dismissed by user or SDK");
        }
      },
      TStatId(), nullptr, ENamedThreads::GameThread);
}

void UImmutablePassport::HandleCustomTabsDismissed() {
  IMTBL_LOG("On PKCE Dismissed");
  if (!OnPKCEDismissed.ExecuteIfBound()) {
    IMTBL_WARN("OnPKCEDismissed delegate was not called");
  }
}

void UImmutablePassport::CallJniStaticVoidMethod(JNIEnv *Env,
                                                 const jclass Class,
                                                 jmethodID Method, ...) {
  va_list Args;
  va_start(Args, Method);
  Env->CallStaticVoidMethodV(Class, Method, Args);
  va_end(Args);

  Env->DeleteLocalRef(Class);
}
#endif

// void UImmutablePassport::OnReady(const FOnPassportReadyDelegate::FDelegate&
// Delegate)
// {
//     if (JSConnector->IsBridgeReady())
//     {
//         // TODO: run on next tick rather than immediately
//         Delegate.ExecuteIfBound();
//     }
//     else
//     {
//         OnPassportReady.Add(Delegate);
//     }
// }
