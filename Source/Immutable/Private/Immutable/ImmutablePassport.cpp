// Fill out your copyright notice in the Description page of Project Settings.

#include "Immutable/ImmutablePassport.h"

#include "Immutable/Misc/ImtblLogging.h"
#include "Immutable/ImmutableResponses.h"
#include "ImtblJSConnector.h"
#include "JsonObjectConverter.h"
#include "Immutable/ImmutableSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "Policies/CondensedJsonPrintPolicy.h"


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

#define PASSPORT_SAVE_GAME_SLOT_NAME TEXT("ImmutablePassport")

FString FImmutablePassportInitData::ToJsonString() const
{
	FString OutString;

	FJsonObjectWrapper Wrapper;
	Wrapper.JsonObject = MakeShared<FJsonObject>();
	FJsonObjectConverter::UStructToJsonObject(StaticStruct(), this, Wrapper.JsonObject.ToSharedRef(), 0, 0);

	if (!Wrapper.JsonObject.IsValid())
	{
		IMTBL_ERR("Could not convert FImmutablePassportInitData to JSON")
		return "";
	}
	// Remove redirectUri field if it's empty so that the bridge doesn't try to
	// use it
	if (Wrapper.JsonObject->HasField("redirectUri") && Wrapper.JsonObject->GetStringField("redirectUri").IsEmpty())
	{
		Wrapper.JsonObject->RemoveField("redirectUri");
	}
	Wrapper.JsonObjectToString(OutString);

	return OutString;
}

