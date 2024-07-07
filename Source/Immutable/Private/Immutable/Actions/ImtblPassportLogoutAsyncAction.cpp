// Fill out your copyright notice in the Description page of Project Settings.

#include "Immutable/Actions/ImtblPassportLogoutAsyncAction.h"

#include "Immutable/ImmutablePassport.h"
#include "Immutable/ImmutableSubsystem.h"
#include "Immutable/Misc/ImtblLogging.h"

UImtblPassportLogoutAsyncAction* UImtblPassportLogoutAsyncAction::Logout(UObject* WorldContextObject, bool DoHardLogout)
{
	UImtblPassportLogoutAsyncAction* PassportInitBlueprintNode = NewObject<UImtblPassportLogoutAsyncAction>();

	PassportInitBlueprintNode->WorldContextObject = WorldContextObject;
	PassportInitBlueprintNode->bDoHardLogout = DoHardLogout;

	return PassportInitBlueprintNode;
}

void UImtblPassportLogoutAsyncAction::Activate()
{
	if (!WorldContextObject || !WorldContextObject->GetWorld())
	{
		const FString ErrorMessage = "Logout failed due to missing world or world context object.";
		
		IMTBL_WARN("%s", *ErrorMessage)
		OnFailure.Broadcast(ErrorMessage);
		
		return;
	}

	GetSubsystem()->WhenReady(this, &UImtblPassportLogoutAsyncAction::DoLogout);
}

void UImtblPassportLogoutAsyncAction::DoLogout(TWeakObjectPtr<UImtblJSConnector> JSConnector)
{
	auto Passport = GetSubsystem()->GetPassport();
	
	Passport->Logout(bDoHardLogout, UImmutablePassport::FImtblPassportResponseDelegate::CreateUObject(this, &UImtblPassportLogoutAsyncAction::OnLogoutResponse));
}

void UImtblPassportLogoutAsyncAction::OnLogoutResponse(FImmutablePassportResult Result) const
{
	if (Result.Success)
	{
		OnSuccess.Broadcast("");
	}
	else
	{
		OnFailure.Broadcast(Result.Error);
	}
}
