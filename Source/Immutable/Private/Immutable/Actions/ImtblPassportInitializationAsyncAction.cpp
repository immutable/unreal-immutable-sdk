// Fill out your copyright notice in the Description page of Project Settings.

#include "Immutable/Actions/ImtblPassportInitializationAsyncAction.h"

#include "Immutable/ImmutablePassport.h"
#include "Immutable/ImmutableSubsystem.h"
#include "Immutable/Misc/ImtblLogging.h"

UImtblPassportInitializationAsyncAction* UImtblPassportInitializationAsyncAction::InitializePassport(UObject* WorldContextObject, const FString& ClientID, const FString& RedirectUri,
																									 const FString& Environment)
{
	UImtblPassportInitializationAsyncAction* PassportInitBlueprintNode = NewObject<UImtblPassportInitializationAsyncAction>();
	
	PassportInitBlueprintNode->ClientId = ClientID;
	PassportInitBlueprintNode->RedirectUri = RedirectUri;
	PassportInitBlueprintNode->Environment = Environment;
	PassportInitBlueprintNode->WorldContextObject = WorldContextObject;
	
	return PassportInitBlueprintNode;
}

void UImtblPassportInitializationAsyncAction::Activate()
{
	if (!WorldContextObject || !WorldContextObject->GetWorld())
	{
		Failed.Broadcast("Initialization failed due to missing world or world context object.");
		return;
	}

	GetSubsystem()->WhenReady(this, &UImtblPassportInitializationAsyncAction::DoInit); //, /* timoutSec
	//*/ 15.0f);
}

void UImtblPassportInitializationAsyncAction::DoInit(TWeakObjectPtr<UImtblJSConnector> JSConnector)
{
	// Get Passport
	auto Passport = GetSubsystem()->GetPassport();
	// Run Initialize
	Passport->Initialize(FImmutablePassportInitData{ ClientId, RedirectUri, Environment },
						 UImmutablePassport::FImtblPassportResponseDelegate::CreateUObject(this, &UImtblPassportInitializationAsyncAction::OnInitialized));
}

void UImtblPassportInitializationAsyncAction::OnInitialized(FImmutablePassportResult Result)
{
	if (Result.Success)
	{
		Initialized.Broadcast(Result.Message);
	}
	else
	{
		Failed.Broadcast(Result.Message);
	}
}
