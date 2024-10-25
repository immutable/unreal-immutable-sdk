#include "Immutable/Actions/ImtblPassportZkEvmSignTypedDataV4AsyncAction.h"

#include "Immutable/ImmutablePassport.h"
#include "Immutable/ImmutableSubsystem.h"
#include "Immutable/Misc/ImtblLogging.h"


UImtblPassportZkEvmSignTypedDataV4AsyncAction* UImtblPassportZkEvmSignTypedDataV4AsyncAction::ZkEvmSignTypedDataV4(UObject* WorldContextObject, const FString& JsonStringRequest)
{
	UImtblPassportZkEvmSignTypedDataV4AsyncAction* PassportZkEvmSignTypedDataV4BlueprintNode = NewObject<UImtblPassportZkEvmSignTypedDataV4AsyncAction>();
	
	PassportZkEvmSignTypedDataV4BlueprintNode->WorldContextObject = WorldContextObject;
	PassportZkEvmSignTypedDataV4BlueprintNode->JsonStringSignRequest = JsonStringRequest;

	return PassportZkEvmSignTypedDataV4BlueprintNode;
}

void UImtblPassportZkEvmSignTypedDataV4AsyncAction::Activate()
{
	if (!WorldContextObject || !WorldContextObject->GetWorld())
	{
		FString Err = "zkEVM Sign Typed Data V4 failed due to missing world or world " "context object.";
		IMTBL_WARN("%s", *Err)
		Failed.Broadcast(Err, TEXT(""));
		return;
	}

	GetSubsystem()->WhenReady(this, &UImtblPassportZkEvmSignTypedDataV4AsyncAction::DoZkEvmSignTypedDataV4);
}

void UImtblPassportZkEvmSignTypedDataV4AsyncAction::DoZkEvmSignTypedDataV4(TWeakObjectPtr<UImtblJSConnector> JSConnector)
{
	auto Passport = GetSubsystem()->GetPassport();

	if (Passport.IsValid())
	{
		Passport->ZkEvmSignTypedDataV4(JsonStringSignRequest, UImmutablePassport::FImtblPassportResponseDelegate::CreateUObject(this, &UImtblPassportZkEvmSignTypedDataV4AsyncAction::OnZkEvmSignTypedDataV4Response));
	}
}

void UImtblPassportZkEvmSignTypedDataV4AsyncAction::OnZkEvmSignTypedDataV4Response(FImmutablePassportResult Result)
{
	if (Result.Success)
	{
		IMTBL_LOG("zkEVM Sign Typed Data V4 success")
		MessageSigned.Broadcast(TEXT(""), UImmutablePassport::GetResponseResultAsString(Result.Response));
	}
	else
	{
		IMTBL_LOG("zkEVM Sign Typed Data V4 failed")
		Failed.Broadcast(Result.Error, TEXT(""));
	}
}