FString FImxBatchNftTransferRequest::ToJsonString() const
{
	FString OutString;
	FJsonObjectWrapper Wrapper;
	Wrapper.JsonObject = MakeShared<FJsonObject>();
	FJsonObjectConverter::UStructToJsonObject(StaticStruct(), this, Wrapper.JsonObject.ToSharedRef(), 0, 0);

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

TOptional<FImmutablePassportInitDeviceFlowData> FImmutablePassportInitDeviceFlowData::FromJsonString(const FString& JsonObjectString)
{
	FImmutablePassportInitDeviceFlowData PassportConnect;
	if (!FJsonObjectConverter::JsonObjectStringToUStruct(JsonObjectString, &PassportConnect, 0, 0))
	{
		IMTBL_WARN("Could not parse response from JavaScript into the expected " "Passport connect format")
		return TOptional<FImmutablePassportInitDeviceFlowData>();
	}
	return PassportConnect;
}

FString FImmutablePassportZkEvmRequestAccountsData::ToJsonString() const
{
	FString OutString;
	FJsonObjectConverter::UStructToJsonObjectString(*this, OutString, 0, 0, 0, nullptr, false);
	return OutString;
}

TOptional<FImmutablePassportZkEvmRequestAccountsData> FImmutablePassportZkEvmRequestAccountsData::FromJsonString(
	const FString& JsonObjectString)
{
	FImmutablePassportZkEvmRequestAccountsData RequestAccounts;
	if (!FJsonObjectConverter::JsonObjectStringToUStruct(JsonObjectString, &RequestAccounts, 0, 0))
	{
		IMTBL_WARN("Could not parse response from JavaScript into the expected "
			"Passport ZkEvm request accounts format")
		return TOptional<FImmutablePassportZkEvmRequestAccountsData>();
	}
	return RequestAccounts;
}

TOptional<FImmutablePassportZkEvmRequestAccountsData> FImmutablePassportZkEvmRequestAccountsData::FromJsonObject(
	const TSharedPtr<FJsonObject>& JsonObject)
{
	if (!JsonObject.IsValid()) { return TOptional<FImmutablePassportZkEvmRequestAccountsData>(); }

	// Parse the JSON
	FImmutablePassportZkEvmRequestAccountsData RequestAccounts;
	if (!FJsonObjectConverter::JsonObjectToUStruct(JsonObject.ToSharedRef(), &RequestAccounts, 0, 0))
	{
		IMTBL_ERR("Could not parse response from JavaScript into the expected "
			"Passport ZkEvm request accounts format")
		return TOptional<FImmutablePassportZkEvmRequestAccountsData>();
	}
	return RequestAccounts;
}

FString FImmutablePassportZkEvmGetBalanceData::ToJsonString() const
{
	FString OutString;

	FJsonObjectWrapper Wrapper;
	Wrapper.JsonObject = MakeShared<FJsonObject>();
	FJsonObjectConverter::UStructToJsonObject(StaticStruct(), this, Wrapper.JsonObject.ToSharedRef(), 0, 0);

	if (!Wrapper.JsonObject.IsValid())
	{
		IMTBL_ERR("Could not convert FImmutablePassportZkEvmGetBalanceData to JSON")
		return "";
	}
	Wrapper.JsonObjectToString(OutString);

	return OutString;
}

void UImmutablePassport::Initialize(const FImmutablePassportInitData& Data,
                                    const FImtblPassportResponseDelegate& ResponseDelegate)
{
	check(JSConnector.IsValid());

	InitData = Data;

	CallJS(ImmutablePassportAction::INIT, InitData.ToJsonString(), ResponseDelegate,
	       FImtblJSResponseDelegate::CreateUObject(this, &UImmutablePassport::OnInitializeResponse), false);
}

void UImmutablePassport::Connect(bool IsConnectImx, bool TryToRelogin, const FImtblPassportResponseDelegate& ResponseDelegate)
{
	SetStateFlags(IPS_CONNECTING);
	if (IsConnectImx)
	{
		SetStateFlags(IPS_IMX);
	}
	if (TryToRelogin)
	{
		CallJS(IsConnectImx ? ImmutablePassportAction::RECONNECT : ImmutablePassportAction::RELOGIN, TEXT(""), ResponseDelegate,
			FImtblJSResponseDelegate::CreateUObject(this, &UImmutablePassport::ReinstateConnection));
	}
	else
	{
		CallJS(ImmutablePassportAction::INIT_DEVICE_FLOW, TEXT(""), ResponseDelegate,
			FImtblJSResponseDelegate::CreateUObject(this, &UImmutablePassport::OnInitDeviceFlowResponse));
	}
}

#if PLATFORM_ANDROID | PLATFORM_IOS | PLATFORM_MAC
void UImmutablePassport::ConnectPKCE(bool IsConnectImx, const FImtblPassportResponseDelegate &ResponseDelegate)
{
	SetStateFlags(IPS_CONNECTING|IPS_PKCE);
	if (IsConnectImx)
	{
		SetStateFlags(IPS_IMX);
	}
	PKCEResponseDelegate = ResponseDelegate;
	CallJS(ImmutablePassportAction::GetPKCEAuthUrl, TEXT(""), PKCEResponseDelegate,
		FImtblJSResponseDelegate::CreateUObject(this, &UImmutablePassport::OnGetPKCEAuthUrlResponse));
}
#endif

void UImmutablePassport::Logout(const FImtblPassportResponseDelegate& ResponseDelegate)
{
#if PLATFORM_ANDROID | PLATFORM_IOS | PLATFORM_MAC
	if (IsStateFlagsSet(IPS_PKCE) || bIsPrevConnectedViaPKCEFlow)
	{
		PKCELogoutResponseDelegate = ResponseDelegate;
	}
#endif
	if (IsStateFlagsSet(IPS_CONNECTED))
	{
		CallJS(ImmutablePassportAction::Logout, TEXT(""), ResponseDelegate,
		   FImtblJSResponseDelegate::CreateUObject(this, &UImmutablePassport::OnLogoutResponse));
	}
	else
	{
		IMTBL_WARN("Passport is not connected to execute logout.");
	}
}

void UImmutablePassport::ConnectEvm(const FImtblPassportResponseDelegate& ResponseDelegate)
{
	CallJS(ImmutablePassportAction::ConnectEvm, TEXT(""), ResponseDelegate,
	       FImtblJSResponseDelegate::CreateUObject(this, &UImmutablePassport::OnConnectEvmResponse));
}

void UImmutablePassport::ZkEvmRequestAccounts(const FImtblPassportResponseDelegate& ResponseDelegate)
{
	CallJS(ImmutablePassportAction::ZkEvmRequestAccounts, TEXT(""), ResponseDelegate,
	       FImtblJSResponseDelegate::CreateUObject(this, &UImmutablePassport::OnZkEvmRequestAccountsResponse));
}

void UImmutablePassport::ZkEvmGetBalance(const FImmutablePassportZkEvmGetBalanceData& Data,
                                         const FImtblPassportResponseDelegate& ResponseDelegate)
{
	CallJS(ImmutablePassportAction::ZkEvmGetBalance, Data.ToJsonString(), ResponseDelegate,
	       FImtblJSResponseDelegate::CreateUObject(this, &UImmutablePassport::OnZkEvmGetBalanceResponse));
}

void UImmutablePassport::ZkEvmSendTransaction(const FImtblTransactionRequest& Request,
                                              const FImtblPassportResponseDelegate& ResponseDelegate)
{
	CallJS(ImmutablePassportAction::ZkEvmSendTransaction, UStructToJsonString(Request), ResponseDelegate,
	       FImtblJSResponseDelegate::CreateUObject(this, &UImmutablePassport::OnZkEvmSendTransactionResponse));
}

void UImmutablePassport::ConfirmCode(const FString& DeviceCode, const float Interval,
                                     const FImtblPassportResponseDelegate& ResponseDelegate)
{
	FImmutablePassportCodeConfirmRequestData Data { DeviceCode, Interval };
	FString Action = IsStateFlagsSet(IPS_IMX) ? ImmutablePassportAction::CONNECT_CONFIRM_CODE : ImmutablePassportAction::LOGIN_CONFIRM_CODE;
	
	CallJS(Action, UStructToJsonString(Data), ResponseDelegate, FImtblJSResponseDelegate::CreateUObject(this, &UImmutablePassport::OnConfirmCodeResponse));
}

void UImmutablePassport::GetIdToken(const FImtblPassportResponseDelegate& ResponseDelegate)
{
	CallJS(ImmutablePassportAction::GetIdToken, TEXT(""), ResponseDelegate,
	       FImtblJSResponseDelegate::CreateUObject(this, &UImmutablePassport::OnGetIdTokenResponse));
}

void UImmutablePassport::GetAccessToken(const FImtblPassportResponseDelegate& ResponseDelegate)
{
	CallJS(ImmutablePassportAction::GetAccessToken, TEXT(""), ResponseDelegate,
	       FImtblJSResponseDelegate::CreateUObject(this, &UImmutablePassport::OnGetAccessTokenResponse));
}

void UImmutablePassport::GetAddress(const FImtblPassportResponseDelegate& ResponseDelegate)
{
	CallJS(ImmutablePassportAction::GetAddress, TEXT(""), ResponseDelegate,
	       FImtblJSResponseDelegate::CreateUObject(this, &UImmutablePassport::OnGetAddressResponse));
}

void UImmutablePassport::GetEmail(const FImtblPassportResponseDelegate& ResponseDelegate)
{
	CallJS(ImmutablePassportAction::GetEmail, TEXT(""), ResponseDelegate,
	       FImtblJSResponseDelegate::CreateUObject(this, &UImmutablePassport::OnGetEmailResponse));
}

void UImmutablePassport::ImxTransfer(const FImxTransferRequest& RequestData,
                                     const FImtblPassportResponseDelegate& ResponseDelegate)
{
	IMTBL_LOG("Tranfer Request: %s", *UStructToJsonString(RequestData))
	CallJS(ImmutablePassportAction::ImxTransfer, UStructToJsonString(RequestData), ResponseDelegate,
	       FImtblJSResponseDelegate::CreateUObject(this, &UImmutablePassport::OnTransferResponse));
}

void UImmutablePassport::ImxBatchNftTransfer(const FImxBatchNftTransferRequest& RequestData,
                                             const FImtblPassportResponseDelegate& ResponseDelegate)
{
	CallJS(ImmutablePassportAction::ImxBatchNftTransfer, RequestData.ToJsonString(), ResponseDelegate,
	       FImtblJSResponseDelegate::CreateUObject(this, &UImmutablePassport::OnBatchNftTransferResponse));
}

void UImmutablePassport::ImxIsRegisteredOffchain(const FImtblPassportResponseDelegate& ResponseDelegate)
{
	CallJS(ImmutablePassportAction::ImxIsRegisteredOffchain, TEXT(""), ResponseDelegate,
	       FImtblJSResponseDelegate::CreateUObject(this, &UImmutablePassport::OnImxIsRegisteredOffchain));
}

void UImmutablePassport::ImxRegisterOffchain(const FImtblPassportResponseDelegate& ResponseDelegate)
{
	CallJS(ImmutablePassportAction::ImxRegisterOffchain, TEXT(""), ResponseDelegate,
	       FImtblJSResponseDelegate::CreateUObject(this, &UImmutablePassport::OnImxRegisterOffchain));
}

void UImmutablePassport::HasStoredCredentials(const FImtblPassportResponseDelegate& ResponseDelegate)
{
	// we do check credentials into two steps, we check accessToken and then IdToken
	// check access token
	CallJS(ImmutablePassportAction::GetAccessToken, TEXT(""), ResponseDelegate, FImtblJSResponseDelegate::CreateLambda(
		       [=](FImtblJSResponse Response)
		       {
			       FString AccessToken;

			       Response.JsonObject->TryGetStringField(TEXT("result"), AccessToken);
			       if (!Response.success || AccessToken.IsEmpty())
			       {
				       ResponseDelegate.ExecuteIfBound(FImmutablePassportResult{
					       false,
					       Response.JsonObject->HasField(TEXT("error"))
						       ? Response.JsonObject->GetStringField(TEXT("error"))
						       : "Failed to retrieve Access Token.",
					       Response
				       });
			       }
			       else
			       {
				       // check for id token
				       CallJS(ImmutablePassportAction::GetIdToken, TEXT(""), ResponseDelegate,
				              FImtblJSResponseDelegate::CreateLambda([ResponseDelegate](FImtblJSResponse Response)
				              {
					              FString IdToken;

					              Response.JsonObject->TryGetStringField(TEXT("result"), IdToken);
					              if (!Response.success || IdToken.IsEmpty())
					              {
						              ResponseDelegate.ExecuteIfBound(FImmutablePassportResult{
							              false,
							              Response.JsonObject->HasField(TEXT("error"))
								              ? Response.JsonObject->GetStringField(TEXT("error"))
								              : "Failed to retrieve Id Token.",
							              Response
						              });
					              }
					              else
					              {
						              ResponseDelegate.ExecuteIfBound(FImmutablePassportResult{
							              Response.success, "", Response
						              });
					              }
				              }));
			       }
		       }));
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

void UImmutablePassport::ReinstateConnection(FImtblJSResponse Response)
{
	ResetStateFlags(IPS_CONNECTING);

	if (auto ResponseDelegate = GetResponseDelegate(Response))
	{
		// currently, this response has to be called only for RELOGIN AND RECONNECT bridge routines
		const FString CallbackName = (Response.responseFor.Compare(ImmutablePassportAction::RELOGIN, ESearchCase::IgnoreCase) == 0) ? "Relogin" : "Reconnect";

		if (Response.JsonObject->GetBoolField(TEXT("result")))
		{
			SetStateFlags(IPS_CONNECTED);
			ResponseDelegate->ExecuteIfBound(FImmutablePassportResult{ true, "", Response });
		}
		else
		{
			CallJS(ImmutablePassportAction::INIT_DEVICE_FLOW, TEXT(""), ResponseDelegate.GetValue(),
				FImtblJSResponseDelegate::CreateUObject(this, &UImmutablePassport::OnInitDeviceFlowResponse));
		}
	}
}

bool UImmutablePassport::CheckIsInitialized(const FString& Action, const FImtblPassportResponseDelegate& ResponseDelegate) const
{
	const bool IsInitialized = IsStateFlagsSet(IPS_INITIALIZED);
	
	if (!IsInitialized)
	{
		IMTBL_WARN("Attempting action '%s' before Passport is initialized", *Action)
		ResponseDelegate.ExecuteIfBound(FImmutablePassportResult{false, "Passport is not initialized"});
	}
	
	return IsInitialized;
}

void UImmutablePassport::CallJS(const FString& Action, const FString& Data,
                                const FImtblPassportResponseDelegate& ClientResponseDelegate,
                                const FImtblJSResponseDelegate& HandleJSResponse,
                                const bool bCheckInitialized /*= true*/)
{
	if (bCheckInitialized && !CheckIsInitialized(Action, ClientResponseDelegate)) { return; }

	check(JSConnector.IsValid());
	const FString Guid = JSConnector->CallJS(Action, Data, HandleJSResponse);
	ResponseDelegates.Add(Guid, ClientResponseDelegate);
}

TOptional<UImmutablePassport::FImtblPassportResponseDelegate> UImmutablePassport::GetResponseDelegate(
	const FImtblJSResponse& Response)
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
			SetStateFlags(IPS_INITIALIZED);
			IMTBL_LOG("Passport initialization succeeded.")
			// we load settings in case if player has not logged out properly
			LoadPassportSettings();
		}
		else
		{
			IMTBL_ERR("Passport initialization failed.")
			Response.Error.IsSet()
				? Msg = Response.Error->ToString()
				: Msg = Response.JsonObject->GetStringField(TEXT("error"));
		}

		ResponseDelegate->ExecuteIfBound(FImmutablePassportResult{ Response.success, Msg, Response });
	}
}

