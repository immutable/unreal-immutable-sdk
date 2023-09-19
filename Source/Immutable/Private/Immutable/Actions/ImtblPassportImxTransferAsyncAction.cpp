// Fill out your copyright notice in the Description page of Project Settings.


#include "Immutable/Actions/ImtblPassportImxTransferAsyncAction.h"

#include "Immutable/ImmutablePassport.h"
#include "Immutable/ImmutableSubsystem.h"
#include "Immutable/Misc/ImtblLogging.h"


UImmutablePassportImxTransferAsyncAction* UImmutablePassportImxTransferAsyncAction::Transfer(UObject* WorldContextObject,
		const FString& Receiver, const FString& Type, const FString& Amount, const FString& TokenId,
		const FString& TokenAddress)
{
	UImmutablePassportImxTransferAsyncAction* BlueprintNode = NewObject<UImmutablePassportImxTransferAsyncAction>();
	BlueprintNode->WorldContextObject = WorldContextObject;
	BlueprintNode->Receiver = Receiver;
	BlueprintNode->Type = Type;
	BlueprintNode->Amount = Amount;
	BlueprintNode->TokenId = TokenId;
	BlueprintNode->TokenAddress = TokenAddress;
	return BlueprintNode;
}


void UImmutablePassportImxTransferAsyncAction::Activate()
{
	if (!WorldContextObject || !WorldContextObject->GetWorld())
	{
		const FString Err = "GetEmail failed due to missing world or world context object.";
		IMTBL_WARN("Error: %s", *Err)
		Failed.Broadcast(Err, TEXT(""));
		return;
	}

	GetSubsystem()->WhenReady(this, &UImmutablePassportImxTransferAsyncAction::DoTransfer);//, /* timoutSec */ 15.0f);
}


void UImmutablePassportImxTransferAsyncAction::DoTransfer(TWeakObjectPtr<UImtblJSConnector> JSConnector)
{
	// Get Passport
	FImxTransferRequest Request;
	Request.receiver = Receiver;
	Request.type = Type;
	Request.amount = Amount;
	Request.tokenId = TokenId;
	Request.tokenAddress = TokenAddress;
	// Run Transfer
	GetSubsystem()->GetPassport()->Transfer(Request, UImmutablePassport::FImtblPassportResponseDelegate::CreateUObject(this, &UImmutablePassportImxTransferAsyncAction::OnTransferResponse));
}


void UImmutablePassportImxTransferAsyncAction::OnTransferResponse(FImmutablePassportResult Result)
{
	if (Result.Success)
	{
		FString TransferId;
		if (auto TransferResponse = JsonObjectToUStruct<FImxTransferResponse>(Result.Response.JsonObject))
		{
			TransferId = FString::Printf(TEXT("%u"), TransferResponse->transferId);
		}
		Success.Broadcast(TEXT(""), TransferId);
	}
	else
	{
		Failed.Broadcast(Result.Message, TEXT(""));
	}
}

