// Fill out your copyright notice in the Description page of Project Settings.


#include "Immutable/Actions/ImtblPassportZkEvmSendTransactionAsyncAction.h"

#include "Immutable/ImmutablePassport.h"
#include "Immutable/ImmutableSubsystem.h"
#include "Immutable/Misc/ImtblLogging.h"


UImtblPassportZkEvmSendTransactionAsyncAction* UImtblPassportZkEvmSendTransactionAsyncAction::ZkEvmSendTransaction(UObject* WorldContextObject, const FImtblTransactionRequest& Request)
{
	UImtblPassportZkEvmSendTransactionAsyncAction* PassportInitBlueprintNode = NewObject<UImtblPassportZkEvmSendTransactionAsyncAction>();
	PassportInitBlueprintNode->WorldContextObject = WorldContextObject;
	PassportInitBlueprintNode->TransactionRequest = Request;
	return PassportInitBlueprintNode;
}


void UImtblPassportZkEvmSendTransactionAsyncAction::Activate()
{
	if (!WorldContextObject || !WorldContextObject->GetWorld())
	{
		FString Err = "ZkEvmSendTransaction failed due to missing world or world context object.";
		IMTBL_WARN("%s", *Err)
		Failed.Broadcast(Err, TEXT(""));
		return;
	}

	GetSubsystem()->WhenReady(this, &UImtblPassportZkEvmSendTransactionAsyncAction::DoZkEvmSendTransaction);
}


void UImtblPassportZkEvmSendTransactionAsyncAction::DoZkEvmSendTransaction(TWeakObjectPtr<UImtblJSConnector> JSConnector)
{
	// Get Passport
	auto Passport = GetSubsystem()->GetPassport();
	// Run ZkEvmSendTransaction
	Passport->ZkEvmSendTransaction(TransactionRequest, UImmutablePassport::FImtblPassportResponseDelegate::CreateUObject(this, &UImtblPassportZkEvmSendTransactionAsyncAction::OnZkEvmSendTransactionResponse));
}


void UImtblPassportZkEvmSendTransactionAsyncAction::OnZkEvmSendTransactionResponse(FImmutablePassportResult Result)
{
	if (Result.Success)
	{
		IMTBL_LOG("ZkEvmSendTransaction success")
		TransactionSent.Broadcast(TEXT(""), Result.Message);
	}
	else
	{
		IMTBL_LOG("ZkEvmSendTransaction failed")
		Failed.Broadcast(Result.Message, TEXT(""));
	}
}

