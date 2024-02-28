// Fill out your copyright notice in the Description page of Project Settings.

#include "Immutable/Actions/ImtblPassportGetAddressAsyncAction.h"

#include "Immutable/ImmutablePassport.h"
#include "Immutable/ImmutableSubsystem.h"
#include "Immutable/Misc/ImtblLogging.h"

UImtblPassportGetAddressAsyncAction* UImtblPassportGetAddressAsyncAction::GetAddress(UObject* WorldContextObject)
{
	UImtblPassportGetAddressAsyncAction* PassportInitBlueprintNode = NewObject<UImtblPassportGetAddressAsyncAction>();
	PassportInitBlueprintNode->WorldContextObject = WorldContextObject;
	return PassportInitBlueprintNode;
}

void UImtblPassportGetAddressAsyncAction::Activate()
{
	if (!WorldContextObject || !WorldContextObject->GetWorld())
	{
		FString Err = "GetAddress failed due to missing world or world context object.";
		IMTBL_WARN("%s", *Err)
		Failed.Broadcast(Err, TEXT(""));
		return;
	}

	GetSubsystem()->WhenReady(this, &UImtblPassportGetAddressAsyncAction::DoGetAddress); //, /* timoutSec
	//*/ 15.0f);
}

void UImtblPassportGetAddressAsyncAction::DoGetAddress(TWeakObjectPtr<UImtblJSConnector> JSConnector)
{
	// Get Passport
	auto Passport = GetSubsystem()->GetPassport();
	// Run GetAddress
	Passport->GetAddress(UImmutablePassport::FImtblPassportResponseDelegate::CreateUObject(this, &UImtblPassportGetAddressAsyncAction::OnGetAddressResponse));
}

void UImtblPassportGetAddressAsyncAction::OnGetAddressResponse(FImmutablePassportResult Result)
{
	if (Result.Success)
	{
		GotAddress.Broadcast(TEXT(""), Result.Message);
	}
	else
	{
		Failed.Broadcast(Result.Message, TEXT(""));
	}
}
