// Fill out your copyright notice in the Description page of Project Settings.


#include "Immutable/ImmutablePassport.h"

#include "JsonObjectConverter.h"
#include "ImtblJSConnector.h"
#include "Immutable/Misc/ImtblLogging.h"

#if PLATFORM_ANDROID
#include "GenericPlatform/GenericPlatformHttp.h"
#include "Android/ImmutableAndroidJNI.h"
#endif

#if PLATFORM_ANDROID
static void HandleDeepLink(FString DeepLink)
{
    if (OnHandleDeepLink.ExecuteIfBound(DeepLink))
    {
        IMTBL_WARN("OnHandleDeepLink delegate was not called");
    }
}
#endif

FString FImmutablePassportInitData::ToJsonString() const
{
    FString OutString;

    FJsonObjectWrapper Wrapper;
    Wrapper.JsonObject = MakeShared<FJsonObject>();
    FJsonObjectConverter::UStructToJsonObject(FImmutablePassportInitData::StaticStruct(), this, Wrapper.JsonObject.ToSharedRef(), 0, 0);

    if (!Wrapper.JsonObject.IsValid())
    {
        IMTBL_ERR("Could not convert FImmutablePassportInitData to JSON")
        return "";
    }
    // Remove redirectUri field if it's empty so that the bridge doesn't try to use it
    if (Wrapper.JsonObject->HasField("redirectUri") && Wrapper.JsonObject->GetStringField("redirectUri").IsEmpty())
        Wrapper.JsonObject->RemoveField("redirectUri");
    Wrapper.JsonObjectToString(OutString);

    return OutString;
}


FString FImxBatchNftTransferRequest::ToJsonString() const
{
    FString OutString;
    FJsonObjectWrapper Wrapper;
    Wrapper.JsonObject = MakeShared<FJsonObject>();
    FJsonObjectConverter::UStructToJsonObject(FImxBatchNftTransferRequest::StaticStruct(), this, Wrapper.JsonObject.ToSharedRef(), 0, 0);

    if (!Wrapper.JsonObject.IsValid())
    {
        IMTBL_ERR("Could not convert FImxBatchNftTransferRequest to JSON")
        return "";
    }

    if (Wrapper.JsonObject->HasField("nftTransferDetails"))
    {
        const auto Writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&OutString);
        FJsonSerializer::Serialize(Wrapper.JsonObject->GetArrayField("nftTransferDetails"), Writer);
        IMTBL_LOG("FImxBatchNftTransferRequest Serialised: %s", *OutString);
        Writer->Close();
    }
    return OutString;
}

TOptional<FImmutablePassportConnectData> FImmutablePassportConnectData::FromJsonString(const FString& JsonObjectString)
{
    FImmutablePassportConnectData PassportConnect;
    if (!FJsonObjectConverter::JsonObjectStringToUStruct(JsonObjectString, &PassportConnect, 0, 0))
    {
        IMTBL_WARN("Could not parse response from JavaScript into the expected Passport connect format")
        return TOptional<FImmutablePassportConnectData>();
    }
    return PassportConnect;
}


FString FImmutablePassportZkEvmRequestAccountsData::ToJsonString() const
{
    FString OutString;
    FJsonObjectConverter::UStructToJsonObjectString(*this, OutString, 0, 0, 0, nullptr, false);
    return OutString;
}


FString FImmutablePassportConnectPKCEData::ToJsonString() const
{
    FString OutString;
    FJsonObjectConverter::UStructToJsonObjectString(*this, OutString, 0, 0, 0, nullptr, false);
    return OutString;
}


TOptional<FImmutablePassportZkEvmRequestAccountsData> FImmutablePassportZkEvmRequestAccountsData::FromJsonString(const FString& JsonObjectString)
{
    FImmutablePassportZkEvmRequestAccountsData RequestAccounts;
    if (!FJsonObjectConverter::JsonObjectStringToUStruct(JsonObjectString, &RequestAccounts, 0, 0))
    {
        IMTBL_WARN("Could not parse response from JavaScript into the expected Passport ZkEvm request accounts format")
        return TOptional<FImmutablePassportZkEvmRequestAccountsData>();
    }
    return RequestAccounts;
}


