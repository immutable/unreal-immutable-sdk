#pragma once

#include "CoreMinimal.h"
#include "Immutable/Misc/ImtblLogging.h"
#include "JsonObjectConverter.h"
// clang-format off
#include "ImtblJSMessages.generated.h"
// clang-format on

UENUM()
enum class EImtblPassportError : int8
{
	AuthenticationError,
	WalletConnectionError,
	UserRegistrationError,
	RefreshTokenError,
	TransferError,
	CreateOrderError,
	CancelOrderError,
	CreateTradeError,
	BatchTransferError,
	ExchangeTransferError,
	OperationNotSupportedError
	// unknown, timeout?
};

USTRUCT()
struct IMMUTABLE_API FImtblResponseError
{
	GENERATED_BODY()

	UPROPERTY()
	EImtblPassportError errorType = static_cast<EImtblPassportError>(-1);

	UPROPERTY()
	FString errorMessage;

	FString ToString()
	{
		const FString ErrType = StaticEnum<EImtblPassportError>()->GetNameStringByValue(static_cast<int8>(errorType));
		return ErrType == "" ? errorMessage : ErrType + ": " + errorMessage;
	}
};

USTRUCT()
struct IMMUTABLE_API FImtblJSRequest
{
	GENERATED_BODY()

	UPROPERTY()
	FString fxName;

	UPROPERTY()
	FString data;

	UPROPERTY()
	FString requestId;

	FString ToJsonString() const
	{
		FString OutString;
		FJsonObjectConverter::UStructToJsonObjectString<FImtblJSRequest>(*this, OutString, 0, 0, 0, nullptr, false);
		return OutString;
	}
};

USTRUCT()
struct IMMUTABLE_API FImtblJSResponse
{
	GENERATED_BODY()

	UPROPERTY()
	FString responseFor;

	UPROPERTY()
	FString requestId;

	UPROPERTY()
	bool success = false;

	TOptional<FImtblResponseError> Error;

	// Store the parsed response object for later in case we need to access more
	// data from it (tokens, addresses, user email, etc)
	TSharedPtr<FJsonObject> JsonObject;

	static TOptional<FImtblJSResponse> FromJsonString(const FString& JsonString)
	{
		TOptional<FImtblJSResponse> Response;
		FImtblJSResponse JSResponse;

		// Parse the JSON
		const TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(JsonString);
		if (!FJsonSerializer::Deserialize(JsonReader, JSResponse.JsonObject) || !JSResponse.JsonObject.IsValid())
		{
			IMTBL_ERR("Could not parse response from JavaScript -- invalid JSON: %s", *JsonString)
		}
		else if (!FJsonObjectConverter::JsonObjectToUStruct(JSResponse.JsonObject.ToSharedRef(), &JSResponse, 0, 0))
		{
			IMTBL_ERR("Could not parse response from JavaScript into the expected " "response object format: %s", *JsonString)
		}
		else
		{
			Response = JSResponse;
		}

		if (!JSResponse.success)
		{
			double ErrType = -1;
			FString Error;
			if (JSResponse.JsonObject.IsValid())
			{
				JSResponse.JsonObject->TryGetNumberField(TEXT("errorType"), ErrType);
				JSResponse.JsonObject->TryGetStringField(TEXT("error"), Error);
			}
			JSResponse.Error = FImtblResponseError{static_cast<EImtblPassportError>(ErrType), Error};
		}

		return Response;
	}
};
