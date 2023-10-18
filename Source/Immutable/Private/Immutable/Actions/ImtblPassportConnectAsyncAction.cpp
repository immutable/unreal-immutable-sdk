// Fill out your copyright notice in the Description page of Project Settings.

#include "Immutable/Actions/ImtblPassportConnectAsyncAction.h"

#include "Immutable/ImmutablePassport.h"
#include "Immutable/ImmutableSubsystem.h"
#include "Immutable/Misc/ImtblLogging.h"

UImtblPassportConnectAsyncAction *
UImtblPassportConnectAsyncAction::Connect(UObject *WorldContextObject) {
  UImtblPassportConnectAsyncAction *PassportInitBlueprintNode =
      NewObject<UImtblPassportConnectAsyncAction>();
  PassportInitBlueprintNode->WorldContextObject = WorldContextObject;
  return PassportInitBlueprintNode;
}

void UImtblPassportConnectAsyncAction::Activate() {
  if (!WorldContextObject || !WorldContextObject->GetWorld()) {
    FString Err =
        "Connect failed due to missing world or world context object.";
    IMTBL_WARN("%s", *Err)
    Failed.Broadcast(Err);
    return;
  }

  GetSubsystem()->WhenReady(
      this,
      &UImtblPassportConnectAsyncAction::DoConnect); //, /* timoutSec */ 15.0f);
}

void UImtblPassportConnectAsyncAction::DoConnect(
    TWeakObjectPtr<UImtblJSConnector> JSConnector) {
  // Get Passport
  auto Passport = GetSubsystem()->GetPassport();
  // Run Connect
  Passport->Connect(
      UImmutablePassport::FImtblPassportResponseDelegate::CreateUObject(
          this, &UImtblPassportConnectAsyncAction::OnConnect));
}

void UImtblPassportConnectAsyncAction::OnConnect(
    FImmutablePassportResult Result) {
  if (Result.Success) {
    Success.Broadcast(TEXT(""));
  } else {
    Failed.Broadcast(Result.Message);
  }
}