TOptional<FImmutablePassportZkEvmRequestAccountsData> FImmutablePassportZkEvmRequestAccountsData::FromJsonObject(const TSharedPtr<FJsonObject>& JsonObject)
{
    if (!JsonObject.IsValid())
        return TOptional<FImmutablePassportZkEvmRequestAccountsData>();

    // Parse the JSON
    FImmutablePassportZkEvmRequestAccountsData RequestAccounts;
    if (!FJsonObjectConverter::JsonObjectToUStruct(JsonObject.ToSharedRef(), &RequestAccounts, 0, 0))
    {
        IMTBL_ERR("Could not parse response from JavaScript into the expected Passport ZkEvm request accounts format")
        return TOptional<FImmutablePassportZkEvmRequestAccountsData>();
    }
    return RequestAccounts;
}

FString FImmutablePassportZkEvmGetBalanceData::ToJsonString() const
{
    FString OutString;

    FJsonObjectWrapper Wrapper;
    Wrapper.JsonObject = MakeShared<FJsonObject>();
    FJsonObjectConverter::UStructToJsonObject(FImmutablePassportZkEvmGetBalanceData::StaticStruct(), this, Wrapper.JsonObject.ToSharedRef(), 0, 0);

    if (!Wrapper.JsonObject.IsValid())
    {
        IMTBL_ERR("Could not convert FImmutablePassportZkEvmGetBalanceData to JSON")
        return "";
    }
    Wrapper.JsonObjectToString(OutString);

    return OutString;
}

// @param Environment can be one of ImmutablePassportAction::EnvSandbox or ImmutablePassportAction::EnvProduction
void UImmutablePassport::Initialize(const FImmutablePassportInitData& Data, const FImtblPassportResponseDelegate& ResponseDelegate)
{
    check(JSConnector.IsValid());

    InitData = Data;

    CallJS(
        ImmutablePassportAction::Initialize,
        InitData.ToJsonString(),
        ResponseDelegate,
        FImtblJSResponseDelegate::CreateUObject(this, &UImmutablePassport::OnInitializeResponse),
        false
    );
}

void UImmutablePassport::CheckStoredCredentials(const FImtblPassportResponseDelegate& ResponseDelegate)
{
    CallJS(
        ImmutablePassportAction::CheckStoredCredentials,
        TEXT(""),
        ResponseDelegate,
        FImtblJSResponseDelegate::CreateUObject(this, &UImmutablePassport::OnCheckStoredCredentialsResponse)
    );
}

void UImmutablePassport::ConnectSilent(const FImtblPassportResponseDelegate& ResponseDelegate)
{
    CallJS(
        ImmutablePassportAction::CheckStoredCredentials,
        TEXT(""),
        ResponseDelegate,
        FImtblJSResponseDelegate::CreateUObject(this, &UImmutablePassport::OnConnectSilentResponse)
    );
}

void UImmutablePassport::Logout(const FImtblPassportResponseDelegate& ResponseDelegate)
{
    CallJS(
        ImmutablePassportAction::Logout,
        TEXT(""),
        ResponseDelegate,
        FImtblJSResponseDelegate::CreateUObject(this, &UImmutablePassport::OnLogoutResponse)
    );
}

void UImmutablePassport::Connect(const FImtblPassportResponseDelegate& ResponseDelegate)
{
    CallJS(
        ImmutablePassportAction::Connect,
        TEXT(""),
        ResponseDelegate,
        FImtblJSResponseDelegate::CreateUObject(this, &UImmutablePassport::OnConnectResponse)
    );
}

void UImmutablePassport::ConnectEvm(const FImtblPassportResponseDelegate& ResponseDelegate)
{
    CallJS(
        ImmutablePassportAction::ConnectEvm,
        TEXT(""),
        ResponseDelegate,
        FImtblJSResponseDelegate::CreateUObject(this, &UImmutablePassport::OnConnectEvmResponse)
    );
}

void UImmutablePassport::ZkEvmRequestAccounts(const FImtblPassportResponseDelegate& ResponseDelegate)
{
    CallJS(
        ImmutablePassportAction::ZkEvmRequestAccounts,
        TEXT(""),
        ResponseDelegate,
        FImtblJSResponseDelegate::CreateUObject(this, &UImmutablePassport::OnZkEvmRequestAccountsResponse)
    );
}

