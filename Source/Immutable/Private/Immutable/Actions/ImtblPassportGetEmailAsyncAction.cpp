// Fill out your copyright notice in the Description page of Project Settings.

#include "Immutable/Actions/ImtblPassportGetEmailAsyncAction.h"

#include "Immutable/ImmutablePassport.h"
#include "Immutable/ImmutableSubsystem.h"
#include "Immutable/Misc/ImtblLogging.h"

UImtblPassportGetEmailAsyncAction* UImtblPassportGetEmailAsyncAction::GetEmail(UObject* WorldContextObject)
{
	UImtblPassportGetEmailAsyncAction* PassportInitBlueprintNode = NewObject<UImtblPassportGetEmailAsyncAction>();
	PassportInitBlueprintNode->WorldContextObject = WorldContextObject;
	return PassportInitBlueprintNode;
}

void UImtblPassportGetEmailAsyncAction::Activate()
{
	if (!WorldContextObject || !WorldContextObject->GetWorld())
	{
		FString Err = "GetEmail failed due to missing world or world context object.";
		IMTBL_WARN("%s", *Err)
		Failed.Broadcast(Err, TEXT(""));
		return;
	}

	GetSubsystem()->WhenReady(this, &UImtblPassportGetEmailAsyncAction::DoGetEmail); //, /* timoutSec
	//*/ 15.0f);
}

void UImtblPassportGetEmailAsyncAction::DoGetEmail(TWeakObjectPtr<UImtblJSConnector> JSConnector)
{
	// Get Passport
	auto Passport = GetSubsystem()->GetPassport();
	// Run GetEmail
	Passport->GetEmail(UImmutablePassport::FImtblPassportResponseDelegate::CreateUObject(this, &UImtblPassportGetEmailAsyncAction::OnGetEmailResponse));
}

void UImtblPassportGetEmailAsyncAction::OnGetEmailResponse(FImmutablePassportResult Result)
{
	if (Result.Success)
	{
		GotEmail.Broadcast(TEXT(""), UImmutablePassport::GetResponseResultAsString(Result.Response));
	}
	else
	{
		Failed.Broadcast(Result.Error, TEXT(""));
	}
}
