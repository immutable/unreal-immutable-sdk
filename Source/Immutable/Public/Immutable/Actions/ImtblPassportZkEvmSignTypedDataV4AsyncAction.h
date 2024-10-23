#pragma once


#include "Immutable/ImmutablePassport.h"
#include "ImtblBlueprintAsyncAction.h"

#include "ImtblPassportZkEvmSignTypedDataV4AsyncAction.generated.h"

/**
 * Async action blueprint node for zkEVM SignTypedDataV4
 */
UCLASS()
class IMMUTABLE_API UImtblPassportZkEvmSignTypedDataV4AsyncAction : public UImtblBlueprintAsyncAction
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FPassportZkEvmSignTypedDataV4OutputPin, FString, ErrorMessage, FString, Signature);

public:
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", BlueprintInternalUseOnly = "true"), Category = "Immutable")
	static UImtblPassportZkEvmSignTypedDataV4AsyncAction* ZkEvmSignTypedDataV4(UObject* WorldContextObject, const FString& JsonStringRequest);

	virtual void Activate() override;

private:
	FString JsonStringSignRequest;

	UPROPERTY(BlueprintAssignable)
	FPassportZkEvmSignTypedDataV4OutputPin MessageSigned;
	UPROPERTY(BlueprintAssignable)
	FPassportZkEvmSignTypedDataV4OutputPin Failed;

	void DoZkEvmSignTypedDataV4(TWeakObjectPtr<class UImtblJSConnector> JSGetConnector);
	void OnZkEvmSignTypedDataV4Response(FImmutablePassportResult Result);
};
