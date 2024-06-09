// Fill out your copyright notice in the Description page of Project Settings.

#include "Immutable/Actions/ImtblPassportZkEvmGetTransactionReceiptAA.h"

#include "Immutable/ImmutablePassport.h"
#include "Immutable/ImmutableSubsystem.h"
#include "Immutable/Misc/ImtblLogging.h"

UImtblPassportZkEvmGetTransactionReceiptAA* UImtblPassportZkEvmGetTransactionReceiptAA::ZkEvmGetTransactionReceipt(UObject* WorldContextObject, const FString& Hash)
{
	UImtblPassportZkEvmGetTransactionReceiptAA* PassportZkEvmSendTransactionBlueprintNode = NewObject<UImtblPassportZkEvmGetTransactionReceiptAA>();
	
	PassportZkEvmSendTransactionBlueprintNode->WorldContextObject = WorldContextObject;
	PassportZkEvmSendTransactionBlueprintNode->Hash = Hash;
	
	return PassportZkEvmSendTransactionBlueprintNode;
}

void UImtblPassportZkEvmGetTransactionReceiptAA::Activate()
{
	if (!WorldContextObject || !WorldContextObject->GetWorld())
	{
		const FString ErrorMessage = "zkEVM Transaction Receipt failed due to missing world context object.";
		IMTBL_WARN("%s", *ErrorMessage)
		Failed.Broadcast(ErrorMessage, FZkEvmTransactionReceipt());
		return;
	}

	GetSubsystem()->WhenReady(this, &UImtblPassportZkEvmGetTransactionReceiptAA::DoZkEvmGetTransactionReceipt);
}

void UImtblPassportZkEvmGetTransactionReceiptAA::DoZkEvmGetTransactionReceipt(TWeakObjectPtr<UImtblJSConnector> JSConnector)
{
	auto Passport = GetSubsystem()->GetPassport();

	if (Passport.IsValid())
	{
		Passport->ZkEvmGetTransactionReceipt({ Hash }, UImmutablePassport::FImtblPassportResponseDelegate::CreateUObject(this, &UImtblPassportZkEvmGetTransactionReceiptAA::OnZkEvmGetTransactionReceiptResponse));	
	}
}

void UImtblPassportZkEvmGetTransactionReceiptAA::OnZkEvmGetTransactionReceiptResponse(FImmutablePassportResult Result)
{
	if (Result.Success)
	{
		auto Receipt = JsonObjectToUStruct<FZkEvmTransactionReceipt>(Result.Response.JsonObject);
		
		if (Receipt.IsSet())
		{
			IMTBL_LOG("zkEVM Transaction Receipt retrival is sucessful")
			Success.Broadcast(TEXT(""), Receipt.GetValue());	
		}
		else
		{
			IMTBL_LOG("zkEVM Transaction Receipt is not provided")
			Success.Broadcast(TEXT(""), FZkEvmTransactionReceipt());
		}
	}
	else
	{
		IMTBL_LOG("zkEVM Transaction Receipt retrival failed")
		Failed.Broadcast(Result.Message, FZkEvmTransactionReceipt());
	}
}
