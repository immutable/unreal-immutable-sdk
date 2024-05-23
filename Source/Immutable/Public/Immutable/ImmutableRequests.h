#pragma once


#include "ImmutableRequests.generated.h"


USTRUCT(BlueprintType)
struct IMMUTABLE_API FImtblTransactionRequest
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString to;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString data = "0x";

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString value;
};

USTRUCT(BlueprintType)
struct IMMUTABLE_API FImxTransferRequest
{
	GENERATED_BODY()

	UPROPERTY()
	FString receiver;

	UPROPERTY()
	FString type;

	UPROPERTY()
	FString amount;

	UPROPERTY()
	FString tokenId;

	UPROPERTY()
	FString tokenAddress;

	FString ToJsonString() const;
};

USTRUCT(BlueprintType)
struct IMMUTABLE_API FImxBatchNftTransferRequest
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<FNftTransferDetails> nftTransferDetails;

	FString ToJsonString() const;
};

USTRUCT(BlueprintType)
struct IMMUTABLE_API FZkEvmTransactionReceiptRequest
{
	GENERATED_BODY()

	UPROPERTY()
	FString txHash;
};
