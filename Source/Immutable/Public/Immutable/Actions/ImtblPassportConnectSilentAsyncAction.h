#pragma once

#include "CoreMinimal.h"
#include "ImtblBlueprintAsyncAction.h"

#include "ImtblPassportConnectSilentAsyncAction.generated.h"

/**
 *
 */
UCLASS()
class IMMUTABLE_API UImtblPassportConnectSilentAsyncAction : public UImtblBlueprintAsyncAction
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPassportConnectSilentOutputPin, FString, ErrorMessage);

public:

	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", BlueprintInternalUseOnly = "true"), Category = "Immutable")
	static UImtblPassportConnectSilentAsyncAction* ConnectSilent(UObject* WorldContextObject);

	void Activate() override;

private:

	UPROPERTY(BlueprintAssignable)
	FPassportConnectSilentOutputPin OnSuccess;
	UPROPERTY(BlueprintAssignable)
	FPassportConnectSilentOutputPin OnFailure;

	void DoConnectSilent(TWeakObjectPtr<class UImtblJSConnector> JSGetConnector);
	void OnConnectSilentResponse(struct FImmutablePassportResult Result);
};
