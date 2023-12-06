
#include "Immutable/Actions/ImtblPassportReconnectAsyncAction.h"

#include "Immutable/ImmutablePassport.h"
#include "Immutable/ImmutableSubsystem.h"
#include "Immutable/Misc/ImtblLogging.h"


UImtblPassportReconnectAsyncAction * UImtblPassportReconnectAsyncAction::Reconnect(UObject *WorldContextObject)
{
	UImtblPassportReconnectAsyncAction *PassportInitBlueprintNode = NewObject<UImtblPassportReconnectAsyncAction>();

	PassportInitBlueprintNode->WorldContextObject = WorldContextObject;

	return PassportInitBlueprintNode;
}

void UImtblPassportReconnectAsyncAction::Activate()
{
	if (!WorldContextObject || !WorldContextObject->GetWorld())
	{
		FString Err = "Reconnect failed due to missing world or world context object.";
		IMTBL_WARN("%s", *Err)
		OnFailure.Broadcast(Err);
		return;
	}

	GetSubsystem()->WhenReady(this, &UImtblPassportReconnectAsyncAction:: DoReconnect);
}

void UImtblPassportReconnectAsyncAction::DoReconnect(TWeakObjectPtr<UImtblJSConnector> JSConnector)
{
	auto Passport = GetSubsystem()->GetPassport();

	if (Passport.IsValid())
	{
		Passport->Reconnect(UImmutablePassport::FImtblPassportResponseDelegate::CreateUObject(this, &UImtblPassportReconnectAsyncAction::OnReconnectResponse));	
	}
}

void UImtblPassportReconnectAsyncAction::OnReconnectResponse(FImmutablePassportResult Result)
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
