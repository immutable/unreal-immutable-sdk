#pragma once

#include "Immutable/Actions/ImtblBlueprintAsyncAction.h"
#include "Immutable/Actions/ImtblConnectImxAsyncAction.h"

#include "ImtblEmbeddedLoginAsyncAction.generated.h"

class UImmutableJSConnectorBrowserWidget;

UCLASS()
class IMMUTABLE_API UImtblEmbeddedLoginAsyncAction : public UImtblBlueprintAsyncAction
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Meta = (WorldContext = "WorldContextObject", BlueprintInternalUseOnly = "true"), Category = "Immutable")
	static UImtblEmbeddedLoginAsyncAction* Login(UObject* WorldContextObject, UImmutableJSConnectorBrowserWidget* JSConnectorBrowserWidget);

	virtual void Activate() override;
	virtual void SetReadyToDestroy() override;

	UImtblConnectionAsyncActions* GetLoginAsyncAction() const;

	FImmutableMessageMulticastDelegate* LoginFailed_MulticastDelegate();
	FImmutableMessageDynamicMulticastDelegate* LoginFailed_DynamicMulticastDelegate();

	FSimpleMulticastDelegate* LoginSuccess_MulticastDelegate();
	FImmutableSimpleDynamicMulticastDelegate* LoginSuccess_DynamicMulticastDelegate();

	FSimpleMulticastDelegate* Closed_MulticastDelegate();
	FImmutableSimpleDynamicMulticastDelegate* Closed_DynamicMulticastDelegate();

protected:
	UFUNCTION()
	void LoginAsyncAction_DynamicMulticastDelegate_OnSuccess(FString String);

	UFUNCTION()
	void LoginAsyncAction_DynamicMulticastDelegate_OnFailed(FString String);

protected:
	TWeakObjectPtr<UWorld> BindedWorld;
	TWeakObjectPtr<UImmutableJSConnectorBrowserWidget> BindedJSConnectorBrowserWidget;

	UPROPERTY()
	TObjectPtr<UImtblConnectionAsyncActions> LoginAsyncAction;

	FImmutableMessageMulticastDelegate Internal_LoginFailed_MulticastDelegate;
	UPROPERTY(BlueprintAssignable, Meta = (DisplayName = "LoginFailed"))
	FImmutableMessageDynamicMulticastDelegate Internal_LoginFailed_DynamicMulticastDelegate;

	FSimpleMulticastDelegate Internal_LoginSuccess_MulticastDelegate;
	UPROPERTY(BlueprintAssignable, Meta = (DisplayName = "LoginSuccess"))
	FImmutableSimpleDynamicMulticastDelegate Internal_LoginSuccess_DynamicMulticastDelegate;

	FSimpleMulticastDelegate Internal_Closed_MulticastDelegate;
	UPROPERTY(BlueprintAssignable, Meta = (DisplayName = "Closed"))
	FImmutableSimpleDynamicMulticastDelegate Internal_Closed_DynamicMulticastDelegate;
};