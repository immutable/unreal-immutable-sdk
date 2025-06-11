// Fill out your copyright notice in the Description page of Project Settings.

#include "Immutable/ImmutablePassport.h"

#include "ImmutableAnalytics.h"
#include "Immutable/Misc/ImtblLogging.h"
#include "Immutable/ImtblJSConnector.h"
#include "JsonObjectConverter.h"
#include "Immutable/ImmutableSaveGame.h"
#include "Immutable/ImmutableSettings.h"
#include "Kismet/GameplayStatics.h"

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
#if PLATFORM_WINDOWS
#include "Immutable/Windows/ImmutablePKCEWindows.h"
#endif

#define PASSPORT_SAVE_GAME_SLOT_NAME TEXT("Immutable")

void UImmutablePassport::Initialize(const FImmutablePassportInitData& Data, const FImtblPassportResponseDelegate& ResponseDelegate)
{
	check(JSConnector.IsValid());
	
	LoadPassportSettings();
	// we check saved settings in case if player has not logged out properly
	if (Data.logoutRedirectUri.IsEmpty() && IsStateFlagsSet(IPS_PKCE))
	{
		IMTBL_ERR("Logout URI is empty. Previously logged in via PKCE.")
		ResetStateFlags(IPS_PKCE);
		SavePassportSettings();
	}

	InitData = Data;

	CallJS(ImmutablePassportAction::INIT, InitData.ToJsonString(), ResponseDelegate, FImtblJSResponseDelegate::CreateUObject(this, &UImmutablePassport::OnInitializeResponse), false);
}

void UImmutablePassport::Initialize(const FImtblPassportResponseDelegate& ResponseDelegate)
{
	check(JSConnector.IsValid());

	const UImmutableSettings* Settings = GetDefault<UImmutableSettings>();

	if (!Settings)
	{
		ResponseDelegate.ExecuteIfBound(FImmutablePassportResult{false, "Failed to find Immutable Settings"});

		return;
	}

	UApplicationConfig* ApplicationConfig = Settings->DefaultApplicationConfig.GetDefaultObject();

	if (!ApplicationConfig)
	{
		ResponseDelegate.ExecuteIfBound(FImmutablePassportResult{false, "Failed to retrieve default application configuration for Passport initialization"});

		return;
	}

	InitData.clientId = ApplicationConfig->GetClientID();
	InitData.environment = ApplicationConfig->GetEnvironmentString();
	InitData.redirectUri = ApplicationConfig->GetRedirectURL();
	InitData.logoutRedirectUri = ApplicationConfig->GetLogoutURL();

	LoadPassportSettings();
	// we check saved settings in case if player has not logged out properly
	if (InitData.logoutRedirectUri.IsEmpty() && IsStateFlagsSet(IPS_PKCE))
	{
		IMTBL_ERR("Logout URI is empty. Previously logged in via PKCE.")
		ResetStateFlags(IPS_PKCE);
		SavePassportSettings();
	}

	CallJS(ImmutablePassportAction::INIT, InitData.ToJsonString(), ResponseDelegate, FImtblJSResponseDelegate::CreateUObject(this, &UImmutablePassport::OnInitializeResponse), false);
}

