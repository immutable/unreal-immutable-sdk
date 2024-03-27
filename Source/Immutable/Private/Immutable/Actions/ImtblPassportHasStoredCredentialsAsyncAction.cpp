#include "Immutable/Actions/ImtblPassportHasStoredCredentialsAsyncAction.h"

#include "Immutable/ImmutablePassport.h"
#include "Immutable/ImmutableSubsystem.h"
#include "Immutable/Misc/ImtblLogging.h"


UImtblPassportHasStoredCredentialsAsyncAction* UImtblPassportHasStoredCredentialsAsyncAction::HasStoredCredentials(UObject* WorldContextObject)
{
	UImtblPassportHasStoredCredentialsAsyncAction* PassportInitBlueprintNode = NewObject<UImtblPassportHasStoredCredentialsAsyncAction>();

	PassportInitBlueprintNode->SavedWorldContextObject = WorldContextObject;

	return PassportInitBlueprintNode;
}

void UImtblPassportHasStoredCredentialsAsyncAction::Activate()
{
	if (!SavedWorldContextObject || !SavedWorldContextObject->GetWorld())
	{
		FString Err = "HasStoredCredentials failed due to missing world or world context object.";
		IMTBL_WARN("%s", *Err)
		OnFalse.Broadcast(Err);
		return;
	}

	GetSubsystem()->WhenReady(this, &UImtblPassportHasStoredCredentialsAsyncAction::DoHasStoredCredentials);
}

void UImtblPassportHasStoredCredentialsAsyncAction::DoHasStoredCredentials(TWeakObjectPtr<UImtblJSConnector> JSConnector)
{
	const auto Passport = GetSubsystem()->GetPassport();

	if (Passport.IsValid())
	{
		Passport->HasStoredCredentials(UImmutablePassport::FImtblPassportResponseDelegate::CreateUObject(this, &UImtblPassportHasStoredCredentialsAsyncAction::OnHasStoredCredentialsResponse));
	}
}

void UImtblPassportHasStoredCredentialsAsyncAction::OnHasStoredCredentialsResponse(FImmutablePassportResult Result)
{
	if (Result.Success)
	{
		OnTrue.Broadcast(TEXT(""));
	}
	else
	{
		OnFalse.Broadcast(Result.Message);
	}
}
