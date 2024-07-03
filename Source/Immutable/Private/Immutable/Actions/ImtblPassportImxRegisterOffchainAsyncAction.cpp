#include "Immutable/Actions/ImtblPassportImxRegisterOffchainAsyncAction.h"

#include "Immutable/ImmutablePassport.h"
#include "Immutable/ImmutableSubsystem.h"
#include "Immutable/ImmutableResponses.h"
#include "Immutable/Misc/ImtblLogging.h"


UImtblPassportImxRegisterOffchainAsyncAction* UImtblPassportImxRegisterOffchainAsyncAction::RegisterOffchain(UObject* WorldContextObject)
{
	UImtblPassportImxRegisterOffchainAsyncAction* PassportInitBlueprintNode = NewObject<UImtblPassportImxRegisterOffchainAsyncAction>();

	PassportInitBlueprintNode->WorldContextObject = WorldContextObject;

	return PassportInitBlueprintNode;
}

void UImtblPassportImxRegisterOffchainAsyncAction::Activate()
{
	if (!WorldContextObject || !WorldContextObject->GetWorld())
	{
		FString Err = "RegisterOffchain failed due to missing world or world context object.";
		IMTBL_WARN("%s", *Err)
		OnFailure.Broadcast(TEXT(""), Err);
		return;
	}

	GetSubsystem()->WhenReady(this, &UImtblPassportImxRegisterOffchainAsyncAction::DoImxRegisterOffchain);
}

void UImtblPassportImxRegisterOffchainAsyncAction::DoImxRegisterOffchain(TWeakObjectPtr<UImtblJSConnector> JSConnector)
{
	const auto Passport = GetSubsystem()->GetPassport();

	if (Passport.IsValid())
	{
		Passport->ImxRegisterOffchain(UImmutablePassport::FImtblPassportResponseDelegate::CreateUObject(this, &UImtblPassportImxRegisterOffchainAsyncAction::OnImxRegisterOffchainResponse));
	}
}

void UImtblPassportImxRegisterOffchainAsyncAction::OnImxRegisterOffchainResponse(FImmutablePassportResult Result)
{
	if (Result.Success)
	{
		if (auto ResponseData = JsonObjectToUStruct<FImxRegisterOffchainResponse>(Result.Response.JsonObject))
		{
			OnSuccess.Broadcast(ResponseData->tx_hash, TEXT(""));
			return;
		}
	}

	OnFailure.Broadcast(TEXT(""), Result.Error);
}