#if PLATFORM_ANDROID | PLATFORM_IOS | PLATFORM_MAC | PLATFORM_WINDOWS
void UImmutablePassport::Connect(bool IsConnectImx, const FImtblPassportResponseDelegate& ResponseDelegate)
{
	SetStateFlags(IPS_CONNECTING | IPS_PKCE);

#if PLATFORM_WINDOWS
	// Verify PKCEData is null before initializing to ensure we're not overriding an active PKCE operation.
	// A non-null value indicates another PKCE operation is already in progress.
	ensureAlways(!PKCEData);
	PKCEData = UImmutablePKCEWindows::Initialise(InitData);
	if (PKCEData)
	{
		PKCEData->DynamicMulticastDelegate_DeepLinkCallback.AddDynamic(this, &ThisClass::OnDeepLinkActivated);
	}
#endif

	if (IsConnectImx)
	{
		SetStateFlags(IPS_IMX);
	}
	PKCEResponseDelegate = ResponseDelegate;
	Analytics->Track(IsConnectImx ? UImmutableAnalytics::EEventName::START_CONNECT_IMX_PKCE : UImmutableAnalytics::EEventName::START_LOGIN_PKCE);
	CallJS(ImmutablePassportAction::GetPKCEAuthUrl, TEXT(""), PKCEResponseDelegate, FImtblJSResponseDelegate::CreateUObject(this, &UImmutablePassport::OnGetAuthUrlResponse));
}
#endif

void UImmutablePassport::Logout(bool DoHardLogout, const FImtblPassportResponseDelegate& ResponseDelegate)
{
#if PLATFORM_WINDOWS
	// Verify PKCEData is null before initializing to ensure we're not overriding an active PKCE operation.
	// A non-null value indicates another PKCE operation is already in progress.
	ensureAlways(!PKCEData);
	PKCEData = UImmutablePKCEWindows::Initialise(InitData);
	if (PKCEData)
	{
		PKCEData->DynamicMulticastDelegate_DeepLinkCallback.AddDynamic(this, &ThisClass::OnDeepLinkActivated);
	}
#endif
#if PLATFORM_ANDROID | PLATFORM_IOS | PLATFORM_MAC | PLATFORM_WINDOWS
	if (IsStateFlagsSet(IPS_PKCE))
	{
		PKCELogoutResponseDelegate = ResponseDelegate;
	}
#endif
	if (IsStateFlagsSet(IPS_CONNECTED))
	{
		if (DoHardLogout)
		{
			SetStateFlags(IPS_HARDLOGOUT);
		}
		CallJS(ImmutablePassportAction::Logout, TEXT(""), ResponseDelegate, FImtblJSResponseDelegate::CreateUObject(this, &UImmutablePassport::OnLogoutResponse));
	}
	else
	{
		IMTBL_WARN("Passport is not connected to execute logout.");
	}
}

void UImmutablePassport::ConnectEvm(const FImtblPassportResponseDelegate& ResponseDelegate)
{
	CallJS(ImmutablePassportAction::ConnectEvm, TEXT(""), ResponseDelegate, FImtblJSResponseDelegate::CreateUObject(this, &UImmutablePassport::OnBridgeCallbackResponse));
}

void UImmutablePassport::ZkEvmRequestAccounts(const FImtblPassportResponseDelegate& ResponseDelegate)
{
	CallJS(ImmutablePassportAction::ZkEvmRequestAccounts, TEXT(""), ResponseDelegate, FImtblJSResponseDelegate::CreateUObject(this, &UImmutablePassport::OnBridgeCallbackResponse));
}

void UImmutablePassport::ZkEvmGetBalance(const FImmutablePassportZkEvmGetBalanceData& Data, const FImtblPassportResponseDelegate& ResponseDelegate)
{
	CallJS(ImmutablePassportAction::ZkEvmGetBalance, Data.ToJsonString(), ResponseDelegate, FImtblJSResponseDelegate::CreateUObject(this, &UImmutablePassport::OnBridgeCallbackResponse));
}

void UImmutablePassport::ZkEvmSendTransaction(const FImtblTransactionRequest& Request, const FImtblPassportResponseDelegate& ResponseDelegate)
{
	CallJS(ImmutablePassportAction::ZkEvmSendTransaction, UStructToJsonString(Request), ResponseDelegate, FImtblJSResponseDelegate::CreateUObject(this, &UImmutablePassport::OnBridgeCallbackResponse));
}

