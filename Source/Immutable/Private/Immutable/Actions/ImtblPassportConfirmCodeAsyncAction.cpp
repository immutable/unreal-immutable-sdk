// Fill out your copyright notice in the Description page of Project Settings.


#include "Immutable/Actions/ImtblPassportConfirmCodeAsyncAction.h"

#include "Immutable/ImmutablePassport.h"
#include "Immutable/ImmutableSubsystem.h"
#include "Immutable/Misc/ImtblLogging.h"


UImtblPassportConfirmCodeAsyncAction* UImtblPassportConfirmCodeAsyncAction::ConfirmCode(UObject* WorldContextObject, const FString& DeviceCode)
{
    UImtblPassportConfirmCodeAsyncAction* BlueprintNode = NewObject<UImtblPassportConfirmCodeAsyncAction>();
    BlueprintNode->WorldContextObject = WorldContextObject;
    BlueprintNode->DeviceCode = DeviceCode;
    return BlueprintNode;
}


void UImtblPassportConfirmCodeAsyncAction::Activate()
{
    if (!WorldContextObject || !WorldContextObject->GetWorld())
    {
        FString Err = "ConfirmCode failed due to missing world or world context object.";
        IMTBL_WARN("%s", *Err)
        Failed.Broadcast(Err);
        return;
    }

    GetSubsystem()->WhenReady(this, &UImtblPassportConfirmCodeAsyncAction::DoConfirmCode);//, /* timoutSec */ 15.0f);
}


void UImtblPassportConfirmCodeAsyncAction::DoConfirmCode(TWeakObjectPtr<UImtblJSConnector> JSConfirmConnector)
{
    // Get Passport
    auto Passport = GetSubsystem()->GetPassport();
    // Run ConfirmCode
    Passport->ConfirmCode(DeviceCode, UImmutablePassport::FImtblPassportResponseDelegate::CreateUObject(this, &UImtblPassportConfirmCodeAsyncAction::OnConfirmCodeCodeResponse));
}


void UImtblPassportConfirmCodeAsyncAction::OnConfirmCodeCodeResponse(FImmutablePassportResult Result)
{
    if (Result.Success)
    {
        CodeConfirmed.Broadcast(TEXT(""));
    }
    else
    {
        Failed.Broadcast(Result.Message);
    }
}