void UImmutablePassport::ZkEvmGetBalance(const FImmutablePassportZkEvmGetBalanceData& Data, const FImtblPassportResponseDelegate& ResponseDelegate)
{
    CallJS(
        ImmutablePassportAction::ZkEvmGetBalance,
        Data.ToJsonString(),
        ResponseDelegate,
        FImtblJSResponseDelegate::CreateUObject(this, &UImmutablePassport::OnZkEvmGetBalanceResponse)
    );
}

void UImmutablePassport::ConfirmCode(const FString& DeviceCode, const float Interval, const FImtblPassportResponseDelegate& ResponseDelegate)
{
    FImmutablePassportCodeConfirmRequestData Data;
    Data.deviceCode = DeviceCode;
    Data.interval = Interval;
    CallJS(
        ImmutablePassportAction::ConfirmCode,
        UStructToJsonString(Data),
        ResponseDelegate,
        FImtblJSResponseDelegate::CreateUObject(this, &UImmutablePassport::OnConfirmCodeResponse)
    );
}


#if PLATFORM_ANDROID
void UImmutablePassport::ConnectPKCE(const FImtblPassportResponseDelegate& ResponseDelegate)
{
    PKCEResponseDelegate = ResponseDelegate;
    CallJS(
        ImmutablePassportAction::GetPKCEAuthUrl,
        TEXT(""),
        PKCEResponseDelegate,
        FImtblJSResponseDelegate::CreateUObject(this, &UImmutablePassport::OnGetPKCEAuthUrlResponse)
    );
}
#endif


void UImmutablePassport::GetAddress(const FImtblPassportResponseDelegate& ResponseDelegate)
{
    CallJS(
        ImmutablePassportAction::GetAddress,
        TEXT(""),
        ResponseDelegate,
        FImtblJSResponseDelegate::CreateUObject(this, &UImmutablePassport::OnGetAddressResponse)
    );
}

void UImmutablePassport::GetEmail(const FImtblPassportResponseDelegate& ResponseDelegate)
{
    CallJS(
        ImmutablePassportAction::GetEmail,
        TEXT(""),
        ResponseDelegate,
        FImtblJSResponseDelegate::CreateUObject(this, &UImmutablePassport::OnGetEmailResponse)
    );
}

void UImmutablePassport::Transfer(const FImxTransferRequest& RequestData, const FImtblPassportResponseDelegate& ResponseDelegate)
{
    IMTBL_LOG("Tranfer Request: %s", *UStructToJsonString(RequestData))
    CallJS(
        ImmutablePassportAction::Transfer,
        UStructToJsonString(RequestData),
        ResponseDelegate,
        FImtblJSResponseDelegate::CreateUObject(this, &UImmutablePassport::OnTransferResponse)
    );
}

void UImmutablePassport::BatchNftTransfer(const FImxBatchNftTransferRequest& RequestData, const FImtblPassportResponseDelegate& ResponseDelegate)
{
    CallJS(
        ImmutablePassportAction::BatchNftTransfer,
        RequestData.ToJsonString(),
        ResponseDelegate,
        FImtblJSResponseDelegate::CreateUObject(this, &UImmutablePassport::OnBatchNftTransferResponse)
    );
}

void UImmutablePassport::Setup(const TWeakObjectPtr<UImtblJSConnector> Connector)
{
    IMTBL_LOG_FUNCSIG

    if (!Connector.IsValid())
    {
        IMTBL_ERR("Invalid JSConnector passed to UImmutablePassport::Initialize")
        return;
    }

    JSConnector = Connector.Get();
}


bool UImmutablePassport::CheckIsInitialized(const FString& Action, const FImtblPassportResponseDelegate& ResponseDelegate) const
{
    if (!bIsInitialized)
    {
        IMTBL_WARN("Attempting action '%s' before Passport is initialized", *Action)
        ResponseDelegate.ExecuteIfBound(FImmutablePassportResult{false, "Passport is not initialized"});
    }
    return bIsInitialized;
}


