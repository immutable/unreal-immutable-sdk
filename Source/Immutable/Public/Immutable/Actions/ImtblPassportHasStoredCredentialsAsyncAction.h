#pragma once

#include "CoreMinimal.h"
#include "ImtblBlueprintAsyncAction.h"

#include "ImtblPassportHasStoredCredentialsAsyncAction.generated.h"

/**
 *
 */
UCLASS()
class IMMUTABLE_API UImtblPassportHasStoredCredentialsAsyncAction : public UImtblBlueprintAsyncAction
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPassportHasStoredCredentialsOutputPin, FString, ErrorMessage);

public:
	
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", BlueprintInternalUseOnly = "true"), Category = "Immutable")
	static UImtblPassportHasStoredCredentialsAsyncAction* HasStoredCredentials(UObject* WorldContextObject);

	void Activate() override;

private:
	
	void DoHasStoredCredentials(TWeakObjectPtr<class UImtblJSConnector> JSGetConnector);
	void OnHasStoredCredentialsResponse(struct FImmutablePassportResult Result);

	UPROPERTY(BlueprintAssignable, meta = (DisplayName = "True"))
	FPassportHasStoredCredentialsOutputPin OnTrue;
	UPROPERTY(BlueprintAssignable, meta = (DisplayName = "False"))
	FPassportHasStoredCredentialsOutputPin OnFalse;
};