void UImmutablePassport::ZkEvmSendTransactionWithConfirmation(const FImtblTransactionRequest& Request,
	const FImtblPassportResponseDelegate& ResponseDelegate)
{
	CallJS(ImmutablePassportAction::zkEvmSendTransactionWithConfirmation, UStructToJsonString(Request), ResponseDelegate, FImtblJSResponseDelegate::CreateUObject(this, &UImmutablePassport::OnBridgeCallbackResponse));
}

void UImmutablePassport::ZkEvmGetTransactionReceipt(const FZkEvmTransactionReceiptRequest& Request, const FImtblPassportResponseDelegate& ResponseDelegate)
{
	CallJS(ImmutablePassportAction::ZkEvmGetTransactionReceipt, UStructToJsonString(Request), ResponseDelegate, FImtblJSResponseDelegate::CreateUObject(this, &UImmutablePassport::OnBridgeCallbackResponse));
}

void UImmutablePassport::ZkEvmSignTypedDataV4(const FString& RequestJsonString, const FImtblPassportResponseDelegate& ResponseDelegate)
{
	CallJS(ImmutablePassportAction::ZkEvmSignTypedDataV4, RequestJsonString, ResponseDelegate, FImtblJSResponseDelegate::CreateUObject(this, &UImmutablePassport::OnBridgeCallbackResponse));
}



void UImmutablePassport::GetIdToken(const FImtblPassportResponseDelegate& ResponseDelegate)
{
	CallJS(ImmutablePassportAction::GetIdToken, TEXT(""), ResponseDelegate, FImtblJSResponseDelegate::CreateUObject(this, &UImmutablePassport::OnBridgeCallbackResponse));
}

void UImmutablePassport::GetAccessToken(const FImtblPassportResponseDelegate& ResponseDelegate)
{
	CallJS(ImmutablePassportAction::GetAccessToken, TEXT(""), ResponseDelegate, FImtblJSResponseDelegate::CreateUObject(this, &UImmutablePassport::OnBridgeCallbackResponse));
}

void UImmutablePassport::GetAddress(const FImtblPassportResponseDelegate& ResponseDelegate)
{
	CallJS(ImmutablePassportAction::GetAddress, TEXT(""), ResponseDelegate, FImtblJSResponseDelegate::CreateUObject(this, &UImmutablePassport::OnBridgeCallbackResponse));
}

void UImmutablePassport::GetEmail(const FImtblPassportResponseDelegate& ResponseDelegate)
{
	CallJS(ImmutablePassportAction::GetEmail, TEXT(""), ResponseDelegate, FImtblJSResponseDelegate::CreateUObject(this, &UImmutablePassport::OnBridgeCallbackResponse));
}

void UImmutablePassport::GetLinkedAddresses(const FImtblPassportResponseDelegate& ResponseDelegate)
{
	CallJS(ImmutablePassportAction::GetLinkedAddresses, TEXT(""), ResponseDelegate, FImtblJSResponseDelegate::CreateUObject(this, &UImmutablePassport::OnBridgeCallbackResponse));
}

void UImmutablePassport::ImxTransfer(const FImxTransferRequest& RequestData, const FImtblPassportResponseDelegate& ResponseDelegate)
{
	IMTBL_LOG("Tranfer Request: %s", *UStructToJsonString(RequestData))
	CallJS(ImmutablePassportAction::ImxTransfer, UStructToJsonString(RequestData), ResponseDelegate, FImtblJSResponseDelegate::CreateUObject(this, &UImmutablePassport::OnBridgeCallbackResponse));
}

void UImmutablePassport::ImxBatchNftTransfer(const FImxBatchNftTransferRequest& RequestData, const FImtblPassportResponseDelegate& ResponseDelegate)
{
	CallJS(ImmutablePassportAction::ImxBatchNftTransfer, RequestData.ToJsonString(), ResponseDelegate, FImtblJSResponseDelegate::CreateUObject(this, &UImmutablePassport::OnBridgeCallbackResponse));
}