void UImmutablePassport::CallJS(const FString& Action, const FString& Data, const FImtblPassportResponseDelegate& ClientResponseDelegate, const FImtblJSResponseDelegate& HandleJSResponse, const bool bCheckInitialized /*= true*/)
{
    if (bCheckInitialized && !CheckIsInitialized(Action, ClientResponseDelegate))
        return;

    check(JSConnector.IsValid());
    const FString Guid = JSConnector->CallJS(Action, Data, HandleJSResponse);
    ResponseDelegates.Add(Guid, ClientResponseDelegate);
}


TOptional<UImmutablePassport::FImtblPassportResponseDelegate> UImmutablePassport::GetResponseDelegate(const FImtblJSResponse& Response)
{
    FImtblPassportResponseDelegate ResponseDelegate;
    if (!ResponseDelegates.RemoveAndCopyValue(Response.requestId, ResponseDelegate))
    {
        IMTBL_WARN("Couldn't find delegate for %s response", *Response.responseFor)
        return TOptional<FImtblPassportResponseDelegate>();
    }
    return ResponseDelegate;
}


void UImmutablePassport::OnInitializeResponse(FImtblJSResponse Response)
{
    if (auto ResponseDelegate = GetResponseDelegate(Response))
    {
        FString Msg;
        if (Response.success)
        {
            bIsInitialized = true;
            IMTBL_LOG("Passport initialization succeeded.")
        }
        else
        {
            IMTBL_LOG("Passport initialization failed.")
            if (Response.Error.IsSet())
            {
                Msg = Response.Error->ToString();
            }
        }

        ResponseDelegate->ExecuteIfBound(FImmutablePassportResult{Response.success, Msg, Response});
    }
}


void UImmutablePassport::OnCheckStoredCredentialsResponse(FImtblJSResponse Response)
{
    if (auto ResponseDelegate = GetResponseDelegate(Response))
    {
        // Extract the credentials
        auto Credentials = JsonObjectToUStruct<FImmutablePassportTokenData>(Response.JsonObject);

        if (!Response.success || !Credentials.IsSet() || !Credentials->accessToken.Len())
        {
            IMTBL_LOG("No stored credentials found.");
            FString Msg;
            if (Response.Error.IsSet())
                Msg = Response.Error->ToString();
            ResponseDelegate->ExecuteIfBound(FImmutablePassportResult{false, Msg, Response});
        }
        else
        {
            IMTBL_LOG("Stored credentials found.");
            ResponseDelegate->ExecuteIfBound(FImmutablePassportResult{true, UStructToJsonString(*Credentials)});
        }
    }
}


void UImmutablePassport::OnConnectSilentResponse(FImtblJSResponse Response)
{
     if (auto ResponseDelegate = GetResponseDelegate(Response))
     {
         // Extract the credentials
         auto Credentials = JsonObjectToUStruct<FImmutablePassportTokenData>(Response.JsonObject);

         if (!Response.success || !Credentials.IsSet() || !Credentials->accessToken.Len())
         {
             IMTBL_LOG("No stored credentials found.");
             FString Msg;
             if (Response.Error.IsSet())
                 Msg = Response.Error->ToString();
             ResponseDelegate->ExecuteIfBound(FImmutablePassportResult{false, Msg, Response});
             return;
         }

         IMTBL_LOG("Stored credentials found; attemping login.");

         CallJS(
             ImmutablePassportAction::ConnectWithCredentials,
             UStructToJsonString(*Credentials),
             ResponseDelegate.GetValue(),
             FImtblJSResponseDelegate::CreateUObject(this, &UImmutablePassport::OnConnectWithCredentialsResponse)
         );
     }
}


void UImmutablePassport::OnConnectWithCredentialsResponse(FImtblJSResponse Response)
{
    if (auto ResponseDelegate = GetResponseDelegate(Response))
    {
        FString Msg;
        if (Response.success)
        {
            IMTBL_LOG("Connect with credentials succeeded.");
            bIsLoggedIn = true;
        }
        else
        {
            IMTBL_LOG("Connect with credentials failed.");
            if (Response.Error.IsSet())
                Msg = Response.Error->ToString();
            // Send log out action to Passport and move on, logging and ignoring the response
            JSConnector->CallJS(ImmutablePassportAction::Logout, TEXT(""), FImtblJSResponseDelegate::CreateUObject(this, &UImmutablePassport::LogAndIgnoreResponse));
        }
        ResponseDelegate->ExecuteIfBound(FImmutablePassportResult{Response.success, Msg, Response});
    }
}


