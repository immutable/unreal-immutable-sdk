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

	GetSubsystem()->WhenReady(this, &UImtblPassportZkEvmGetBalanceAsyncAction::DoZkEvmGetBalance);//, /* timoutSec */ 15.0f);
}


void UImtblPassportZkEvmGetBalanceAsyncAction::DoZkEvmGetBalance(TWeakObjectPtr<UImtblJSConnector> JSConnector)
{
	// Get Passport
	auto Passport = GetSubsystem()->GetPassport();
	// Run ZkEvmGetBalance
	Passport->ZkEvmGetBalance(FImmutablePassportZkEvmGetBalanceData{Address, BlockNumberOrTag}, UImmutablePassport::FImtblPassportResponseDelegate::CreateUObject(this, &UImtblPassportZkEvmGetBalanceAsyncAction::OnZkEvmGetBalanceResponse));
}


void UImtblPassportZkEvmGetBalanceAsyncAction::OnZkEvmGetBalanceResponse(FImmutablePassportResult Result)
{
	if (Result.Success)
	{
		IMTBL_LOG("ZkEvmGetBalance success")
		GotBalance.Broadcast(TEXT(""), Result.Message);
	}
	else
	{
		IMTBL_LOG("ZkEvmGetBalance failed")
		Failed.Broadcast(Result.Message, TEXT(""));
	}
}