void UImmutablePassport::ImxIsRegisteredOffchain(const FImtblPassportResponseDelegate& ResponseDelegate)
{
	CallJS(ImmutablePassportAction::ImxIsRegisteredOffchain, TEXT(""), ResponseDelegate, FImtblJSResponseDelegate::CreateUObject(this, &UImmutablePassport::OnBridgeCallbackResponse));
}

void UImmutablePassport::ImxRegisterOffchain(const FImtblPassportResponseDelegate& ResponseDelegate)
{
	CallJS(ImmutablePassportAction::ImxRegisterOffchain, TEXT(""), ResponseDelegate, FImtblJSResponseDelegate::CreateUObject(this, &UImmutablePassport::OnBridgeCallbackResponse));
}

void UImmutablePassport::HasStoredCredentials(const FImtblPassportResponseDelegate& ResponseDelegate)
{
	// we do check credentials into two steps, we check accessToken and then IdToken
	// check access token
	CallJS(ImmutablePassportAction::GetAccessToken, TEXT(""), ResponseDelegate, FImtblJSResponseDelegate::CreateLambda([this, ResponseDelegate](FImtblJSResponse Response)
	{
		FString AccessToken;

		Response.JsonObject->TryGetStringField(TEXT("result"), AccessToken);
		if (!Response.success || AccessToken.IsEmpty())
		{
			ResponseDelegate.ExecuteIfBound(FImmutablePassportResult{false, Response.JsonObject->HasField(TEXT("error")) ? Response.JsonObject->GetStringField(TEXT("error")) : "Failed to retrieve Access Token.", Response});
		}
		else
		{
			// check for id token
			CallJS(ImmutablePassportAction::GetIdToken, TEXT(""), ResponseDelegate, FImtblJSResponseDelegate::CreateLambda([ResponseDelegate](FImtblJSResponse Response)
			{
				FString IdToken;

				Response.JsonObject->TryGetStringField(TEXT("result"), IdToken);
				if (!Response.success || IdToken.IsEmpty())
				{
					ResponseDelegate.ExecuteIfBound(FImmutablePassportResult{false, Response.JsonObject->HasField(TEXT("error")) ? Response.JsonObject->GetStringField(TEXT("error")) : "Failed to retrieve Id Token.", Response});
				}
				else
				{
					ResponseDelegate.ExecuteIfBound(FImmutablePassportResult{Response.success, "", Response});
				}
			}));
		}
	}));
}

FString UImmutablePassport::GetResponseResultAsString(const FImtblJSResponse& Response)
{
	if (!Response.JsonObject.IsValid())
	{
		IMTBL_ERR("Response JSON data for %s is not valid", *Response.responseFor)
		
		return "";	
	}

	return Response.JsonObject->GetStringField(TEXT("result"));
}

bool UImmutablePassport::GetResponseResultAsBool(const FImtblJSResponse& Response)
{
	if (!Response.JsonObject.IsValid())
	{
		IMTBL_ERR("Response JSON data for %s is not valid", *Response.responseFor)
		
		return false;	
	}

	return Response.JsonObject->GetBoolField(TEXT("result"));
}

TArray<FString> UImmutablePassport::GetResponseResultAsStringArray(const FImtblJSResponse& Response)
{
	if (!Response.JsonObject.IsValid())
	{
		IMTBL_ERR("Response JSON data for %s is not valid", *Response.responseFor)
		
		return TArray<FString>();	
	}
	
	TArray<FString> StringArray;
	
	for (const TSharedPtr<FJsonValue>& Value : Response.JsonObject->GetArrayField(TEXT("result")))
	{
		StringArray.Add(Value->AsString());
	}

	return StringArray;
}


