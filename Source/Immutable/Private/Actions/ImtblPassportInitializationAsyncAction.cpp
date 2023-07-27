// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/ImtblPassportInitializationAsyncAction.h"

#include "ImmutablePassport.h"
#include "ImmutableSubsystem.h"
#include "ImtblLogging.h"


UImtblPassportInitializationAsyncAction* UImtblPassportInitializationAsyncAction::InitializePassport(UObject* WorldContextObject, const FString& ClientID)
{
    UImtblPassportInitializationAsyncAction* PassportInitBlueprintNode = NewObject<UImtblPassportInitializationAsyncAction>();
    PassportInitBlueprintNode->ClientId = ClientID;
    PassportInitBlueprintNode->WorldContextObject = WorldContextObject;
    return PassportInitBlueprintNode;
}


void UImtblPassportInitializationAsyncAction::Activate()
{
    if (!WorldContextObject || !WorldContextObject->GetWorld())
    {
        FailedToInitialize.Broadcast("Initialization failed due to missing world or world context object.");
        return;
    }

    GetSubsystem()->WhenReady(this, &UImtblPassportInitializationAsyncAction::DoInit);//, /* timoutSec */ 15.0f);
}


void UImtblPassportInitializationAsyncAction::DoInit(TWeakObjectPtr<UImtblJSConnector> JSConnector)
{
    // Get Passport
    auto Passport = GetSubsystem()->GetPassport();
    // Run Initialize
    Passport->Initialize(ClientId, UImmutablePassport::FImtblPassportResponseDelegate::CreateUObject(this, &UImtblPassportInitializationAsyncAction::OnInitialized));
}


void UImtblPassportInitializationAsyncAction::OnInitialized(FImmutablePassportResult Result)
{
    if (Result.Success)
    {
        // Attempt to connect with saved credentials
        auto Passport = GetSubsystem()->GetPassport();
        Passport->AttemptSilentConnect(UImmutablePassport::FImtblPassportResponseDelegate::CreateUObject(this, &UImtblPassportInitializationAsyncAction::OnSilentConnectResult));
    }
    else
    {
        FailedToInitialize.Broadcast(Result.Message);
    }
}


void UImtblPassportInitializationAsyncAction::OnSilentConnectResult(FImmutablePassportResult Result)
{
    if (Result.Success)
    {
        InitializedAndLoggedIn.Broadcast("");
    }
    else
    {
        Initialized.Broadcast("");
    }
}

