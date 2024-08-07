// Fill out your copyright notice in the Description page of Project Settings.

#include "Immutable/Actions/ImtblPassportZkEvmRequestAccountsAsyncAction.h"

#include "Immutable/ImmutablePassport.h"
#include "Immutable/ImmutableSubsystem.h"
#include "Immutable/Misc/ImtblLogging.h"

UImtblPassportZkEvmRequestAccountsAsyncAction* UImtblPassportZkEvmRequestAccountsAsyncAction::RequestAccounts(UObject* WorldContextObject)
{
	UImtblPassportZkEvmRequestAccountsAsyncAction* PassportInitBlueprintNode = NewObject<UImtblPassportZkEvmRequestAccountsAsyncAction>();
	PassportInitBlueprintNode->WorldContextObject = WorldContextObject;
	return PassportInitBlueprintNode;
}

void UImtblPassportZkEvmRequestAccountsAsyncAction::Activate()
{
	if (!WorldContextObject || !WorldContextObject->GetWorld())
	{
		FString Err = "ZkEvmRequestAccounts failed due to missing world or world " "context object.";
		IMTBL_WARN("%s", *Err)
		TArray<FString> StrArr;
		Failed.Broadcast(Err, StrArr);
		return;
	}

	GetSubsystem()->WhenReady(this, &UImtblPassportZkEvmRequestAccountsAsyncAction::DoRequestAccounts);
}

void UImtblPassportZkEvmRequestAccountsAsyncAction::DoRequestAccounts(TWeakObjectPtr<UImtblJSConnector> JSConnector)
{
	auto Passport = GetSubsystem()->GetPassport();

	if (Passport.IsValid())
	{
		Passport->ZkEvmRequestAccounts(UImmutablePassport::FImtblPassportResponseDelegate::CreateUObject(this, &UImtblPassportZkEvmRequestAccountsAsyncAction::OnRequestAccountsResponse));
	}
}

void UImtblPassportZkEvmRequestAccountsAsyncAction::OnRequestAccountsResponse(FImmutablePassportResult Result)
{
	if (Result.Success)
	{
		const auto RequestAccountsData = FImmutablePassportZkEvmRequestAccountsData::FromJsonObject(Result.Response.JsonObject);

		GotAccounts.Broadcast(TEXT(""), RequestAccountsData->accounts);
	}
	else
	{
		Failed.Broadcast(Result.Error, TArray<FString>());
	}
}