void UImmutablePassport::Setup(const TWeakObjectPtr<UImtblJSConnector> Connector)
{
	IMTBL_LOG_FUNCSIG

	if (!Connector.IsValid())
	{
		IMTBL_ERR("Invalid JSConnector passed to UImmutablePassport::Setup.")
		return;
	}

	JSConnector = Connector.Get();

	// Analytics
	Analytics = NewObject<UImmutableAnalytics>(this);
	Analytics->Setup(Connector);
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

void UImmutablePassport::CallJS(const FString& Action, const FString& Data, const FImtblPassportResponseDelegate& ClientResponseDelegate, const FImtblJSResponseDelegate& HandleJSResponse, const bool bCheckInitialized /*= true*/)
{
	if (bCheckInitialized && !CheckIsInitialized(Action, ClientResponseDelegate)) { return; }

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
	if (TOptional<FImtblPassportResponseDelegate> ResponseDelegate = GetResponseDelegate(Response))
	{
		FString Error;
		
		if (Response.success)
		{
			SetStateFlags(IPS_INITIALIZED);
			IMTBL_LOG("Passport initialization succeeded.")
		}
		else
		{
			IMTBL_ERR("Passport initialization failed.")
			Response.Error.IsSet() ? Error = Response.Error->ToString() : Error = Response.JsonObject->GetStringField(TEXT("error"));
		}
		Analytics->Track(UImmutableAnalytics::EEventName::INIT_PASSPORT, Response.success);
		ResponseDelegate->ExecuteIfBound(FImmutablePassportResult { Response.success, Error, Response });
	}
}



void UImmutablePassport::OnLogoutResponse(FImtblJSResponse Response)
{
	auto ResponseDelegate = GetResponseDelegate(Response);
	
	if (!ResponseDelegate)
	{
		return;
	}

	FString Message;

	if (!Response.success)
	{
		Message = Response.Error.IsSet() ? Response.Error->ToString() : Response.JsonObject->GetStringField(TEXT("error"));

		IMTBL_ERR("%s", *Message)
		ResponseDelegate->ExecuteIfBound(FImmutablePassportResult{ Response.success, Message, Response });

		return;
	}

	if (!IsStateFlagsSet(IPS_HARDLOGOUT))
	{
		IMTBL_LOG("Logged out without clearing browser session")
		ResponseDelegate->ExecuteIfBound(FImmutablePassportResult{ true });
		
		return;
	}

	auto Logout = [this](const FImtblJSResponse& Response)
	{
		TOptional<FImtblPassportResponseDelegate> ResponseDelegate = GetResponseDelegate(Response);

		FString Url;
		Response.JsonObject->TryGetStringField(TEXT("result"), Url);

		FString ErrorMessage;
		FPlatformProcess::LaunchURL(*Url, nullptr, &ErrorMessage);

		if (ErrorMessage.Len())
		{
			ErrorMessage = "Failed to launch browser: " + ErrorMessage;
			IMTBL_ERR("%s", *ErrorMessage);
			ResponseDelegate->ExecuteIfBound(FImmutablePassportResult{false, ErrorMessage, Response});
		}
	};

	ResetStateFlags(IPS_HARDLOGOUT);

	FString Url;
	Response.JsonObject->TryGetStringField(TEXT("result"), Url);

	if (!Url.IsEmpty())
	{
		OnHandleDeepLink.AddUObject(this, &UImmutablePassport::OnDeepLinkActivated);
#if PLATFORM_ANDROID
		LaunchAndroidUrl(Url);
#elif PLATFORM_IOS
		[[ImmutableIOS instance] launchUrl:TCHAR_TO_ANSI(*Url)];
#elif PLATFORM_MAC
		[[ImmutableMac instance] launchUrl:TCHAR_TO_ANSI(*Url) forRedirectUri:TCHAR_TO_ANSI(*InitData.logoutRedirectUri)];
#elif PLATFORM_WINDOWS
		Logout(Response);
#endif
	}
	else
	{
		ResponseDelegate->ExecuteIfBound(FImmutablePassportResult{false, "Logout Url is empty", Response});
	}
	ResetStateFlags(IPS_CONNECTED);
}

#if PLATFORM_ANDROID | PLATFORM_IOS | PLATFORM_MAC | PLATFORM_WINDOWS
void UImmutablePassport::OnGetAuthUrlResponse(FImtblJSResponse Response)
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
			OnHandleDeepLink.AddUObject(this, &UImmutablePassport::OnDeepLinkActivated);

			Msg = Response.JsonObject->GetStringField(TEXT("result")).Replace(TEXT(" "), TEXT("+"));
#if PLATFORM_ANDROID
			OnPKCEDismissed = FImtblPassportOnPKCEDismissedDelegate::CreateUObject(this, &UImmutablePassport::HandleOnLoginDismissed);
			LaunchAndroidUrl(Msg);
#elif PLATFORM_IOS
			[[ImmutableIOS instance] launchUrl:TCHAR_TO_ANSI(*Msg)];
#elif PLATFORM_MAC
			[[ImmutableMac instance] launchUrl:TCHAR_TO_ANSI(*Msg) forRedirectUri:TCHAR_TO_ANSI(*InitData.redirectUri)];
#elif PLATFORM_WINDOWS
			FString ErrorMessage;
			FPlatformProcess::LaunchURL(*Msg, nullptr, &ErrorMessage);
			if (!ErrorMessage.IsEmpty())
			{
				ErrorMessage = "Failed to launch browser: " + ErrorMessage;
				IMTBL_ERR("%s", *ErrorMessage);
				PKCEResponseDelegate.ExecuteIfBound(FImmutablePassportResult{false, ErrorMessage});
				PKCEResponseDelegate.Unbind();
				ResetStateFlags(IPS_PKCE | IPS_CONNECTING);
			}
#endif
		}
	}
	else
	{
		IMTBL_ERR("Unable to return a response for Connect PKCE.");
	}
}

