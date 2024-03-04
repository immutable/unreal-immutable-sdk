#pragma once

#include "ImmutableResponses.generated.h"


USTRUCT()
struct FImxTransferResponse
{
	GENERATED_BODY()

	UPROPERTY()
	FString sentSignature;

	UPROPERTY()
	FString status;

	UPROPERTY()
	float time = 0.0f;

	UPROPERTY()
	unsigned transferId = 0;
};

USTRUCT()
struct FImxBatchNftTransferResponse
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<int> transferIds;
};

USTRUCT()
struct FImxRegisterOffchainResponse
{
	GENERATED_BODY()

	UPROPERTY()
	FString tx_hash;
};
