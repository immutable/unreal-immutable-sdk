#pragma once

#include "CoreMinimal.h"
#include "ImtblBlueprintAsyncAction.h"
#include "ImtblPassportImxRegisterOffchainAsyncAction.generated.h"

/**
 * Blueprint node to register passport off-chain 
 */
UCLASS()
class IMMUTABLE_API UImtblPassportImxRegisterOffchainAsyncAction : public UImtblBlueprintAsyncAction
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOutputPin, FString, TxHash, FString, ErrorMessage);

public:
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", BlueprintInternalUseOnly = "true"), Category = "Immutable")
	static UImtblPassportImxRegisterOffchainAsyncAction* RegisterOffchain(UObject* WorldContextObject);

	virtual void Activate() override;

private:
	void DoImxRegisterOffchain(TWeakObjectPtr<class UImtblJSConnector> JSGetConnector);
	void OnImxRegisterOffchainResponse(struct FImmutablePassportResult Result);

	UPROPERTY(BlueprintAssignable)
	FOutputPin OnSuccess;
	UPROPERTY(BlueprintAssignable)
	FOutputPin OnFailure;
};
