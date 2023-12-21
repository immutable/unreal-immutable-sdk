// Fill out your copyright notice in the Description page of Project Settings.

#include "Immutable/Actions/ImtblPassportLogoutAsyncAction.h"

#include "Immutable/ImmutablePassport.h"
#include "Immutable/ImmutableSubsystem.h"
#include "Immutable/Misc/ImtblLogging.h"

UImtblPassportLogoutAsyncAction* UImtblPassportLogoutAsyncAction::Logout(UObject* WorldContextObject)
{
	UImtblPassportLogoutAsyncAction* PassportInitBlueprintNode = NewObject<UImtblPassportLogoutAsyncAction>();

	PassportInitBlueprintNode->WorldContextObject = WorldContextObject;

	return PassportInitBlueprintNode;
}

void UImtblPassportLogoutAsyncAction::Activate()
{
	if (!WorldContextObject || !WorldContextObject->GetWorld())
	{
		FString Err = "Logout failed due to missing world or world context object.";
		IMTBL_WARN("%s", *Err)
		Failed.Broadcast(Err);
		return;
	}

	GetSubsystem()->WhenReady(this, &UImtblPassportLogoutAsyncAction::DoLogout); //, /* timoutSec */ 15.0f);
}

void UImtblPassportLogoutAsyncAction::DoLogout(TWeakObjectPtr<UImtblJSConnector> JSConnector)
{
	// Get Passport
	auto Passport = GetSubsystem()->GetPassport();
	// Run Logout
	Passport->Logout(UImmutablePassport::FImtblPassportResponseDelegate::CreateUObject(this, &UImtblPassportLogoutAsyncAction::OnLogoutResponse));
}

void UImtblPassportLogoutAsyncAction::OnLogoutResponse(FImmutablePassportResult Result)
{
	if (Result.Success)
	{
		LoggedOut.Broadcast(Result.Message);
	}
	else
	{
		Failed.Broadcast(Result.Message);
	}
}
