#include "Immutable/Actions/ImtblEmbeddedLoginAsyncAction.h"

#include "Immutable/Actions/ImtblConnectImxAsyncAction.h"
#include "Immutable/Browser/ImmutableJSConnectorBrowserWidget.h"

UImtblEmbeddedLoginAsyncAction* UImtblEmbeddedLoginAsyncAction::Login(UObject* WorldContextObject, UImmutableJSConnectorBrowserWidget* JSConnectorBrowserWidget)
{
	ThisClass* Action = NewObject<ThisClass>();

	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		Action->BindedWorld = World;
		Action->BindedJSConnectorBrowserWidget = JSConnectorBrowserWidget;
		Action->RegisterWithGameInstance(WorldContextObject);
	}

	return Action;
}

void UImtblEmbeddedLoginAsyncAction::Activate()
{
	Super::Activate();

	if (UWorld* World = BindedWorld.Get())
	{
		if (UImmutableJSConnectorBrowserWidget* JSConnectorBrowserWidget = BindedJSConnectorBrowserWidget.Get())
		{
			JSConnectorBrowserWidget->GetJSConnector()->MulticastDelegate_OnEventToGame()->AddWeakLambda(this, [this, World, JSConnectorBrowserWidget](const FString& Event, const FString& Message, const TOptional<FImtblJSResponse>& Response)
			{
				if (Event == TEXT("HandleLoginData") && Response.IsSet())
				{
					FImmutableDirectLoginOptions DirectLoginOptions;
					if (UImmutableDirectLoginOptionsStatics::FromJSResponse(Response.GetValue(), DirectLoginOptions))
					{
						if ((LoginAsyncAction = UImtblConnectionAsyncActions::Login(World, DirectLoginOptions)))
						{
							LoginAsyncAction->DynamicMulticastDelegate_OnSuccess()->AddDynamic(this, &ThisClass::LoginAsyncAction_DynamicMulticastDelegate_OnSuccess);
							LoginAsyncAction->DynamicMulticastDelegate_OnFailed()->AddDynamic(this, &ThisClass::LoginAsyncAction_DynamicMulticastDelegate_OnFailed);
							LoginAsyncAction->Activate();
						}
						else
						{
							SetReadyToDestroy();
						}
					}
				}
				else if (Event == TEXT("HandleClose"))
				{
					JSConnectorBrowserWidget->LoadURL(TEXT("about:blank"));
					Internal_Closed_MulticastDelegate.Broadcast();
					Internal_Closed_DynamicMulticastDelegate.Broadcast();
					SetReadyToDestroy();
				}
			});
			JSConnectorBrowserWidget->LoadURL(TEXT("https://auth.immutable.com/im-embedded-login-prompt?isWebView=true"));
		}
		else
		{
			SetReadyToDestroy();
		}
	}
	else
	{
		SetReadyToDestroy();
	}
}

void UImtblEmbeddedLoginAsyncAction::SetReadyToDestroy()
{
	if (LoginAsyncAction)
	{
		LoginAsyncAction->DynamicMulticastDelegate_OnSuccess()->RemoveAll(this);
		LoginAsyncAction->DynamicMulticastDelegate_OnFailed()->RemoveAll(this);
		LoginAsyncAction->SetReadyToDestroy();
		LoginAsyncAction = nullptr;
	}
	if (UImmutableJSConnectorBrowserWidget* JSConnectorBrowserWidget = BindedJSConnectorBrowserWidget.Get())
	{
		JSConnectorBrowserWidget->GetJSConnector()->MulticastDelegate_OnEventToGame()->RemoveAll(this);
	}
	Super::SetReadyToDestroy();
}

UImtblConnectionAsyncActions* UImtblEmbeddedLoginAsyncAction::GetLoginAsyncAction() const
{
	return LoginAsyncAction;
}

FImmutableMessageMulticastDelegate* UImtblEmbeddedLoginAsyncAction::LoginFailed_MulticastDelegate()
{
	return &Internal_LoginFailed_MulticastDelegate;
}

FImmutableMessageDynamicMulticastDelegate* UImtblEmbeddedLoginAsyncAction::LoginFailed_DynamicMulticastDelegate()
{
	return &Internal_LoginFailed_DynamicMulticastDelegate;
}

FSimpleMulticastDelegate* UImtblEmbeddedLoginAsyncAction::LoginSuccess_MulticastDelegate()
{
	return &Internal_LoginSuccess_MulticastDelegate;
}

FImmutableSimpleDynamicMulticastDelegate* UImtblEmbeddedLoginAsyncAction::LoginSuccess_DynamicMulticastDelegate()
{
	return &Internal_LoginSuccess_DynamicMulticastDelegate;
}

FSimpleMulticastDelegate* UImtblEmbeddedLoginAsyncAction::Closed_MulticastDelegate()
{
	return &Internal_Closed_MulticastDelegate;
}

FImmutableSimpleDynamicMulticastDelegate* UImtblEmbeddedLoginAsyncAction::Closed_DynamicMulticastDelegate()
{
	return &Internal_Closed_DynamicMulticastDelegate;
}

void UImtblEmbeddedLoginAsyncAction::LoginAsyncAction_DynamicMulticastDelegate_OnSuccess(FString String)
{
	Internal_LoginSuccess_MulticastDelegate.Broadcast();
	Internal_LoginSuccess_DynamicMulticastDelegate.Broadcast();
	SetReadyToDestroy();
}

void UImtblEmbeddedLoginAsyncAction::LoginAsyncAction_DynamicMulticastDelegate_OnFailed(FString String)
{
	Internal_LoginFailed_MulticastDelegate.Broadcast(String);
	Internal_LoginFailed_DynamicMulticastDelegate.Broadcast(String);
	SetReadyToDestroy();
}