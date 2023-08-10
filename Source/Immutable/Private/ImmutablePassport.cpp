// Fill out your copyright notice in the Description page of Project Settings.


#include "ImmutablePassport.h"

#include "JsonObjectConverter.h"
#include "ImtblJSConnector.h"
#include "ImtblLogging.h"


FString FImmutablePassportInitData::ToJsonString() const
{
    FString OutString;
    FJsonObjectWrapper Wrapper;
    FJsonObjectConverter::UStructToJsonObject(FImmutablePassportInitData::StaticStruct(), this, Wrapper.JsonObject.ToSharedRef());
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


FString FImmutablePassportTokenData::ToJsonString() const
{
    FString OutString;
    FJsonObjectConverter::UStructToJsonObjectString(*this, OutString, 0, 0, 0, nullptr, false);
    return OutString;
}


TOptional<FImmutablePassportTokenData> FImmutablePassportTokenData::FromJsonObject(const TSharedPtr<FJsonObject>& JsonObject)
{
    if (!JsonObject.IsValid())
        return TOptional<FImmutablePassportTokenData>();

    FImmutablePassportTokenData PassportToken;

    // Parse the JSON
    if (!FJsonObjectConverter::JsonObjectToUStruct(JsonObject.ToSharedRef(), &PassportToken, 0, 0))
    {
        IMTBL_ERR("Could not parse response from JavaScript into the expected Passport token format")
        return TOptional<FImmutablePassportTokenData>();
    }

    return PassportToken;
}


FString FImmutablePassportConnectData::ToJsonString() const
{
    FString OutString;
    FJsonObjectConverter::UStructToJsonObjectString(*this, OutString, 0, 0, 0, nullptr, false);
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


TOptional<FImmutablePassportConnectData> FImmutablePassportConnectData::FromJsonObject(const TSharedPtr<FJsonObject>& JsonObject)
{
    if (!JsonObject.IsValid())
        return TOptional<FImmutablePassportConnectData>();

    // Parse the JSON
    FImmutablePassportConnectData PassportConnect;
    if (!FJsonObjectConverter::JsonObjectToUStruct(JsonObject.ToSharedRef(), &PassportConnect, 0, 0))
    {
        IMTBL_ERR("Could not parse response from JavaScript into the expected Passport token format")
        return TOptional<FImmutablePassportConnectData>();
    }
    return PassportConnect;
}


FString FImmutablePassportCodeConfirmRequestData::ToJsonString() const
{
    FString OutString;
    FJsonObjectConverter::UStructToJsonObjectString(*this, OutString, 0, 0, 0, nullptr, false);
    return OutString;
}


void UImmutablePassport::Initialize(const FString& ClientID, const FImtblPassportResponseDelegate& ResponseDelegate)
{
    check(JSConnector.IsValid());

    ClientId = ClientID;

    CallJS(
        ImmutablePassportAction::Initialize,
        FImmutablePassportInitData{ClientID}.ToJsonString(),
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


void UImmutablePassport::ConfirmCode(const FString& DeviceCode, const float Interval, const FImtblPassportResponseDelegate& ResponseDelegate)
{
    FImmutablePassportCodeConfirmRequestData Data;
    Data.deviceCode = DeviceCode;
    Data.interval = Interval;
    CallJS(
        ImmutablePassportAction::ConfirmCode,
        Data.ToJsonString(),
        ResponseDelegate,
        FImtblJSResponseDelegate::CreateUObject(this, &UImmutablePassport::OnConfirmCodeResponse)
    );
}


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

        ResponseDelegate->ExecuteIfBound(FImmutablePassportResult{Response.success, Msg});
    }
}


void UImmutablePassport::OnCheckStoredCredentialsResponse(FImtblJSResponse Response)
{
    if (auto ResponseDelegate = GetResponseDelegate(Response))
    {
        // Extract the credentials 
        auto Credentials = FImmutablePassportTokenData::FromJsonObject(Response.JsonObject);

        if (!Response.success || !Credentials.IsSet() || !Credentials->accessToken.Len())
        {
            IMTBL_LOG("No stored credentials found.");
            FString Msg;
            if (Response.Error.IsSet())
                Msg = Response.Error->ToString();
            ResponseDelegate->ExecuteIfBound(FImmutablePassportResult{false, Msg});
        }
        else
        {
            IMTBL_LOG("Stored credentials found.");
            ResponseDelegate->ExecuteIfBound(FImmutablePassportResult{true, Credentials->ToJsonString()});
        }
    }
}


void UImmutablePassport::OnConnectSilentResponse(FImtblJSResponse Response)
{
     if (auto ResponseDelegate = GetResponseDelegate(Response))
     {
         // Extract the credentials 
         auto Credentials = FImmutablePassportTokenData::FromJsonObject(Response.JsonObject);

         if (!Response.success || !Credentials.IsSet() || !Credentials->accessToken.Len())
         {
             IMTBL_LOG("No stored credentials found.");
             FString Msg;
             if (Response.Error.IsSet())
                 Msg = Response.Error->ToString();
             ResponseDelegate->ExecuteIfBound(FImmutablePassportResult{false, Msg});
             return;
         }

         IMTBL_LOG("Stored credentials found; attemping login.");

         CallJS(
             ImmutablePassportAction::ConnectWithCredentials,
             Credentials->ToJsonString(),
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
        ResponseDelegate->ExecuteIfBound(FImmutablePassportResult{Response.success, Msg});
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
        ResponseDelegate->ExecuteIfBound(FImmutablePassportResult{Response.success, Msg});
    }
}


void UImmutablePassport::OnConnectResponse(FImtblJSResponse Response)
{
    if (auto ResponseDelegate = GetResponseDelegate(Response))
    {
        const auto ConnectData = FImmutablePassportConnectData::FromJsonObject(Response.JsonObject);
        if (!Response.success || !ConnectData || !ConnectData->code.Len())
        {
            FString Msg;
            IMTBL_LOG("Connect attempt failed.");
            if (Response.Error.IsSet())
            {
                Msg = Response.Error->ToString();
            }
            ResponseDelegate->ExecuteIfBound(FImmutablePassportResult{false, Msg});
            return;
        }
        FString Err;
        FPlatformProcess::LaunchURL(*ConnectData->url, nullptr, &Err);
        if (!Err.Len())
        {
            FString Msg = "Failed to connect to Browser: " + Err;
            IMTBL_ERR("%s", *Msg);
            ResponseDelegate->ExecuteIfBound(FImmutablePassportResult{false, Msg});
            return;
        }
        ConfirmCode(ConnectData->deviceCode, ConnectData->interval, ResponseDelegate.GetValue());
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
        ResponseDelegate->ExecuteIfBound(FImmutablePassportResult{Response.success, Msg});
    }
}


void UImmutablePassport::OnGetAddressResponse(FImtblJSResponse Response)
{
    if (auto ResponseDelegate = GetResponseDelegate(Response))
    {
        auto ConnectData = FImmutablePassportConnectData::FromJsonObject(Response.JsonObject);
        
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


void UImmutablePassport::OnGetEmailResponse(FImtblJSResponse Response)
{
    if (auto ResponseDelegate = GetResponseDelegate(Response))
    {
        auto ConnectData = FImmutablePassportConnectData::FromJsonObject(Response.JsonObject);
        
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
        ResponseDelegate->ExecuteIfBound(FImmutablePassportResult{bSuccess, Msg});
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
