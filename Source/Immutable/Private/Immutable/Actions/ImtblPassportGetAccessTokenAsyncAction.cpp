// Fill out your copyright notice in the Description page of Project Settings.

#include "Immutable/Actions/ImtblPassportGetAccessTokenAsyncAction.h"

#include "Immutable/ImmutablePassport.h"
#include "Immutable/ImmutableSubsystem.h"
#include "Immutable/Misc/ImtblLogging.h"

UImtblPassportGetAccessTokenAsyncAction *
UImtblPassportGetAccessTokenAsyncAction::GetAccessToken(UObject *WorldContextObject) {
  UImtblPassportGetAccessTokenAsyncAction *PassportInitBlueprintNode =
      NewObject<UImtblPassportGetAccessTokenAsyncAction>();
  PassportInitBlueprintNode->WorldContextObject = WorldContextObject;
  return PassportInitBlueprintNode;
}

void UImtblPassportGetAccessTokenAsyncAction::Activate() {
  if (!WorldContextObject || !WorldContextObject->GetWorld()) {
    FString Err =
        "GetAccessToken failed due to missing world or world context object.";
    IMTBL_WARN("%s", *Err)
    Failed.Broadcast(Err, TEXT(""));
    return;
  }

  GetSubsystem()->WhenReady(this,
                            &UImtblPassportGetAccessTokenAsyncAction::DoGetAccessToken);
}

void UImtblPassportGetAccessTokenAsyncAction::DoGetAccessToken(
    TWeakObjectPtr<UImtblJSConnector> JSConnector) {
  // Get Passport
  auto Passport = GetSubsystem()->GetPassport();
  // Run GetAccessToken
  Passport->GetAccessToken(
      UImmutablePassport::FImtblPassportResponseDelegate::CreateUObject(
          this, &UImtblPassportGetAccessTokenAsyncAction::OnGetAccessTokenResponse));
}

void UImtblPassportGetAccessTokenAsyncAction::OnGetAccessTokenResponse(
    FImmutablePassportResult Result) {
  if (Result.Success) {
    GotAccessToken.Broadcast(TEXT(""), Result.Message);
  } else {
    Failed.Broadcast(Result.Message, TEXT(""));
  }
}