void UImmutablePassport::OnConnectResponse(FImtblJSResponse Response)
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
		Analytics->Track(IsStateFlagsSet(IPS_IMX) ? UImmutableAnalytics::EEventName::COMPLETE_CONNECT_IMX_PKCE : UImmutableAnalytics::EEventName::COMPLETE_LOGIN_PKCE, Response.success);
		PKCEResponseDelegate.ExecuteIfBound(FImmutablePassportResult{Response.success, Msg});
		PKCEResponseDelegate = nullptr;

		// we save passport state for PKCE flow in case if we decide to close a game
		// and reopen it later with Passport is still being connected and we decided to logout.
		// In this case, we logout using PKCE flow
		SavePassportSettings();
	}
	else
	{
		IMTBL_ERR("Unable to return a response for Connect PKCE.");
	}
	ResetStateFlags(IPS_COMPLETING_PKCE);
}



void UImmutablePassport::OnBridgeCallbackResponse(FImtblJSResponse Response)
{
	auto ResponseDelegate = GetResponseDelegate(Response);

	if (!ResponseDelegate)
	{
		IMTBL_ERR("Response delegate is not assigned for %s", *Response.responseFor);
		return;
	}

	if (!Response.success)
	{
		IMTBL_LOG_FUNC("Response for %s is unsuccessfull", *Response.responseFor);
	}

	FString Error;

	Response.JsonObject->TryGetStringField(TEXT("error"), Error);

	ResponseDelegate->ExecuteIfBound(FImmutablePassportResult{ Response.success, Error, Response });
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

	SaveGameInstance->bWasConnectedViaPKCEFlow = IsStateFlagsSet(IPS_PKCE | IPS_CONNECTED);

	UGameplayStatics::SaveGameToSlot(SaveGameInstance, PASSPORT_SAVE_GAME_SLOT_NAME, 0);
}