void UImmutablePassport::OnInitDeviceFlowResponse(FImtblJSResponse Response)
{
	if (auto ResponseDelegate = GetResponseDelegate(Response))
	{
		const auto InitDeviceFlowData = JsonObjectToUStruct<FImmutablePassportInitDeviceFlowData>(Response.JsonObject);

		if (!Response.success || !InitDeviceFlowData || !InitDeviceFlowData->code.Len())
		{
			FString Msg;
			
			IMTBL_WARN("Login device flow initialization attempt failed.");
			Response.Error.IsSet() ? Msg = Response.Error->ToString() : Msg = Response.JsonObject->GetStringField(TEXT("error"));
			ResponseDelegate->ExecuteIfBound(FImmutablePassportResult{ false, Msg, Response });
			
			return;
		}
		FString Err;

		FPlatformProcess::LaunchURL(*InitDeviceFlowData->url, nullptr, &Err);
		if (Err.Len())
		{
			FString Msg = "Failed to connect to Browser: " + Err;
			
			IMTBL_ERR("%s", *Msg);
			ResponseDelegate->ExecuteIfBound(FImmutablePassportResult{ false, Msg, Response });
			return;
		}
		ConfirmCode(InitDeviceFlowData->deviceCode, InitDeviceFlowData->interval, ResponseDelegate.GetValue());
	}
}

