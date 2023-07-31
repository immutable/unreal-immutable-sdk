// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/ImtblPassportHasStoredCredentialsAsyncAction.h"

#include "ImmutablePassport.h"
#include "ImmutableSubsystem.h"
#include "ImtblLogging.h"


UImtblPassportHasStoredCredentialsAsyncAction* UImtblPassportHasStoredCredentialsAsyncAction::HasStoredCredentials(UObject* WorldContextObject)
{
    UImtblPassportHasStoredCredentialsAsyncAction* PassportInitBlueprintNode = NewObject<UImtblPassportHasStoredCredentialsAsyncAction>();
    PassportInitBlueprintNode->WorldContextObject = WorldContextObject;
    return PassportInitBlueprintNode;
}


void UImtblPassportHasStoredCredentialsAsyncAction::Activate()
{
    if (!WorldContextObject || !WorldContextObject->GetWorld())
    {
        FString Err = "HasStoredCredentials failed due to missing world or world context object.";
        IMTBL_WARN("%s", *Err)
        NotFound.Broadcast(Err);
        return;
    }

    GetSubsystem()->WhenReady(this, &UImtblPassportHasStoredCredentialsAsyncAction::DoHasStoredCredentials);//, /* timoutSec */ 15.0f);
}


void UImtblPassportHasStoredCredentialsAsyncAction::DoHasStoredCredentials(TWeakObjectPtr<UImtblJSConnector> JSConnector)
{
    // Get Passport
    auto Passport = GetSubsystem()->GetPassport();
    // Run HasCredentialsSaved
    Passport->CheckStoredCredentials(UImmutablePassport::FImtblPassportResponseDelegate::CreateUObject(this, &UImtblPassportHasStoredCredentialsAsyncAction::OnHasStoredCredentialsResponse));
}


void UImtblPassportHasStoredCredentialsAsyncAction::OnHasStoredCredentialsResponse(FImmutablePassportResult Result)
{
    if (Result.Success)
    {
        HasCredentials.Broadcast(TEXT(""));
    }
    else
    {
        NotFound.Broadcast(Result.Message);
    }
}