void UImmutablePassport::OnLogoutResponse(FImtblJSResponse Response)
{
    if (auto ResponseDelegate = GetResponseDelegate(Response))
    {
        FString Msg;
        if (Response.success)
        {
            IMTBL_LOG("Logged out.")
            bIsLoggedIn = false;
        }
        else
        {
            IMTBL_LOG("Error logging out.")
            if (Response.Error.IsSet())
                Msg = Response.Error->ToString();
        }
        ResponseDelegate->ExecuteIfBound(FImmutablePassportResult{Response.success, Msg, Response});
    }
}


void UImmutablePassport::OnConnectResponse(FImtblJSResponse Response)
{
    if (auto ResponseDelegate = GetResponseDelegate(Response))
    {
        const auto ConnectData = JsonObjectToUStruct<FImmutablePassportConnectData>(Response.JsonObject);
        if (!Response.success || !ConnectData || !ConnectData->code.Len())
        {
            FString Msg;
            IMTBL_LOG("Connect attempt failed.");
            if (Response.Error.IsSet())
            {
                Msg = Response.Error->ToString();
            }
            ResponseDelegate->ExecuteIfBound(FImmutablePassportResult{false, Msg, Response});
            return;
        }
        FString Err;
        FPlatformProcess::LaunchURL(*ConnectData->url, nullptr, &Err);
        if (Err.Len())
        {
            FString Msg = "Failed to connect to Browser: " + Err;
            IMTBL_ERR("%s", *Msg);
            ResponseDelegate->ExecuteIfBound(FImmutablePassportResult{false, Msg, Response});
            return;
        }
        ConfirmCode(ConnectData->deviceCode, ConnectData->interval, ResponseDelegate.GetValue());
    }
}

void UImmutablePassport::OnConnectEvmResponse(FImtblJSResponse Response)
{
    if (auto ResponseDelegate = GetResponseDelegate(Response))
    {
        FString Msg;
        if (Response.success)
        {
            IMTBL_LOG("Connected to Evm.")
        }
        else
        {
            IMTBL_LOG("Error connecting to Evm.")
            if (Response.Error.IsSet())
                Msg = Response.Error->ToString();
        }
        ResponseDelegate->ExecuteIfBound(FImmutablePassportResult{Response.success, Msg});
    }
}

#if PLATFORM_ANDROID
void UImmutablePassport::OnGetPKCEAuthUrlResponse(FImtblJSResponse Response)
{
    if (PKCEResponseDelegate.IsBound())
    {
        FString Msg;
        bool bSuccess = true;
        if (!Response.success || !Response.JsonObject->HasTypedField<EJson::String>(TEXT("result")))
        {
            IMTBL_LOG("Could not get PKCE auth URL from Passport.");
        }
        else
        {
            // Handle deeplink calls
            SetDeeplinkCallbackMethod(HandleDeepLink);
            OnHandleDeepLink = FImtblPassportHandleDeepLinkDelegate::CreateUObject(this, &UImmutablePassport::OnDeepLinkActivated);

            FString Err;
            Msg = Response.JsonObject->GetStringField(TEXT("result"));
            FPlatformProcess::LaunchURL(*Msg, nullptr, &Err);
            if (Err.Len())
            {
                Msg = "Failed to connect to Browser: " + Err;
                IMTBL_ERR("%s", *Msg);
                bSuccess = false;
            }
            else
            {
                return;
            }
        }
        PKCEResponseDelegate.ExecuteIfBound(FImmutablePassportResult{bSuccess, Msg});
        PKCEResponseDelegate = nullptr;
    }
    else
    {
        IMTBL_ERR("Unable to return a response for Connect PKCE");
    }
}