void UImmutablePassport::OnLogoutResponse(FImtblJSResponse Response)
{
	if (auto ResponseDelegate = GetResponseDelegate(Response))
	{
		if (Response.success)
		{
			FString Url;
			FString Err;
			
			Response.JsonObject->TryGetStringField(TEXT("result"), Url);
			if (!Url.IsEmpty())
			{
#if PLATFORM_ANDROID | PLATFORM_IOS | PLATFORM_MAC
				if (IsStateFlagsSet(IPS_PKCE) || bIsPrevConnectedViaPKCEFlow)
				{
					OnHandleDeepLink = FImtblPassportHandleDeepLinkDelegate::CreateUObject(this, &UImmutablePassport::OnDeepLinkActivated);
#if PLATFORM_ANDROID
					LaunchAndroidUrl(Url);
#elif PLATFORM_IOS
					[[ImmutableIOS instance] launchUrl:TCHAR_TO_ANSI(*Url)];
#elif PLATFORM_MAC
					[[ImmutableMac instance] launchUrl:TCHAR_TO_ANSI(*Url) forRedirectUri:TCHAR_TO_ANSI(*InitData.logoutRedirectUri)];
#endif
				}
				else
				{
#endif
					FPlatformProcess::LaunchURL(*Url, nullptr, &Err);
					if (Err.Len())
					{
						FString Msg = "Failed to connect to Browser: " + Err;
						IMTBL_ERR("%s", *Msg);
						ResponseDelegate->ExecuteIfBound(FImmutablePassportResult{ false, Msg, Response });
						return;
					}
					IMTBL_LOG("Logged out.")
					ResponseDelegate->ExecuteIfBound(FImmutablePassportResult{ Response.success, "Logged out" });
#if PLATFORM_ANDROID | PLATFORM_IOS | PLATFORM_MAC
				}
#endif
			}
			else
			{
				ResponseDelegate->ExecuteIfBound(FImmutablePassportResult{ false, "Logout Url is empty", Response });
			}
			ResetStateFlags(IPS_CONNECTED);
		}
		else
		{
			FString Msg = Response.Error.IsSet() ? Response.Error->ToString() : Response.JsonObject->GetStringField(TEXT("error"));

			ResponseDelegate->ExecuteIfBound(FImmutablePassportResult{ Response.success, Msg, Response });

			IMTBL_ERR("Error logging out.")
		}
	}
}

