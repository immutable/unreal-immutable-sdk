// Fill out your copyright notice in the Description page of Project Settings.

#include "Immutable/Actions/ImtblPassportZkEvmSendTransactionAsyncAction.h"

#include "Immutable/ImmutablePassport.h"
#include "Immutable/ImmutableSubsystem.h"
#include "Immutable/Misc/ImtblLogging.h"

UImtblPassportZkEvmSendTransactionAsyncAction* UImtblPassportZkEvmSendTransactionAsyncAction::ZkEvmSendTransaction(UObject* WorldContextObject, const FImtblTransactionRequest& Request)
{
	UImtblPassportZkEvmSendTransactionAsyncAction* PassportZkEvmSendTransactionBlueprintNode = NewObject<UImtblPassportZkEvmSendTransactionAsyncAction>();
	PassportZkEvmSendTransactionBlueprintNode->WorldContextObject = WorldContextObject;
	PassportZkEvmSendTransactionBlueprintNode->TransactionRequest = Request;
	return PassportZkEvmSendTransactionBlueprintNode;
}

void UImtblPassportZkEvmSendTransactionAsyncAction::Activate()
{
	if (!WorldContextObject || !WorldContextObject->GetWorld())
	{
		FString Err = "ZkEvmSendTransaction failed due to missing world or world " "context object.";
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

UImtblPassportZkEvmSendTransactionWithConfirmationAA* UImtblPassportZkEvmSendTransactionWithConfirmationAA::ZkEvmSendTransactionWithConfirmation(
	UObject* WorldContextObject, const FImtblTransactionRequest& Request)
{
	UImtblPassportZkEvmSendTransactionWithConfirmationAA* ZkEvmSendTransactionWithConfirmationBPNode = NewObject<UImtblPassportZkEvmSendTransactionWithConfirmationAA>();
	
	ZkEvmSendTransactionWithConfirmationBPNode->WorldContextObject = WorldContextObject;
	ZkEvmSendTransactionWithConfirmationBPNode->TransactionRequest = Request;
	
	return ZkEvmSendTransactionWithConfirmationBPNode;
}

void UImtblPassportZkEvmSendTransactionWithConfirmationAA::Activate()
{
	if (!WorldContextObject || !WorldContextObject->GetWorld())
	{
		FString Err = "zkEVM Send Transaction with confirmation failed due to missing world context object.";

		IMTBL_WARN("%s", *Err)
		Failed.Broadcast(Err, FZkEvmTransactionReceipt());
		
		return;
	}

	GetSubsystem()->WhenReady(this, &UImtblPassportZkEvmSendTransactionWithConfirmationAA::DoZkEvmSendTransactionWithConfirmation);
}

void UImtblPassportZkEvmSendTransactionWithConfirmationAA::DoZkEvmSendTransactionWithConfirmation(TWeakObjectPtr<UImtblJSConnector> JSConnector)
{
	auto Passport = GetSubsystem()->GetPassport();

	if (Passport.IsValid())
	{
		Passport->ZkEvmSendTransactionWithConfirmation(TransactionRequest, UImmutablePassport::FImtblPassportResponseDelegate::CreateUObject(this, &UImtblPassportZkEvmSendTransactionWithConfirmationAA::OnZkEvmSendTransactionWithConfirmationResponse));
	}
}

void UImtblPassportZkEvmSendTransactionWithConfirmationAA::OnZkEvmSendTransactionWithConfirmationResponse(FImmutablePassportResult Result)
{
	if (Result.Success)
	{
		auto Receipt = JsonObjectToUStruct<FZkEvmTransactionReceipt>(Result.Response.JsonObject);
		
		if (Receipt.IsSet())
		{
			IMTBL_LOG("zkEVM Send Transaction with confirmation receipt retrival is sucessful")
			Success.Broadcast(TEXT(""), Receipt.GetValue());	
		}
		else
		{
			IMTBL_LOG("zkEVM Send Transaction with confirmation receipt is not provided")
			Success.Broadcast(TEXT(""), FZkEvmTransactionReceipt());
		}
	}
	else
	{
		IMTBL_LOG("zkEVM Send Transaction with confirmation failed")
		Failed.Broadcast(Result.Message, FZkEvmTransactionReceipt());
	}
}
