// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ImtblJSMessages.h"
#include "Immutable/Misc/ImtblLogging.h"
#include "UObject/Object.h"
#include "ImtblJSConnector.generated.h"


DECLARE_DELEGATE_OneParam(FImtblJSResponseDelegate, struct FImtblJSResponse);


/**
 * JSConnector UObject to bind with a browser widget.
 *
 * See the comment on IWebBrowserWindow::BindUObject a description of the binding
 * and FCEFJSScripting to browse the binding implementation code.
 *
 * Every UFUNCTION is exposed to the browser as a JavaScript function.  As none of
 * the base classes in the UObject hierarchy have any UFUNCTIONs we can be sure that
 * the only UFUNCTIONs exposed to the browser are defined here.
 */
UCLASS()
class IMMUTABLE_API UImtblJSConnector : public UObject
{
    GENERATED_BODY()

    friend class UImtblBrowserWidget;
    friend class UImmutableSubsystem;
    friend class UImmutablePassport;

public:
    DECLARE_MULTICAST_DELEGATE(FOnBridgeReadyDelegate)

    UImtblJSConnector();
    
    virtual void PostInitProperties() override;
    
    // The object name used to access the object in JavaScript (i.e.: window.ue.myjsobjectname).  Will be converted to lowercase automatically.
    static FString JSObjectName() { return "JSConnector"; }
    
    bool IsBound() const;
    bool IsBridgeReady() const;

protected:
    void SetBound();
    
    void WhenBridgeReady(const FOnBridgeReadyDelegate::FDelegate& Delegate);
    
    // Call a JavaScript function in the connected browser
    FString CallJS(const FString& Function, const FString& Data, const FImtblJSResponseDelegate& HandleResponse, float ResponseTimeout = 0.0f);
    
    void HandleInitResponse(struct FImtblJSResponse Response);
    
private:
    FOnBridgeReadyDelegate OnBridgeReady;
    TMap<FString, FImtblJSResponseDelegate> RequestResponseDelegates;
    
    bool bIsBound = false;
    bool bIsBridgeReady = false;

    // Callback for JavaScript to send responses back to Unreal
    UFUNCTION()
    void SendToGame(FString Message);

    class UImtblBrowserWidget* GetBrowserWidget() const;

    // Call a JavaScript function in the connected browser
    void CallJS(const FImtblJSRequest& Request, FImtblJSResponseDelegate HandleResponse, float ResponseTimeout = 0.0f);
};

