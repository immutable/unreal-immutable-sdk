#pragma once
#include "ImmutableDataTypes.h"

#include "ImmutableRequests.generated.h"


USTRUCT(BlueprintType)
struct FImtblTransactionRequest
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString to;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString data = "0x";

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString value;
};

USTRUCT()
struct FImxTransferRequest
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

USTRUCT()
struct FImxBatchNftTransferRequest
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<FNftTransferDetails> nftTransferDetails;

	FString ToJsonString() const;
};
