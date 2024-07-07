// Fill out your copyright notice in the Description page of Project Settings.

#include "Immutable/Actions/ImtblPassportZkEvmGetBalanceAsyncAction.h"

#include "Immutable/ImmutablePassport.h"
#include "Immutable/ImmutableSubsystem.h"
#include "Immutable/Misc/ImtblLogging.h"

UImtblPassportZkEvmGetBalanceAsyncAction* UImtblPassportZkEvmGetBalanceAsyncAction::ZkEvmGetBalance(UObject* WorldContextObject, const FString& Address, const FString& BlockNumberOrTag)
{
	UImtblPassportZkEvmGetBalanceAsyncAction* PassportInitBlueprintNode = NewObject<UImtblPassportZkEvmGetBalanceAsyncAction>();
	PassportInitBlueprintNode->WorldContextObject = WorldContextObject;
	PassportInitBlueprintNode->Address = Address;
	PassportInitBlueprintNode->BlockNumberOrTag = BlockNumberOrTag;
	return PassportInitBlueprintNode;
}

void UImtblPassportZkEvmGetBalanceAsyncAction::Activate()
{
	if (!WorldContextObject || !WorldContextObject->GetWorld())
	{
		FString Err = "ZkEvmGetBalance failed due to missing world or world context object.";
		IMTBL_WARN("%s", *Err)
		Failed.Broadcast(Err, TEXT(""));
		return;
	}

	GetSubsystem()->WhenReady(this, &UImtblPassportZkEvmGetBalanceAsyncAction::DoZkEvmGetBalance);
}

void UImtblPassportZkEvmGetBalanceAsyncAction::DoZkEvmGetBalance(TWeakObjectPtr<UImtblJSConnector> JSConnector)
{
	auto Passport = GetSubsystem()->GetPassport();

	if (Passport.IsValid())
	{
		Passport->ZkEvmGetBalance(FImmutablePassportZkEvmGetBalanceData{Address, BlockNumberOrTag}, UImmutablePassport::FImtblPassportResponseDelegate::CreateUObject(this, &UImtblPassportZkEvmGetBalanceAsyncAction::OnZkEvmGetBalanceResponse));
	}
}

void UImtblPassportZkEvmGetBalanceAsyncAction::OnZkEvmGetBalanceResponse(FImmutablePassportResult Result)
{
	if (Result.Success)
	{
		GotBalance.Broadcast(TEXT(""), UImmutablePassport::GetResponseResultAsString(Result.Response));
	}
	else
	{
		Failed.Broadcast(Result.Error, TEXT(""));
	}
}
