// Fill out your copyright notice in the Description page of Project Settings.

#include "Immutable/Actions/ImtblConnectImxAsyncAction.h"

#include "Immutable/ImmutablePassport.h"
#include "Immutable/ImmutableSubsystem.h"
#include "Immutable/Misc/ImtblLogging.h"


UImtblConnectionAsyncActions* UImtblConnectionAsyncActions::Login(UObject* WorldContextObject, bool Relogin, bool ConnectImx)
{
	UImtblConnectionAsyncActions* PassportInitBlueprintNode = NewObject<UImtblConnectionAsyncActions>();

	PassportInitBlueprintNode->WorldContextObject = WorldContextObject;
	PassportInitBlueprintNode->bIsRelogin = Relogin;
	PassportInitBlueprintNode->bIsConnectImx = ConnectImx;

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
		Passport->Connect(bIsConnectImx, bIsRelogin, UImmutablePassport::FImtblPassportResponseDelegate::CreateUObject(this, &UImtblConnectionAsyncActions::OnConnect));
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
		Failed.Broadcast(Result.Message);
	}
}
