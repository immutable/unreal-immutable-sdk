#include "Immutable/Actions/ImtblConnectionAsyncAction.h"

#include "Immutable/ImmutablePassport.h"
#include "Immutable/ImmutableSubsystem.h"
#include "Immutable/Misc/ImtblLogging.h"

UImtblConnectionAsyncActions* UImtblConnectionAsyncActions::Login(UObject* WorldContextObject, const FImmutableDirectLoginOptions& DirectLoginOptions)
{
	UImtblConnectionAsyncActions* Node = NewObject<UImtblConnectionAsyncActions>();

	Node->WorldContextObject = WorldContextObject;
	Node->DirectLoginOptions = DirectLoginOptions;

	return Node;
}

void UImtblConnectionAsyncActions::Activate()
{
	if (!WorldContextObject || !WorldContextObject->GetWorld())
	{
		FString Error = "Connect failed due to missing world or world context object.";
		IMTBL_WARN("%s", *Error)
		Internal_DynamicMulticastDelegate_OnFailed.Broadcast(Error);

		return;
	}

	GetSubsystem()->WhenReady(this, &UImtblConnectionAsyncActions::DoConnect);
}

UImtblConnectionAsyncActions::FPassportConnectOutputPin* UImtblConnectionAsyncActions::DynamicMulticastDelegate_OnSuccess()
{
	return &Internal_DynamicMulticastDelegate_OnSuccess;
}

UImtblConnectionAsyncActions::FPassportConnectOutputPin* UImtblConnectionAsyncActions::DynamicMulticastDelegate_OnFailed()
{
	return &Internal_DynamicMulticastDelegate_OnFailed;
}

void UImtblConnectionAsyncActions::DoConnect(TWeakObjectPtr<UImtblJSConnector> JSConnector)
{
	auto Passport = GetSubsystem()->GetPassport();

	if (Passport.IsValid())
	{
#if PLATFORM_ANDROID | PLATFORM_IOS | PLATFORM_MAC | PLATFORM_WINDOWS
		Passport->Connect(UImmutablePassport::FImtblPassportResponseDelegate::CreateUObject(this, &UImtblConnectionAsyncActions::OnConnect), DirectLoginOptions);
#endif
	}
	else
	{
		IMTBL_ERR("Passport was not valid while trying to connect")
	}
}

void UImtblConnectionAsyncActions::OnConnect(FImmutablePassportResult Result)
{
	if (Result.Success)
	{
		Internal_DynamicMulticastDelegate_OnSuccess.Broadcast(Result.ToJsonString());
	}
	else
	{
		Internal_DynamicMulticastDelegate_OnFailed.Broadcast(Result.Error);
	}
}
