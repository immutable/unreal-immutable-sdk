#include "Immutable/Actions/ImtblPassportImxIsRegisteredOffchainAsyncAction.h"

#include "Immutable/ImmutablePassport.h"
#include "Immutable/ImmutableSubsystem.h"
#include "Immutable/Misc/ImtblLogging.h"


UImtblPassportImxIsRegisteredOffchainAsyncAction* UImtblPassportImxIsRegisteredOffchainAsyncAction::IsRegisteredOffchain(UObject* WorldContextObject)
{
	UImtblPassportImxIsRegisteredOffchainAsyncAction* PassportInitBlueprintNode = NewObject<UImtblPassportImxIsRegisteredOffchainAsyncAction>();

	PassportInitBlueprintNode->SavedWorldContextObject = WorldContextObject;

	return PassportInitBlueprintNode;
}

void UImtblPassportImxIsRegisteredOffchainAsyncAction::Activate()
{
	if (!SavedWorldContextObject || !SavedWorldContextObject->GetWorld())
	{
		FString Err = "IsRegisteredOffchain failed due to missing world or world context object.";
		IMTBL_WARN("%s", *Err)
		OnComplete.Broadcast(false);
		return;
	}

	GetSubsystem()->WhenReady(this, &UImtblPassportImxIsRegisteredOffchainAsyncAction::DoImxIsRegisteredOffchain); //, /* timoutSec */ 15.0f);
}

void UImtblPassportImxIsRegisteredOffchainAsyncAction::DoImxIsRegisteredOffchain(TWeakObjectPtr<UImtblJSConnector> JSConnector)
{
	const auto Passport = GetSubsystem()->GetPassport();

	if (Passport.IsValid())
	{
		Passport->ImxIsRegisteredOffchain(UImmutablePassport::FImtblPassportResponseDelegate::CreateUObject(this, &UImtblPassportImxIsRegisteredOffchainAsyncAction::OnImxIsRegisteredOffchainResponse));
	}
}

void UImtblPassportImxIsRegisteredOffchainAsyncAction::OnImxIsRegisteredOffchainResponse(FImmutablePassportResult Result)
{
	OnComplete.Broadcast(Result.Success);
}
