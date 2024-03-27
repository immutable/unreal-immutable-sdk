// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ImtblJSMessages.h"
#include "UObject/Object.h"

// clang-format off
#include "ImtblJSConnector.generated.h"
// clang-format on

DECLARE_DELEGATE_OneParam(FImtblJSResponseDelegate, struct FImtblJSResponse);

/**
 * JSConnector UObject to bind with a browser widget.
 *
 * See the comment on IWebBrowserWindow::BindUObject a description of the
 * binding and FCEFJSScripting to browse the binding implementation code.
 *
 * Every UFUNCTION is exposed to the browser as a JavaScript function.  As none
 * of the base classes in the UObject hierarchy have any UFUNCTIONs we can be
 * sure that the only UFUNCTIONs exposed to the browser are defined here.
 */
UCLASS()
class IMMUTABLE_API UImtblJSConnector : public UObject
{
	GENERATED_BODY()

	friend class UImmutablePassport;

public:
	DECLARE_MULTICAST_DELEGATE(FOnBridgeReadyDelegate);
	DECLARE_DELEGATE_OneParam(FOnExecuteJsDelegate, const FString &);

	UImtblJSConnector();
	void Init(bool bPageLoaded);

	virtual void PostInitProperties() override;
	bool IsBound() const;

	// The object name used to access the object in JavaScript (i.e.:
	// window.ue.myjsobjectname).  Will be converted to lowercase automatically.
	static FString JSObjectName() { return "JSConnector"; }

	bool IsBridgeReady() const;
	void AddCallbackWhenBridgeReady(const FOnBridgeReadyDelegate::FDelegate& Delegate);

	// Callback for JavaScript to send responses back to Unreal
	UFUNCTION()
	void SendToGame(FString Message);

	// Bind the func to be called for executing JS. Typically by the BrowserWidget
	// (UE5) or Blui for UE4
	FOnExecuteJsDelegate ExecuteJs;

#if PLATFORM_ANDROID | PLATFORM_IOS
  void SetMobileBridgeReady();
#endif

protected:
	// Call a JavaScript function in the connected browser
	FString CallJS(const FString& Function, const FString& Data, const FImtblJSResponseDelegate& HandleResponse, float ResponseTimeout = 0.0f);

private:
	FOnBridgeReadyDelegate OnBridgeReady;
	TMap<FString, FImtblJSResponseDelegate> RequestResponseDelegates;

	bool bIsBound = false;
	bool bIsBridgeReady = false;
	void HandleInitResponse(struct FImtblJSResponse Response);

	// Call a JavaScript function in the connected browser
	void CallJS(const FImtblJSRequest& Request, FImtblJSResponseDelegate HandleResponse, float ResponseTimeout = 0.0f);
};
