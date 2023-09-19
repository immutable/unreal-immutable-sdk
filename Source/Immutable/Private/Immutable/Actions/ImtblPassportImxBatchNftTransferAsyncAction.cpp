// Fill out your copyright notice in the Description page of Project Settings.


#include "Immutable/Actions/ImtblPassportImxBatchNftTransferAsyncAction.h"

#include "Immutable/ImmutablePassport.h"
#include "Immutable/ImmutableSubsystem.h"
#include "Immutable/Misc/ImtblLogging.h"


UImmutablePassportImxBatchNftTransferAsyncAction* UImmutablePassportImxBatchNftTransferAsyncAction::BatchNftTransfer(UObject* WorldContextObject,
	const TArray<FNftTransferDetails>& NftTransferDetails)
{
	UImmutablePassportImxBatchNftTransferAsyncAction* BlueprintNode = NewObject<UImmutablePassportImxBatchNftTransferAsyncAction>();
	BlueprintNode->WorldContextObject = WorldContextObject;
	BlueprintNode->NftTransferDetails = NftTransferDetails;
	return BlueprintNode;
}


void UImmutablePassportImxBatchNftTransferAsyncAction::Activate()
{
	if (!WorldContextObject || !WorldContextObject->GetWorld())
	{
		const FString Err = "GetEmail failed due to missing world or world context object.";
		IMTBL_WARN("Error: %s", *Err)
		TArray<FString> TransferIds;
		Failed.Broadcast(Err, TransferIds);
		return;
	}

	GetSubsystem()->WhenReady(this, &UImmutablePassportImxBatchNftTransferAsyncAction::DoTransfer);//, /* timoutSec */ 15.0f);
}


void UImmutablePassportImxBatchNftTransferAsyncAction::DoTransfer(TWeakObjectPtr<UImtblJSConnector> JSConnector)
{
	// Get Passport
	FImxBatchNftTransferRequest Request;
	Request.nftTransferDetails = NftTransferDetails;
	
	// Run Transfer
	GetSubsystem()->GetPassport()->BatchNftTransfer(Request, UImmutablePassport::FImtblPassportResponseDelegate::CreateUObject(this, &UImmutablePassportImxBatchNftTransferAsyncAction::OnTransferResponse));
}


void UImmutablePassportImxBatchNftTransferAsyncAction::OnTransferResponse(FImmutablePassportResult Result)
{
	TArray<FString> TransferIds;
	if (Result.Success)
	{
		if (auto BatchNftTransferResponse = JsonObjectToUStruct<FImxBatchNftTransferResponse>(Result.Response.JsonObject))
		{
			for (auto Id : BatchNftTransferResponse->transferIds)
			{
				TransferIds.Add(FString::Printf(TEXT("%u,"), Id));	
			}
		}
		Success.Broadcast(TEXT(""), TransferIds);
	}
	else
	{
		Failed.Broadcast(Result.Message, TransferIds);
	}
}