void UImmutablePassport::OnConnectPKCEResponse(FImtblJSResponse Response)
{
    if (PKCEResponseDelegate.IsBound())
    {
        FString Msg;
        if (Response.success)
        {
            IMTBL_LOG("Successfully connected via PKCE")
            bIsLoggedIn = true;
        }
        else
        {
            IMTBL_LOG("Connect PKCE attempt failed.");
            if (Response.Error.IsSet())
                Msg = Response.Error->ToString();
        }
        PKCEResponseDelegate.ExecuteIfBound(FImmutablePassportResult{Response.success, Msg});
        PKCEResponseDelegate = nullptr;
    }
    else
    {
        IMTBL_ERR("Unable to return a response for Connect PKCE");
    }
}
#endif


void UImmutablePassport::OnGetAddressResponse(FImtblJSResponse Response)
{
    if (auto ResponseDelegate = GetResponseDelegate(Response))
    {
        auto ConnectData = JsonObjectToUStruct<FImmutablePassportConnectData>(Response.JsonObject);

        FString Msg;
        bool bSuccess = true;
        if (!Response.success || !Response.JsonObject->HasTypedField<EJson::String>(TEXT("result")))
        {
            IMTBL_LOG("Could not fetch address from Passport.");
            if (Response.Error.IsSet())
                Msg = Response.Error->ToString();
            bSuccess = false;
        }
        else
        {
            Msg = Response.JsonObject->GetStringField(TEXT("result"));
        }
        ResponseDelegate->ExecuteIfBound(FImmutablePassportResult{bSuccess, Msg, Response});
    }
}


void UImmutablePassport::OnZkEvmRequestAccountsResponse(FImtblJSResponse Response)
{
    if (auto ResponseDelegate = GetResponseDelegate(Response))
    {
        const auto RequestAccountstData = FImmutablePassportZkEvmRequestAccountsData::FromJsonObject(Response.JsonObject);
        FString Msg;
        bool bSuccess = true;
        if (!Response.success || !Response.JsonObject->HasTypedField<EJson::Array>(TEXT("accounts")))
        {
            IMTBL_LOG("Error requesting zkevm accounts.")
            if (Response.Error.IsSet())
                Msg = Response.Error->ToString();
            bSuccess = false;
        }
        else
        {
            const auto size = RequestAccountstData->accounts.Num();
            for (int32 Index = 0; Index != size; ++Index)
            {
                Msg += RequestAccountstData->accounts[Index];
                if (Index < size - 1)
                {
                    Msg += TEXT(",");
                }
            }
        }
        ResponseDelegate->ExecuteIfBound(FImmutablePassportResult{bSuccess, Msg});
    }
}


void UImmutablePassport::OnZkEvmGetBalanceResponse(FImtblJSResponse Response)
{
    if (auto ResponseDelegate = GetResponseDelegate(Response))
    {
        FString Msg;
        bool bSuccess = true;
        if (!Response.success || !Response.JsonObject->HasTypedField<EJson::String>(TEXT("result")))
        {
            IMTBL_LOG("Could not fetch address from Passport.");
            if (Response.Error.IsSet())
                Msg = Response.Error->ToString();
            bSuccess = false;
        }
        else
        {
            Msg = Response.JsonObject->GetStringField(TEXT("result"));
        }
        ResponseDelegate->ExecuteIfBound(FImmutablePassportResult{bSuccess, Msg});
    }
}

void UImmutablePassport::OnConfirmCodeResponse(FImtblJSResponse Response)
{
    if (auto ResponseDelegate = GetResponseDelegate(Response))
    {
        FString Msg;
        if (Response.success)
        {
            IMTBL_LOG("Log in code confirmed.")
            bIsLoggedIn = true;
        }
        else
        {
            IMTBL_LOG("Login code not confirmed.")
            if (Response.Error.IsSet())
                Msg = Response.Error->ToString();
        }
        ResponseDelegate->ExecuteIfBound(FImmutablePassportResult{Response.success, Msg, Response});
    }
}


void UImmutablePassport::OnGetEmailResponse(FImtblJSResponse Response)
{
    if (auto ResponseDelegate = GetResponseDelegate(Response))
    {
        auto ConnectData = JsonObjectToUStruct<FImmutablePassportConnectData>(Response.JsonObject);

        FString Msg;
        bool bSuccess = true;
        if (!Response.success || !Response.JsonObject->HasTypedField<EJson::String>(TEXT("result")))
        {
            IMTBL_LOG("Connect attempt failed.");
            if (Response.Error.IsSet())
                Msg = Response.Error->ToString();
            bSuccess = false;
        }
        else
        {
            Msg = Response.JsonObject->GetStringField(TEXT("result"));
        }
        ResponseDelegate->ExecuteIfBound(FImmutablePassportResult{bSuccess, Msg, Response});
    }
}

