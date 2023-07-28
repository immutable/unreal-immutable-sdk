// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/ImtblPassportConnectSilentAsyncAction.h"

#include "ImmutablePassport.h"
#include "ImmutableSubsystem.h"
#include "ImtblLogging.h"


UImtblPassportConnectSilentAsyncAction* UImtblPassportConnectSilentAsyncAction::ConnectSilent(UObject* WorldContextObject)
{
    UImtblPassportConnectSilentAsyncAction* PassportInitBlueprintNode = NewObject<UImtblPassportConnectSilentAsyncAction>();
    PassportInitBlueprintNode->WorldContextObject = WorldContextObject;
    return PassportInitBlueprintNode;
}


void UImtblPassportConnectSilentAsyncAction::Activate()
{
    if (!WorldContextObject || !WorldContextObject->GetWorld())
    {
        FString Err = "ConnectSilent failed due to missing world or world context object.";
        IMTBL_WARN("%s", *Err)
        Failed.Broadcast(Err);
        return;
    }

    GetSubsystem()->WhenReady(this, &UImtblPassportConnectSilentAsyncAction::DoConnectSilent);//, /* timoutSec */ 15.0f);
}


void UImtblPassportConnectSilentAsyncAction::DoConnectSilent(TWeakObjectPtr<UImtblJSConnector> JSConnector)
{
    // Get Passport
    auto Passport = GetSubsystem()->GetPassport();
    // Run ConnectSilent
    Passport->ConnectSilent(UImmutablePassport::FImtblPassportResponseDelegate::CreateUObject(this, &UImtblPassportConnectSilentAsyncAction::OnConnectSilentResponse));
}


void UImtblPassportConnectSilentAsyncAction::OnConnectSilentResponse(FImmutablePassportResult Result)
{
    if (Result.Success)
    {
        Connected.Broadcast(Result.Message);
    }
    else
    {
        Failed.Broadcast(Result.Message);
    }
}

