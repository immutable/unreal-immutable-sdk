#pragma once

#include "CoreMinimal.h"
#include "ImtblBlueprintAsyncAction.h"
#include "ImtblPassportImxIsRegisteredOffchainAsyncAction.generated.h"

/**
 * Blueprint node to check if a passport is registered off-chain 
 */
UCLASS()
class IMMUTABLE_API UImtblPassportImxIsRegisteredOffchainAsyncAction : public UImtblBlueprintAsyncAction
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCompleteOutputPin, bool, IsRegistered);

public:
	
    UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", BlueprintInternalUseOnly = "true"), Category = "Immutable")
    static UImtblPassportImxIsRegisteredOffchainAsyncAction* IsRegisteredOffchain(UObject* WorldContextObject);

    virtual void Activate() override;

private:
	
    void DoImxIsRegisteredOffchain(TWeakObjectPtr<class UImtblJSConnector> JSGetConnector);
    void OnImxIsRegisteredOffchainResponse(struct FImmutablePassportResult Result);

    UPROPERTY(BlueprintAssignable)
    FOnCompleteOutputPin OnComplete;

};
