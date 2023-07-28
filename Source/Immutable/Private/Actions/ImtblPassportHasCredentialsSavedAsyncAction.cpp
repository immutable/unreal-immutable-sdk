// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/ImtblPassportHasCredentialsSavedAsyncAction.h"

#include "ImmutablePassport.h"
#include "ImmutableSubsystem.h"
#include "ImtblLogging.h"


UImtblPassportHasCredentialsSavedAsyncAction* UImtblPassportHasCredentialsSavedAsyncAction::HasCredentialsSaved(UObject* WorldContextObject)
{
    UImtblPassportHasCredentialsSavedAsyncAction* PassportInitBlueprintNode = NewObject<UImtblPassportHasCredentialsSavedAsyncAction>();
    PassportInitBlueprintNode->WorldContextObject = WorldContextObject;
    return PassportInitBlueprintNode;
}


void UImtblPassportHasCredentialsSavedAsyncAction::Activate()
{
    if (!WorldContextObject || !WorldContextObject->GetWorld())
    {
        FString Err = "HasCredentialsSaved failed due to missing world or world context object.";
        IMTBL_WARN("%s", *Err)
        Failed.Broadcast(Err, TEXT(""));
        return;
    }

    GetSubsystem()->WhenReady(this, &UImtblPassportHasCredentialsSavedAsyncAction::DoHasCredentialsSaved);//, /* timoutSec */ 15.0f);
}


void UImtblPassportHasCredentialsSavedAsyncAction::DoHasCredentialsSaved(TWeakObjectPtr<UImtblJSConnector> JSConnector)
{
    // Get Passport
    auto Passport = GetSubsystem()->GetPassport();
    // Run HasCredentialsSaved
    Passport->CheckStoredCredentials(UImmutablePassport::FImtblPassportResponseDelegate::CreateUObject(this, &UImtblPassportHasCredentialsSavedAsyncAction::OnHasCredentialsSavedResponse));
}


void UImtblPassportHasCredentialsSavedAsyncAction::OnHasCredentialsSavedResponse(FImmutablePassportResult Result)
{
    if (Result.Success)
    {
        HasCredentials.Broadcast(TEXT(""), Result.Message);
    }
    else
    {
        Failed.Broadcast(Result.Message, TEXT(""));
    }
}

