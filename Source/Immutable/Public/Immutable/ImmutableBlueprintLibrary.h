#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "Immutable/ImmutableDataTypes.h"

#include "ImmutableBlueprintLibrary.generated.h"


UCLASS()
class IMMUTABLE_API UImmutableBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintPure, Category = "Immutable", meta = (NativeBreakFunc))
	static void BreakFZkEvmTransactionReceiptLog(FZkEvmTransactionReceiptLog Log, FString& Address, FString& Data, FString& BlockNumber, FString& TransactionHash, FString& TransactionIndex, FString& BlockHash, FString& LogIndex, bool& Removed, TArray<FString>& Topics);

	UFUNCTION(BlueprintPure, Category = "Immutable", meta = (NativeBreakFunc))
	static void BreakZkEvmTransactionReceipt(FZkEvmTransactionReceipt Receipt, FString& BlockHash, FString& BlockNumber, FString& ContractAddress, FString& CumulativeGasUsed, FString& EffectiveGasPrice, FString& From, FString& GasUsed, FString& LogsBloom, FString& Status, FString& To, FString& TransactionHash, FString& TransactionIndex, FString& Type, TArray<FZkEvmTransactionReceiptLog>& Logs);
	
};