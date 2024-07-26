// Fill out your copyright notice in the Description page of Project Settings.

#include "Immutable/ImtblJSConnector.h"

#include "Immutable/Misc/ImtblLogging.h"
#include "ImtblBrowserWidget.h"
#include "Immutable/ImtblJSMessages.h"

void UImtblJSConnector::Init(bool bPageLoaded)
{
	IMTBL_LOG("JSConnect::Init called, bPageloaded %d", bPageLoaded);
	if (bPageLoaded)
	{
		IMTBL_LOG("Browser finished loading the bridge document before the " "Immutable JSConnector was created & bound")
		FImtblJSResponse InitResponse{};
		InitResponse.responseFor = "init";
		InitResponse.requestId = "1";
		InitResponse.success = true;
		HandleInitResponse(InitResponse);
	}
	else
	{
		// Add init function to "waiting for response" queue to handle the init
		// message
		RequestResponseDelegates.Add("1", FImtblJSResponseDelegate::CreateUObject(this, &UImtblJSConnector::HandleInitResponse));
	}
}

void UImtblJSConnector::PostInitProperties() { UObject::PostInitProperties(); }

bool UImtblJSConnector::IsBound() const { return bIsBound; }

bool UImtblJSConnector::IsBridgeReady() const { return bIsBridgeReady; }

void UImtblJSConnector::AddCallbackWhenBridgeReady(const FOnBridgeReadyDelegate::FDelegate& Delegate)
{
	if (IsBridgeReady())
	{
		Delegate.Execute();
	}
	else
	{
		OnBridgeReady.Add(Delegate);
	}
}

FString UImtblJSConnector::CallJS(const FString& Function, const FString& Data, const FImtblJSResponseDelegate& HandleResponse, const float ResponseTimeout)
{
	const FString Guid = FGuid::NewGuid().ToString();
	CallJS(FImtblJSRequest{Function, Data, Guid}, HandleResponse, ResponseTimeout);
	return Guid;
}

void UImtblJSConnector::CallJS(const FImtblJSRequest& Request, FImtblJSResponseDelegate HandleResponse, float ResponseTimeout)
{
	if (!IsBridgeReady())
	{
		// IMTBL_WARN("Attempt to call Immutable JS bridge before bridge ready")
		AddCallbackWhenBridgeReady(FOnBridgeReadyDelegate::FDelegate::CreateLambda([Request, HandleResponse, ResponseTimeout, this]()
		{
			CallJS(Request, HandleResponse, ResponseTimeout);
		}));
		return;
	}

	// First, add response callback to map
	RequestResponseDelegates.Add(Request.requestId, HandleResponse);

	// Convert the request to a string
	FString RequestString = Request.ToJsonString();
	RequestString.ReplaceInline(TEXT("\\"), TEXT("\\\\"));
	RequestString.ReplaceInline(TEXT("\""), TEXT("\\\""));
	const FString JS = FString::Printf(TEXT("callFunction(\"%s\");"), *RequestString);
	// const FString JS = FString::Printf(TEXT("callFunction(%s);"),
	// *RequestString);

	// Execute
	IMTBL_LOG("Executing JS: %s", *JS)
	const bool Result = ExecuteJs.ExecuteIfBound(JS);
	if (!Result)
	{
		IMTBL_WARN("ExecuteJS delegate was not called");
	}

	// TODO: add timeout callback
}

void UImtblJSConnector::HandleInitResponse(FImtblJSResponse Response)
{
	IMTBL_LOG_FUNCSIG
	bIsBridgeReady = true;
	OnBridgeReady.Broadcast();
	OnBridgeReady.Clear();
}

void UImtblJSConnector::SendToGame(FString Message)
{
	IMTBL_LOG_FUNC("Received message from JS: %s", *Message);

	// Parse response

	const TOptional<FImtblJSResponse> Response = FImtblJSResponse::FromJsonString(Message);
	if (!Response.IsSet())
	{
		IMTBL_WARN("Received unexpected response from browser: %s", *Message);
		return;
	}

	// Handle response

	if (!RequestResponseDelegates.Contains(Response->requestId))
	{
		IMTBL_WARN("No delegate found for response with id %s", *Response->requestId);
		return;
	}

	if (!RequestResponseDelegates[Response->requestId].ExecuteIfBound(Response.GetValue()))
	{
		IMTBL_WARN("Delegate for response with id %s failed to execute", *Response->requestId);
	}

	RequestResponseDelegates.Remove(Response->requestId);
}

#if PLATFORM_ANDROID | PLATFORM_IOS
void UImtblJSConnector::SetMobileBridgeReady() {
  IMTBL_LOG_FUNCSIG
  bIsBridgeReady = true;
  OnBridgeReady.Broadcast();
  OnBridgeReady.Clear();
}
#endif
