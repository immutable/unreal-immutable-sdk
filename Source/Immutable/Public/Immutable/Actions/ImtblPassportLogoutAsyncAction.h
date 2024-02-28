#pragma once

#include "CoreMinimal.h"
#include "Immutable/ImmutablePassport.h"
#include "ImtblBlueprintAsyncAction.h"
#include "ImtblPassportLogoutAsyncAction.generated.h"

/**
 *
 */
// UCLASS(meta = (HasDedicatedAsyncNode))
UCLASS()
class IMMUTABLE_API UImtblPassportLogoutAsyncAction : public UImtblBlueprintAsyncAction
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPassportLogoutOutPin, FString, Message);

public:
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", BlueprintInternalUseOnly = "true"), Category = "Immutable")
	static UImtblPassportLogoutAsyncAction* Logout(UObject* WorldContextObject);

	virtual void Activate() override;

private:
	void DoLogout(TWeakObjectPtr<class UImtblJSConnector> JSConnector);
	void OnLogoutResponse(FImmutablePassportResult Result) const;

private:
	UPROPERTY(BlueprintAssignable)
	FPassportLogoutOutPin OnSuccess;
	UPROPERTY(BlueprintAssignable)
	FPassportLogoutOutPin OnFailure;
};