void UImmutablePassport::OnTransferResponse(FImtblJSResponse Response)
{
    if (auto ResponseDelegate = GetResponseDelegate(Response))
    {
        auto TransferResponse = JsonObjectToUStruct<FImxTransferResponse>(Response.JsonObject);

        FString Msg;
        bool bSuccess = true;
        if (!Response.success || !Response.JsonObject->HasTypedField<EJson::String>(TEXT("result")))
        {
            IMTBL_LOG("Transfer failed.");
            if (Response.Error.IsSet())
            {
                Msg = Response.Error->ToString();
            }
            bSuccess = false;
        }
        else
        {
            Msg = Response.JsonObject->GetStringField(TEXT("result"));
        }
        ResponseDelegate->ExecuteIfBound(FImmutablePassportResult{bSuccess, Msg, Response});
    }
}

void UImmutablePassport::OnBatchNftTransferResponse(FImtblJSResponse Response)
{
    if (auto ResponseDelegate = GetResponseDelegate(Response))
    {
        auto BatchNftTransferResponse = JsonObjectToUStruct<FImxBatchNftTransferResponse>(Response.JsonObject);

        FString Msg;
        bool bSuccess = true;
        if (!Response.success || !Response.JsonObject->HasTypedField<EJson::Object>(TEXT("result")))
        {
            IMTBL_LOG("Transfer failed.");
            if (Response.Error.IsSet())
            {
                Msg = Response.Error->ToString();
            }
            bSuccess = false;
        }
        else
        {
            Msg = UStructToJsonString(*BatchNftTransferResponse);
        }
        ResponseDelegate->ExecuteIfBound(FImmutablePassportResult{bSuccess, Msg, Response});
    }
}


void UImmutablePassport::LogAndIgnoreResponse(FImtblJSResponse Response)
{
    if (Response.success && !Response.Error)
    {
        IMTBL_LOG("Received success response from Passport for action %s", *Response.responseFor);
    }
    else
    {
        IMTBL_WARN("Received error response from Passport for action %s -- %s", *Response.responseFor, *Response.Error->ToString());
    }
}


#if PLATFORM_ANDROID
void UImmutablePassport::OnDeepLinkActivated(FString DeepLink)
{
    if (DeepLink.StartsWith(InitData.redirectUri))
    {
        CompletePKCEFlow(DeepLink);
    }
}


void UImmutablePassport::CompletePKCEFlow(FString Url)
{
    TOptional<FString> Code = FGenericPlatformHttp::GetUrlParameter(Url, TEXT("code"));
    IMTBL_LOG("CompletePKCEFlow code: %s", *(Code.GetValue()));
    TOptional<FString> State = FGenericPlatformHttp::GetUrlParameter(Url, TEXT("state"));
    IMTBL_LOG("CompletePKCEFlow State: %s", *(State.GetValue()));

    if (!Code.IsSet() || !State.IsSet())
    {
        const FString ErrorMsg = "Uri was missing state and/or code. Please call ConnectPKCE() again";
        IMTBL_ERR("%s", *ErrorMsg);
        PKCEResponseDelegate.ExecuteIfBound(FImmutablePassportResult{false, ErrorMsg});
        PKCEResponseDelegate = nullptr;
    }
    else
    {
        FImmutablePassportConnectPKCEData Data = FImmutablePassportConnectPKCEData{Code.GetValue(), State.GetValue()};
        CallJS(
            ImmutablePassportAction::ConnectPKCE,
            Data.ToJsonString(),
            PKCEResponseDelegate,
            FImtblJSResponseDelegate::CreateUObject(this, &UImmutablePassport::OnConnectPKCEResponse)
        );
    }
}
#endif


// void UImmutablePassport::OnReady(const FOnPassportReadyDelegate::FDelegate& Delegate)
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
