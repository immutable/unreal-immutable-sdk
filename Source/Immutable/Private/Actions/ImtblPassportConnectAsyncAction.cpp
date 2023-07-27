// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/ImtblPassportConnectAsyncAction.h"

#include "ImmutablePassport.h"
#include "ImmutableSubsystem.h"
#include "ImtblLogging.h"


UImtblPassportConnectAsyncAction* UImtblPassportConnectAsyncAction::Connect(UObject* WorldContextObject)
{
    UImtblPassportConnectAsyncAction* PassportInitBlueprintNode = NewObject<UImtblPassportConnectAsyncAction>();
    PassportInitBlueprintNode->WorldContextObject = WorldContextObject;
    return PassportInitBlueprintNode;
}


void UImtblPassportConnectAsyncAction::Activate()
{
    if (!WorldContextObject || !WorldContextObject->GetWorld())
    {
        FString Err = "Connect failed due to missing world or world context object.";
        IMTBL_WARN("%s", *Err)
        Failed.Broadcast(Err, TEXT(""), TEXT(""), TEXT(""));
        return;
    }

    GetSubsystem()->WhenReady(this, &UImtblPassportConnectAsyncAction::DoConnect);//, /* timoutSec */ 15.0f);
}


void UImtblPassportConnectAsyncAction::DoConnect(TWeakObjectPtr<UImtblJSConnector> JSConnector)
{
    // Get Passport
    auto Passport = GetSubsystem()->GetPassport();
    // Run Connect
    Passport->Connect(UImmutablePassport::FImtblPassportResponseDelegate::CreateUObject(this, &UImtblPassportConnectAsyncAction::OnConnectCodeReady));
}


void UImtblPassportConnectAsyncAction::OnConnectCodeReady(FImmutablePassportResult Result)
{
    if (Result.Success)
    {
        if (auto Data = FImmutablePassportConnectData::FromString(Result.Message))
        {
            ConnectData = Data.GetValue();
            
            CodeReady.Broadcast(TEXT(""), ConnectData.code, ConnectData.deviceCode, ConnectData.url);
        }
        else
        {
            IMTBL_WARN("Could not deserialize connect data.")
            Failed.Broadcast(Result.Message, TEXT(""), TEXT(""), TEXT(""));
        }
    }
    else
    {
        Failed.Broadcast(Result.Message, TEXT(""), TEXT(""), TEXT(""));
    }
}