void UImmutablePassport::OnConnectSilentResponse(FImtblJSResponse Response)
{
	if (auto ResponseDelegate = GetResponseDelegate(Response))
	{
		FString Msg;

		if (Response.success)
		{
			IMTBL_LOG("Reconnected.")
		}
		else
		{
			IMTBL_ERR("Failed to reconnect.")
			Response.Error.IsSet() ? Msg = Response.Error->ToString() : Msg = Response.JsonObject->GetStringField(TEXT("error"));
		}

		ResponseDelegate->ExecuteIfBound(FImmutablePassportResult{ Response.success, Msg, Response });
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
			IMTBL_WARN("Error connecting to Evm.")
			Response.Error.IsSet()
				? Msg = Response.Error->ToString()
				: Msg = Response.JsonObject->GetStringField(TEXT("error"));
		}
		ResponseDelegate->ExecuteIfBound(FImmutablePassportResult{Response.success, Msg});
	}
}

#if PLATFORM_ANDROID | PLATFORM_IOS | PLATFORM_MAC
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
			OnHandleDeepLink = FImtblPassportHandleDeepLinkDelegate::CreateUObject(this, &UImmutablePassport::OnDeepLinkActivated);

			Msg = Response.JsonObject->GetStringField(TEXT("result")).Replace(TEXT(" "), TEXT("+"));
#if PLATFORM_ANDROID
			OnPKCEDismissed = FImtblPassportOnPKCEDismissedDelegate::CreateUObject(this, &UImmutablePassport::HandleOnLoginPKCEDismissed);
			LaunchAndroidUrl(Msg);
#elif PLATFORM_IOS
			[[ImmutableIOS instance] launchUrl:TCHAR_TO_ANSI(*Msg)];
#elif PLATFORM_MAC
			[[ImmutableMac instance] launchUrl:TCHAR_TO_ANSI(*Msg) forRedirectUri:TCHAR_TO_ANSI(*InitData.redirectUri)];
#endif
		}
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
			SetStateFlags(IPS_CONNECTED);
		}
		else
		{
			IMTBL_WARN("Connect PKCE attempt failed.");
			ResetStateFlags(IPS_PKCE);
			Response.Error.IsSet() ? Msg = Response.Error->ToString() : Msg = Response.JsonObject->GetStringField(TEXT("error"));
		}
		PKCEResponseDelegate.ExecuteIfBound(FImmutablePassportResult{Response.success, Msg});
		PKCEResponseDelegate = nullptr;

		// we save passport state for PKCE flow in case if we decide to close a game
		// and reopen it later with Passport is still being connected and we decided to logout.
		// In this case, we logout using PKCE flow
		SavePassportSettings();
	}
	else
	{
		IMTBL_ERR("Unable to return a response for Connect PKCE");
	}
	ResetStateFlags(IPS_COMPLETING_PKCE);
}
#endif

void UImmutablePassport::OnGetIdTokenResponse(FImtblJSResponse Response)
{
	if (auto ResponseDelegate = GetResponseDelegate(Response))
	{
		FString IdToken;

		Response.JsonObject->TryGetStringField(TEXT("result"), IdToken);

		if (!Response.success || IdToken.IsEmpty())
		{
			IMTBL_LOG("Failed to retrieve Id Token.");

			const FString Msg = Response.JsonObject->HasField(TEXT("error"))
				                    ? Response.JsonObject->GetStringField(TEXT("error"))
				                    : "Failed to retrieve Id Token.";

			ResponseDelegate->ExecuteIfBound(FImmutablePassportResult{false, Msg, Response});
		}
		else
		{
			IMTBL_LOG("Retrieved Id Token.");
			ResponseDelegate->ExecuteIfBound(FImmutablePassportResult{true, IdToken});
		}
	}
}

