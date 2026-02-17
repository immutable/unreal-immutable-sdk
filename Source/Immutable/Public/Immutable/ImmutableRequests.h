#pragma once

#include "ImmutableDataTypes.h"

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
struct IMMUTABLE_API FZkEvmTransactionReceiptRequest
{
	GENERATED_BODY()

	UPROPERTY()
	FString txHash;
};
