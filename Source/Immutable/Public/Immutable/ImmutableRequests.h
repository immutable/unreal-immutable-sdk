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
	TArray<struct FNftTransferDetails> nftTransferDetails;

	FString ToJsonString() const;
};

USTRUCT(BlueprintType)
struct IMMUTABLE_API FZkEvmTransactionReceiptRequest
{
	GENERATED_BODY()

	UPROPERTY()
	FString txHash;
};

USTRUCT(BlueprintType)
struct IMMUTABLE_API FZkEvmSignTypedDataV4Domain
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString ChainId;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString Name;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString VerifyingContract;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString Version;
};

USTRUCT(BlueprintType)
struct FZkEvmSignTypedDataV4NameType
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString Name;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString Type;
};

USTRUCT(BlueprintType)
struct FZkEvmSignTypedDataV4SignableMessageTypes
{
	GENERATED_BODY()

	// JsonFieldName metadata specifier is used to define the exact name that should be used in the JSON output
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (JsonFieldName = "OrderComponents"))
	TArray<FZkEvmSignTypedDataV4NameType> OrderComponents;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (JsonFieldName = "OfferItem"))
	TArray<FZkEvmSignTypedDataV4NameType> OfferItem;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (JsonFieldName = "ConsiderationItem"))
	TArray<FZkEvmSignTypedDataV4NameType> ConsiderationItem;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (JsonFieldName = "EIP712Domain"))
	TArray<FZkEvmSignTypedDataV4NameType> EIP712Domain;
};

USTRUCT(BlueprintType)
struct FZkEvmSignTypedDataV4OfferItem
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	uint8 ItemType;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString Token;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString IdentifierOrCriteria;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString StartAmount;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString EndAmount;
};

USTRUCT(BlueprintType)
struct FZkEvmSignTypedDataV4ConsiderationItem
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	uint8 ItemType;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString Token;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString IdentifierOrCriteria;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString StartAmount;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString EndAmount;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString Recipient;
};

USTRUCT(BlueprintType)
struct FZkEvmSignTypedDataV4Message
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString Offerer;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString Zone;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FZkEvmSignTypedDataV4OfferItem> Offer;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FZkEvmSignTypedDataV4ConsiderationItem> Consideration;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	uint8 OrderType;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString StartTime;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString EndTime;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString ZoneHash;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString Salt;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString ConduitKey;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString Counter;
};

USTRUCT(BlueprintType)
struct IMMUTABLE_API FZkEvmSignTypedDataV4Request
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FZkEvmSignTypedDataV4Domain domain;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FZkEvmSignTypedDataV4SignableMessageTypes types;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FZkEvmSignTypedDataV4Message message;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString primaryType;
};

// USTRUCT(BlueprintType)
// struct IMMUTABLE_API FZkEvmSignTypedDataV4Request
// {
// 	GENERATED_BODY()
//
// 	UPROPERTY(BlueprintReadWrite, EditAnywhere)
// 	FSignableMessage message;
// };