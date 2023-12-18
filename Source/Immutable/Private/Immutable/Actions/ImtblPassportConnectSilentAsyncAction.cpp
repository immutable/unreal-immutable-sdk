﻿
#include "Immutable/Actions/ImtblPassportConnectSilentAsyncAction.h"

#include "Immutable/ImmutablePassport.h"
#include "Immutable/ImmutableSubsystem.h"
#include "Immutable/Misc/ImtblLogging.h"


UImtblPassportConnectSilentAsyncAction * UImtblPassportConnectSilentAsyncAction::ConnectSilent(UObject *WorldContextObject)
{
	UImtblPassportConnectSilentAsyncAction *PassportInitBlueprintNode = NewObject<UImtblPassportConnectSilentAsyncAction>();

	PassportInitBlueprintNode->WorldContextObject = WorldContextObject;

	return PassportInitBlueprintNode;
}

void UImtblPassportConnectSilentAsyncAction::Activate()
{
	if (!WorldContextObject || !WorldContextObject->GetWorld())
	{
		FString Err = "Reconnect failed due to missing world or world context object.";
		IMTBL_WARN("%s", *Err)
		OnFailure.Broadcast(Err);
		return;
	}

	GetSubsystem()->WhenReady(this, &UImtblPassportConnectSilentAsyncAction:: DoConnectSilent);
}

void UImtblPassportConnectSilentAsyncAction::DoConnectSilent(TWeakObjectPtr<UImtblJSConnector> JSConnector)
{
	auto Passport = GetSubsystem()->GetPassport();

	if (Passport.IsValid())
	{
		Passport->ConnectSilent(UImmutablePassport::FImtblPassportResponseDelegate::CreateUObject(this, &UImtblPassportConnectSilentAsyncAction::OnConnectSilentResponse));	
	}
}

void UImtblPassportConnectSilentAsyncAction::OnConnectSilentResponse(FImmutablePassportResult Result)
{
    if (Result.Success)
    {
	    IMTBL_LOG("Reconnect success")
	    OnSuccess.Broadcast(Result.Message);
    }
	else
	{
	    IMTBL_LOG("Reconnect failed")
	    OnFailure.Broadcast(Result.Message);
	}
}