void UImmutablePassport::LoadPassportSettings()
{
	if (UGameplayStatics::DoesSaveGameExist(PASSPORT_SAVE_GAME_SLOT_NAME, 0))
	{
		if (const UImmutableSaveGame* SaveGameInstance = Cast<UImmutableSaveGame>(UGameplayStatics::LoadGameFromSlot(PASSPORT_SAVE_GAME_SLOT_NAME, 0)))
		{
			SaveGameInstance->bWasConnectedViaPKCEFlow ? SetStateFlags(IPS_PKCE) : ResetStateFlags(IPS_PKCE);
		}
	}
}

void UImmutablePassport::OnDeepLinkActivated(const FString& DeepLink)
{
	OnHandleDeepLink.Clear();
	if (DeepLink.StartsWith(InitData.logoutRedirectUri))
	{
		// execute on game thread to prevent call to Passport instance from another thread
		if (FTaskGraphInterface::IsRunning())
		{
			FGraphEventRef GameThreadTask = FFunctionGraphTask::CreateAndDispatchWhenReady([this]()
			{
				Analytics->Track(UImmutableAnalytics::EEventName::COMPLETE_LOGOUT_PKCE);
				IMTBL_LOG("Complete Logout PKCE")
				PKCELogoutResponseDelegate.ExecuteIfBound(FImmutablePassportResult{true, "Logged out"});
				PKCELogoutResponseDelegate = nullptr;
				ResetStateFlags(IPS_CONNECTED | IPS_PKCE | IPS_IMX);
				SavePassportSettings();
			}, TStatId(), nullptr, ENamedThreads::GameThread);
		}
	}
	else if (DeepLink.StartsWith(InitData.redirectUri))
	{
		CompleteLoginFlow(DeepLink);
	}

	PKCEData = nullptr;
}

void UImmutablePassport::CompleteLoginFlow(FString Url)
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
		ResetStateFlags(IPS_PKCE | IPS_CONNECTING | IPS_COMPLETING_PKCE);
		SavePassportSettings();
	}
	else
	{
		FImmutablePassportConnectData Data = FImmutablePassportConnectData{Code.GetValue(), State.GetValue()};

		CallJS(IsStateFlagsSet(IPS_IMX) ? ImmutablePassportAction::CONNECT_PKCE : ImmutablePassportAction::LOGIN_PKCE, UStructToJsonString(Data), PKCEResponseDelegate,
		       FImtblJSResponseDelegate::CreateUObject(this, &UImmutablePassport::OnConnectResponse));
	}
}
#endif

#if PLATFORM_ANDROID | PLATFORM_WINDOWS
// Called from Android JNI
void UImmutablePassport::HandleDeepLink(FString DeepLink) const
#endif
#if PLATFORM_IOS | PLATFORM_MAC
// Called from iOS Objective C
void UImmutablePassport::HandleDeepLink(NSString* sDeepLink) const
#endif
{
#if PLATFORM_IOS | PLATFORM_MAC
	FString DeepLink = FString(UTF8_TO_TCHAR([sDeepLink UTF8String]));
	IMTBL_LOG("Handle Deep Link: %s", *DeepLink);
#endif
#if PLATFORM_WINDOWS
	if (PKCEData)
	{
		UImmutablePKCEWindows::HandleDeepLink(PKCEData, DeepLink);
	}
#endif

	OnHandleDeepLink.Broadcast(DeepLink);
}

#if PLATFORM_ANDROID
void UImmutablePassport::HandleOnLoginDismissed()
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
		if (FTaskGraphInterface::IsRunning())
		{
			FGraphEventRef GameThreadTask = FFunctionGraphTask::CreateAndDispatchWhenReady([this]()
				{
					if (!PKCEResponseDelegate.ExecuteIfBound(FImmutablePassportResult{ false, "Cancelled" }))
					{
						IMTBL_WARN("Login PKCEResponseDelegate delegate was not called");
					}
					PKCEResponseDelegate = nullptr;
				}, TStatId(), nullptr, ENamedThreads::GameThread);
		}
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