void UImmutablePassport::OnGetAccessTokenResponse(FImtblJSResponse Response)
{
	if (auto ResponseDelegate = GetResponseDelegate(Response))
	{
		FString AccessToken;

		Response.JsonObject->TryGetStringField(TEXT("result"), AccessToken);

		if (!Response.success || AccessToken.IsEmpty())
		{
			IMTBL_LOG("Failed to retrieve Access Token");

			const FString Msg = Response.JsonObject->HasField(TEXT("error"))
				                    ? Response.JsonObject->GetStringField(TEXT("error"))
				                    : "Failed to retrieve Access Token.";

			ResponseDelegate->ExecuteIfBound(FImmutablePassportResult{false, Msg, Response});
		}
		else
		{
			IMTBL_LOG("Retrieved Access Token.");
			ResponseDelegate->ExecuteIfBound(FImmutablePassportResult{true, AccessToken});
		}
	}
}

void UImmutablePassport::OnGetAddressResponse(FImtblJSResponse Response)
{
	if (auto ResponseDelegate = GetResponseDelegate(Response))
	{
		FString Msg;
		bool bSuccess = true;
		
		if (!Response.success || !Response.JsonObject->HasTypedField<EJson::String>(TEXT("result")))
		{
			IMTBL_WARN("Could not fetch address from Passport.");
			Response.Error.IsSet()
				? Msg = Response.Error->ToString()
				: Msg = Response.JsonObject->GetStringField(TEXT("error"));
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
		const auto RequestAccountstData = FImmutablePassportZkEvmRequestAccountsData::FromJsonObject(
			Response.JsonObject);
		FString Msg;
		bool bSuccess = true;
		if (!Response.success || !Response.JsonObject->HasTypedField<EJson::Array>(TEXT("accounts")))
		{
			IMTBL_WARN("Error requesting zkevm accounts.")
			Response.Error.IsSet()
				? Msg = Response.Error->ToString()
				: Msg = Response.JsonObject->GetStringField(TEXT("error"));
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
			IMTBL_WARN("Could not get balance.");
			Response.Error.IsSet()
				? Msg = Response.Error->ToString()
				: Msg = Response.JsonObject->GetStringField(TEXT("error"));
			bSuccess = false;
		}
		else
		{
			Msg = Response.JsonObject->GetStringField(TEXT("result"));
		}
		ResponseDelegate->ExecuteIfBound(FImmutablePassportResult{bSuccess, Msg, Response});
	}
}

void UImmutablePassport::OnZkEvmSendTransactionResponse(FImtblJSResponse Response)
{
	if (auto ResponseDelegate = GetResponseDelegate(Response))
	{
		FString Msg;
		bool bSuccess = true;
		if (!Response.success || !Response.JsonObject->HasTypedField<EJson::String>(TEXT("result")))
		{
			IMTBL_WARN("Could not fetch send transaction.");
			Response.Error.IsSet()
				? Msg = Response.Error->ToString()
				: Msg = Response.JsonObject->GetStringField(TEXT("error"));
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
		FString TypeOfConnection = IsStateFlagsSet(IPS_IMX) ? TEXT("connect") : TEXT("login");

		ResetStateFlags(IPS_CONNECTING);
		if (Response.success)
		{
			IMTBL_LOG("Code confirmed for %s operation.", *TypeOfConnection)
			SetStateFlags(IPS_CONNECTED);
		}
		else
		{
			IMTBL_LOG("%s code not confirmed.", *TypeOfConnection)
			Response.Error.IsSet() ? Msg = Response.Error->ToString() : Msg = Response.JsonObject->GetStringField(TEXT("error"));
		}
		ResponseDelegate->ExecuteIfBound(FImmutablePassportResult{ Response.success, Msg, Response });
	}
}

void UImmutablePassport::OnGetEmailResponse(FImtblJSResponse Response)
{
	if (auto ResponseDelegate = GetResponseDelegate(Response))
	{
		FString Msg;
		bool bSuccess = true;
		
		if (!Response.success || !Response.JsonObject->HasTypedField<EJson::String>(TEXT("result")))
		{
			IMTBL_WARN("Connect attempt failed.");
			Response.Error.IsSet()
				? Msg = Response.Error->ToString()
				: Msg = Response.JsonObject->GetStringField(TEXT("error"));
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
		FString Msg;
		bool bSuccess = true;
		if (!Response.success)
		{
			IMTBL_WARN("ImxTransfer failed.");
			Response.Error.IsSet()
				? Msg = Response.Error->ToString()
				: Msg = Response.JsonObject->GetStringField(TEXT("error"));
			bSuccess = false;
		}
		else
		{
			Msg = Response.JsonObject->GetStringField(TEXT("status"));
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
			IMTBL_WARN("ImxBatchNftTransfer failed.");
			Response.Error.IsSet()
				? Msg = Response.Error->ToString()
				: Msg = Response.JsonObject->GetStringField(TEXT("error"));
			bSuccess = false;
		}
		else
		{
			Msg = UStructToJsonString(*BatchNftTransferResponse);
		}
		ResponseDelegate->ExecuteIfBound(FImmutablePassportResult{bSuccess, Msg, Response});
	}
}

void UImmutablePassport::OnImxIsRegisteredOffchain(FImtblJSResponse Response)
{
	if (auto ResponseDelegate = GetResponseDelegate(Response))
	{
		FString Msg;
		bool bResult = false;

		if (!Response.success)
		{
			IMTBL_WARN("ImxIsRegisteredOffchain failed.");
			Response.Error.IsSet()
				? Msg = Response.Error->ToString()
				: Msg = Response.JsonObject->GetStringField(TEXT("error"));
		}
		else
		{
			bResult = Response.JsonObject->GetBoolField(TEXT("result"));
		}
		ResponseDelegate->ExecuteIfBound(FImmutablePassportResult{bResult, Msg, Response});
	}
}

void UImmutablePassport::OnImxRegisterOffchain(FImtblJSResponse Response)
{
	if (auto ResponseDelegate = GetResponseDelegate(Response))
	{
		FString Msg;

		if (!Response.success)
		{
			IMTBL_WARN("ImxRegisterOffchain failed.");
			Response.Error.IsSet()
				? Msg = Response.Error->ToString()
				: Msg = Response.JsonObject->GetStringField(TEXT("error"));
		}

		ResponseDelegate->ExecuteIfBound(FImmutablePassportResult{Response.success, Msg, Response});
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
		FString Msg;
		Response.Error.IsSet()
			? Msg = Response.Error->ToString()
			: Msg = Response.JsonObject->GetStringField(TEXT("error"));
		IMTBL_WARN("Received error response from Passport for action %s -- %s", *Response.responseFor, *Msg);
	}
}

void UImmutablePassport::SetStateFlags(uint8 StateIn)
{
	StateFlags |= StateIn;
}

void UImmutablePassport::ResetStateFlags(uint8 StateIn)
{
	StateFlags &= ~StateIn;
}

bool UImmutablePassport::IsStateFlagsSet(uint8 StateIn) const
{
	return (StateFlags & StateIn) == StateIn;
}

void UImmutablePassport::SavePassportSettings()
{
	UImmutableSaveGame* SaveGameInstance = Cast<UImmutableSaveGame>(UGameplayStatics::CreateSaveGameObject(UImmutableSaveGame::StaticClass()));

	SaveGameInstance->bWasConnectedViaPKCEFlow = IsStateFlagsSet(IPS_PKCE|IPS_CONNECTED);
	
	UGameplayStatics::SaveGameToSlot(SaveGameInstance, PASSPORT_SAVE_GAME_SLOT_NAME, 0);
}

void UImmutablePassport::LoadPassportSettings()
{
	UImmutableSaveGame* SaveGameInstance = Cast<UImmutableSaveGame>(UGameplayStatics::CreateSaveGameObject(UImmutableSaveGame::StaticClass()));
	
	SaveGameInstance = Cast<UImmutableSaveGame>(UGameplayStatics::LoadGameFromSlot(PASSPORT_SAVE_GAME_SLOT_NAME, 0));

	if (!SaveGameInstance)
	{
		IMTBL_ERR("Could not find Immutable save game to load")
		return;
	}

	bIsPrevConnectedViaPKCEFlow = SaveGameInstance->bWasConnectedViaPKCEFlow;
}

#if PLATFORM_ANDROID | PLATFORM_IOS | PLATFORM_MAC
void UImmutablePassport::OnDeepLinkActivated(FString DeepLink)
{
	IMTBL_LOG_FUNC("URL : %s", *DeepLink);
	OnHandleDeepLink = nullptr;
	if (DeepLink.StartsWith(InitData.logoutRedirectUri))
	{
        // execute on game thread to prevent call to Passport instance from another thread
		if (FTaskGraphInterface::IsRunning())
		{
			FGraphEventRef GameThreadTask = FFunctionGraphTask::CreateAndDispatchWhenReady([this]()
			{
				PKCELogoutResponseDelegate.ExecuteIfBound(FImmutablePassportResult{ true, "Logged out" });
				PKCELogoutResponseDelegate = nullptr;
				ResetStateFlags(IPS_CONNECTED|IPS_PKCE|IPS_IMX);
				SavePassportSettings();
			}, TStatId(), nullptr, ENamedThreads::GameThread);
		}
	}
	else if (DeepLink.StartsWith(InitData.redirectUri))
	{
		CompleteLoginPKCEFlow(DeepLink);
	}
}

void UImmutablePassport::CompleteLoginPKCEFlow(FString Url)
{
	// Required mainly for Android to detect when Chrome Custom tabs is dismissed
	// See HandleOnLoginPKCEDismissed
	SetStateFlags(IPS_COMPLETING_PKCE);

	// Get code and state from deeplink URL
	TOptional<FString> Code, State;
	FString Endpoint, Params;
	Url.Split(TEXT("?"), &Endpoint, &Params);
	TArray<FString> ParamsArray;

	Params.ParseIntoArray(ParamsArray, TEXT("&"));
	for (FString Param : ParamsArray)
	{
		FString Key, Value;

		if (Param.StartsWith("code"))
		{
			Param.Split(TEXT("="), &Key, &Value);
			Code = Value;
		}
		else if (Param.StartsWith("state"))
		{
			Param.Split(TEXT("="), &Key, &Value);
			State = Value;
		}
	}

	if (!Code.IsSet() || !State.IsSet())
	{
		const FString ErrorMsg = "Uri was missing state and/or code. Please call ConnectPKCE() again";
		
		IMTBL_ERR("%s", *ErrorMsg);
		PKCEResponseDelegate.ExecuteIfBound(FImmutablePassportResult{false, ErrorMsg});
		PKCEResponseDelegate = nullptr;
		ResetStateFlags(IPS_PKCE|IPS_CONNECTING|IPS_COMPLETING_PKCE);
		SavePassportSettings();
	}
	else
	{
		FImmutablePassportConnectPKCEData Data = FImmutablePassportConnectPKCEData{Code.GetValue(), State.GetValue()};

		CallJS(IsStateFlagsSet(IPS_IMX)? ImmutablePassportAction::CONNECT_PKCE : ImmutablePassportAction::LOGIN_PKCE,
			UStructToJsonString(Data), PKCEResponseDelegate, FImtblJSResponseDelegate::CreateUObject(this, &UImmutablePassport::OnConnectPKCEResponse));
	}
}

#endif

#if PLATFORM_ANDROID | PLATFORM_IOS | PLATFORM_MAC
#if PLATFORM_ANDROID
// Called from Android JNI
void UImmutablePassport::HandleDeepLink(FString DeepLink) const
{
#elif PLATFORM_IOS | PLATFORM_MAC
// Called from iOS Objective C
void UImmutablePassport::HandleDeepLink(NSString *sDeepLink) const
{
	FString DeepLink = FString(UTF8_TO_TCHAR([sDeepLink UTF8String]));
	IMTBL_LOG("Handle Deep Link: %s", *DeepLink);
#endif

	if (!OnHandleDeepLink.ExecuteIfBound(DeepLink))
	{
		IMTBL_WARN("OnHandleDeepLink delegate was not called");
	}
}
#endif

#if PLATFORM_ANDROID
void UImmutablePassport::HandleOnLoginPKCEDismissed()
{
	IMTBL_LOG("Handle On Login PKCE Dismissed");
	OnPKCEDismissed = nullptr;

	// If the second part of PKCE (CompleteLoginPKCEFlow) has not started yet and custom tabs is dismissed,
	// this means the user manually dismissed the custom tabs before entering all
	// all required details (e.g. email address) into Passport
	// Cannot use IPS_CONNECTING as that is set when PKCE flow is initiated. Here we are checking against the second
	// half of the PKCE flow.
	if (!IsStateFlagsSet(IPS_COMPLETING_PKCE))
	{
		// User hasn't entered all required details (e.g. email address) into
		// Passport yet
		IMTBL_LOG("Login PKCE dismissed before completing the flow");
		if (!PKCEResponseDelegate.ExecuteIfBound(FImmutablePassportResult{ false, "Cancelled" }))
		{
			IMTBL_WARN("Login PKCEResponseDelegate delegate was not called");
		}
		PKCEResponseDelegate = nullptr;
	}
	else
	{
		IMTBL_LOG("PKCE dismissed by user or SDK");
	}
}

void UImmutablePassport::HandleCustomTabsDismissed(FString Url)
{
	IMTBL_LOG("On PKCE Dismissed");

	if (!OnPKCEDismissed.ExecuteIfBound())
	{
		IMTBL_WARN("OnPKCEDismissed delegate was not called");
	}
}

void UImmutablePassport::CallJniStaticVoidMethod(JNIEnv *Env, const jclass Class, jmethodID Method, ...)
{
	va_list Args;

	va_start(Args, Method);
	Env->CallStaticVoidMethodV(Class, Method, Args);
	va_end(Args);
	Env->DeleteLocalRef(Class);
}

void UImmutablePassport::LaunchAndroidUrl(FString Url)
{
	if (JNIEnv *Env = FAndroidApplication::GetJavaEnv())
	{
		jstring jurl = Env->NewStringUTF(TCHAR_TO_UTF8(*Url));
		jclass jimmutableAndroidClass = FAndroidApplication::FindJavaClass("com/immutable/unreal/ImmutableActivity");
		static jmethodID jlaunchUrl = FJavaWrapper::FindStaticMethod(Env, jimmutableAndroidClass, "startActivity", "(Landroid/app/Activity;Ljava/lang/String;)V", false);
					
		CallJniStaticVoidMethod(Env, jimmutableAndroidClass, jlaunchUrl, FJavaWrapper::GameActivityThis, jurl);
	}
}
#endif
