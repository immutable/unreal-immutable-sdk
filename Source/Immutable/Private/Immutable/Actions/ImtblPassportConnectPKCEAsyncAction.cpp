// Fill out your copyright notice in the Description page of Project Settings.


#include "Immutable/Actions/ImtblPassportConnectPKCEAsyncAction.h"

#include "Immutable/ImmutablePassport.h"
#include "Immutable/ImmutableSubsystem.h"
#include "Immutable/Misc/ImtblLogging.h"


UImtblPassportConnectPKCEAsyncAction* UImtblPassportConnectPKCEAsyncAction::ConnectPKCE(UObject* WorldContextObject)
{
    UImtblPassportConnectPKCEAsyncAction* PassportInitBlueprintNode = NewObject<UImtblPassportConnectPKCEAsyncAction>();
    PassportInitBlueprintNode->WorldContextObject = WorldContextObject;
    return PassportInitBlueprintNode;
}


void UImtblPassportConnectPKCEAsyncAction::Activate()
{
    if (!WorldContextObject || !WorldContextObject->GetWorld())
    {
        FString Err = "Connect failed due to missing world or world context object.";
        IMTBL_WARN("%s", *Err)
        Failed.Broadcast(Err);
        return;
    }

    GetSubsystem()->WhenReady(this, &UImtblPassportConnectPKCEAsyncAction::DoConnectPKCE);
}


void UImtblPassportConnectPKCEAsyncAction::DoConnectPKCE(TWeakObjectPtr<UImtblJSConnector> JSConnector)
{
    // Get Passport
    auto Passport = GetSubsystem()->GetPassport();
#if PLATFORM_ANDROID | PLATFORM_IOS
    // Run Connect
    Passport->ConnectPKCE(UImmutablePassport::FImtblPassportResponseDelegate::CreateUObject(this, &UImtblPassportConnectPKCEAsyncAction::OnConnectPKCE));
#endif
}


void UImtblPassportConnectPKCEAsyncAction::OnConnectPKCE(FImmutablePassportResult Result)
{
    if (Result.Success)
    {
        Success.Broadcast(TEXT(""));
    }
    else
    {
        Failed.Broadcast(Result.Message);
    }
}
