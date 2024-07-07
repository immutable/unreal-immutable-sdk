// Fill out your copyright notice in the Description page of Project Settings.

#include "Immutable/Actions/ImtblPassportGetIdTokenAsyncAction.h"

#include "Immutable/ImmutablePassport.h"
#include "Immutable/ImmutableSubsystem.h"
#include "Immutable/Misc/ImtblLogging.h"

UImtblPassportGetIdTokenAsyncAction* UImtblPassportGetIdTokenAsyncAction::GetIdToken(UObject* WorldContextObject)
{
	UImtblPassportGetIdTokenAsyncAction* PassportInitBlueprintNode = NewObject<UImtblPassportGetIdTokenAsyncAction>();
	PassportInitBlueprintNode->WorldContextObject = WorldContextObject;
	return PassportInitBlueprintNode;
}

void UImtblPassportGetIdTokenAsyncAction::Activate()
{
	if (!WorldContextObject || !WorldContextObject->GetWorld())
	{
		FString Err = "GetIdToken failed due to missing world or world context object.";
		IMTBL_WARN("%s", *Err)
		Failed.Broadcast(Err, TEXT(""));
		return;
	}

	GetSubsystem()->WhenReady(this, &UImtblPassportGetIdTokenAsyncAction::DoGetIdToken);
}

void UImtblPassportGetIdTokenAsyncAction::DoGetIdToken(TWeakObjectPtr<UImtblJSConnector> JSConnector)
{
	// Get Passport
	auto Passport = GetSubsystem()->GetPassport();
	// Run GetIdToken
	Passport->GetIdToken(UImmutablePassport::FImtblPassportResponseDelegate::CreateUObject(this, &UImtblPassportGetIdTokenAsyncAction::OnGetIdTokenResponse));
}

void UImtblPassportGetIdTokenAsyncAction::OnGetIdTokenResponse(FImmutablePassportResult Result)
{
	if (Result.Success)
	{
		GotIdToken.Broadcast(TEXT(""), UImmutablePassport::GetResponseResultAsString(Result.Response));
	}
	else
	{
		Failed.Broadcast(Result.Error, TEXT(""));
	}
}
