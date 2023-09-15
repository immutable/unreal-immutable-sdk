// Fill out your copyright notice in the Description page of Project Settings.


#include "Immutable/Actions/ImtblPassportConnectEvmAsyncAction.h"

#include "Immutable/ImmutablePassport.h"
#include "Immutable/ImmutableSubsystem.h"
#include "Immutable/Misc/ImtblLogging.h"


UImtblPassportConnectEvmAsyncAction* UImtblPassportConnectEvmAsyncAction::ConnectEvm(UObject* WorldContextObject)
{
    UImtblPassportConnectEvmAsyncAction* PassportInitBlueprintNode = NewObject<UImtblPassportConnectEvmAsyncAction>();
    PassportInitBlueprintNode->WorldContextObject = WorldContextObject;
    return PassportInitBlueprintNode;
}


void UImtblPassportConnectEvmAsyncAction::Activate()
{
    if (!WorldContextObject || !WorldContextObject->GetWorld())
    {
        FString Err = "ConnectEvm failed due to missing world or world context object.";
        IMTBL_WARN("%s", *Err)
        Failed.Broadcast(Err);
        return;
    }

    GetSubsystem()->WhenReady(this, &UImtblPassportConnectEvmAsyncAction::DoConnectEvm);//, /* timoutSec */ 15.0f);
}


void UImtblPassportConnectEvmAsyncAction::DoConnectEvm(TWeakObjectPtr<UImtblJSConnector> JSConnector)
{
    // Get Passport
    auto Passport = GetSubsystem()->GetPassport();
    // Run ConnectEvm
    Passport->ConnectEvm(UImmutablePassport::FImtblPassportResponseDelegate::CreateUObject(this, &UImtblPassportConnectEvmAsyncAction::OnConnectEvm));
}


void UImtblPassportConnectEvmAsyncAction::OnConnectEvm(FImmutablePassportResult Result)
{
    if (Result.Success)
    {
        Success.Broadcast(Result.Message);
    }
    else
    {
        Failed.Broadcast(Result.Message);
    }
}


