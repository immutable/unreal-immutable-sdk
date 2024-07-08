// Fill out your copyright notice in the Description page of Project Settings.

#include "Immutable/Actions/ImtblPassportGetTokenAsyncAction.h"

#include "Immutable/ImmutablePassport.h"
#include "Immutable/ImmutableSubsystem.h"
#include "Immutable/Misc/ImtblLogging.h"

UImtblPassportGetTokenAsyncAction* UImtblPassportGetTokenAsyncAction::GetAccessToken(UObject* WorldContextObject)
{
	UImtblPassportGetTokenAsyncAction* Node = NewObject<UImtblPassportGetTokenAsyncAction>();

	Node->WorldContextObject = WorldContextObject;
	Node->Type = ACCESS;
	
	return Node;
}

UImtblPassportGetTokenAsyncAction* UImtblPassportGetTokenAsyncAction::GetIdToken(UObject* WorldContextObject)
{
	UImtblPassportGetTokenAsyncAction* Node = NewObject<UImtblPassportGetTokenAsyncAction>();

	Node->WorldContextObject = WorldContextObject;
	Node->Type = ID;

	return Node;
}

void UImtblPassportGetTokenAsyncAction::Activate()
{
	if (!WorldContextObject || !WorldContextObject->GetWorld())
	{
		FString Err = "Get Token failed due to missing world or world context object.";
		IMTBL_WARN("%s", *Err)
		Failed.Broadcast(Err, TEXT(""));
		return;
	}

	
	GetSubsystem()->WhenReady(this, &UImtblPassportGetTokenAsyncAction::DoGetToken);
}

void UImtblPassportGetTokenAsyncAction::DoGetToken(TWeakObjectPtr<UImtblJSConnector> JSConnector)
{
	auto Passport = GetSubsystem()->GetPassport();

	if (Passport.IsValid())
	{
		switch(Type)
		{
		case ID:
			Passport->GetIdToken(UImmutablePassport::FImtblPassportResponseDelegate::CreateUObject(this, &UImtblPassportGetTokenAsyncAction::OnGetTokenResponse));
			break;
		case ACCESS:
			Passport->GetAccessToken(UImmutablePassport::FImtblPassportResponseDelegate::CreateUObject(this, &UImtblPassportGetTokenAsyncAction::OnGetTokenResponse));
			break;
		}
	}
}

void UImtblPassportGetTokenAsyncAction::OnGetTokenResponse(FImmutablePassportResult Result)
{
	if (Result.Success)
	{
		Success.Broadcast(TEXT(""), UImmutablePassport::GetResponseResultAsString(Result.Response));
	}
	else
	{
		Failed.Broadcast(Result.Error, TEXT(""));
	}
}
