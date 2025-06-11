// Fill out your copyright notice in the Description page of Project Settings.

#include "Immutable/Actions/ImtblConnectImxAsyncAction.h"

#include "Immutable/ImmutablePassport.h"
#include "Immutable/ImmutableSubsystem.h"
#include "Immutable/Misc/ImtblLogging.h"


UImtblConnectionAsyncActions* UImtblConnectionAsyncActions::Login(UObject* WorldContextObject)
{
	UImtblConnectionAsyncActions* PassportInitBlueprintNode = NewObject<UImtblConnectionAsyncActions>();

	PassportInitBlueprintNode->WorldContextObject = WorldContextObject;
	PassportInitBlueprintNode->bIsConnectImx = false;
	PassportInitBlueprintNode->bIsPKCE = true;

	return PassportInitBlueprintNode;
}

UImtblConnectionAsyncActions* UImtblConnectionAsyncActions::ConnectImx(UObject* WorldContextObject)
{
	UImtblConnectionAsyncActions* PassportInitBlueprintNode = NewObject<UImtblConnectionAsyncActions>();

	PassportInitBlueprintNode->WorldContextObject = WorldContextObject;
	PassportInitBlueprintNode->bIsConnectImx = true;
	PassportInitBlueprintNode->bIsPKCE = true;

	return PassportInitBlueprintNode;
}

void UImtblConnectionAsyncActions::Activate()
{
	if (!WorldContextObject || !WorldContextObject->GetWorld())
	{
		FString Error = "Connect failed due to missing world or world context object.";
		IMTBL_WARN("%s", *Error)
		Failed.Broadcast(Error);

		return;
	}

	GetSubsystem()->WhenReady(this, &UImtblConnectionAsyncActions::DoConnect);
}

void UImtblConnectionAsyncActions::DoConnect(TWeakObjectPtr<UImtblJSConnector> JSConnector)
{
	auto Passport = GetSubsystem()->GetPassport();

	if (Passport.IsValid())
	{
		if (bIsPKCE)
		{
#if PLATFORM_ANDROID | PLATFORM_IOS | PLATFORM_MAC | PLATFORM_WINDOWS
			Passport->Connect(bIsConnectImx, UImmutablePassport::FImtblPassportResponseDelegate::CreateUObject(this, &UImtblConnectionAsyncActions::OnConnect));
#endif
		}
	}
	else
	{
		IMTBL_ERR("Passport was not valid while trying to connect")
	}
}

void UImtblConnectionAsyncActions::OnConnect(FImmutablePassportResult Result)
{
	if (Result.Success)
	{
		Success.Broadcast(TEXT(""));
	}
	else
	{
		Failed.Broadcast(Result.Error);
	}
}
