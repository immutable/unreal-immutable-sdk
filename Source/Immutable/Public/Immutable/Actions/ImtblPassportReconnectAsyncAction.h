#pragma once

#include "CoreMinimal.h"
#include "ImtblBlueprintAsyncAction.h"

#include "ImtblPassportReconnectAsyncAction.generated.h"

/**
 *
 */
UCLASS()
class IMMUTABLE_API UImtblPassportReconnectAsyncAction : public UImtblBlueprintAsyncAction
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPassportConnectSilentOutputPin, FString, ErrorMessage);

public:

	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", BlueprintInternalUseOnly = "true"), Category = "Immutable")
	static UImtblPassportReconnectAsyncAction* Reconnect(UObject* WorldContextObject);

	void Activate() override;

private:

	UPROPERTY(BlueprintAssignable)
	FPassportConnectSilentOutputPin OnSuccess;
	UPROPERTY(BlueprintAssignable)
	FPassportConnectSilentOutputPin OnFailure;

	void DoReconnect(TWeakObjectPtr<class UImtblJSConnector> JSGetConnector);
	void OnReconnectResponse(struct FImmutablePassportResult Result);
};
