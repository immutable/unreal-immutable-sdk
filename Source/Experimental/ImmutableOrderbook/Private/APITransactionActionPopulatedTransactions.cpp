/**
 * TS SDK API
 * running ts sdk as an api
 *
 * OpenAPI spec version: 1.0.0
 * Contact: contact@immutable.com
 *
 * NOTE: This class is auto generated by OpenAPI Generator
 * https://github.com/OpenAPITools/openapi-generator
 * Do not edit the class manually.
 */

#include "APITransactionActionPopulatedTransactions.h"

#include "ImmutableOrderbookModule.h"
#include "APIHelpers.h"

#include "Templates/SharedPointer.h"

namespace ImmutableOrderbook
{

void APITransactionActionPopulatedTransactions::WriteJson(JsonWriter& Writer) const
{
	Writer->WriteObjectStart();
	if (To.IsSet())
	{
		Writer->WriteIdentifierPrefix(TEXT("to")); WriteJsonValue(Writer, To.GetValue());
	}
	if (From.IsSet())
	{
		Writer->WriteIdentifierPrefix(TEXT("from")); WriteJsonValue(Writer, From.GetValue());
	}
	if (Nonce.IsSet())
	{
		Writer->WriteIdentifierPrefix(TEXT("nonce")); WriteJsonValue(Writer, Nonce.GetValue());
	}
	if (GasLimit.IsSet())
	{
		Writer->WriteIdentifierPrefix(TEXT("gasLimit")); WriteJsonValue(Writer, GasLimit.GetValue());
	}
	if (GasPrice.IsSet())
	{
		Writer->WriteIdentifierPrefix(TEXT("gasPrice")); WriteJsonValue(Writer, GasPrice.GetValue());
	}
	if (Data.IsSet())
	{
		Writer->WriteIdentifierPrefix(TEXT("data")); WriteJsonValue(Writer, Data.GetValue());
	}
	if (Value.IsSet())
	{
		Writer->WriteIdentifierPrefix(TEXT("value")); WriteJsonValue(Writer, Value.GetValue());
	}
	if (ChainId.IsSet())
	{
		Writer->WriteIdentifierPrefix(TEXT("chainId")); WriteJsonValue(Writer, ChainId.GetValue());
	}
	if (Type.IsSet())
	{
		Writer->WriteIdentifierPrefix(TEXT("type")); WriteJsonValue(Writer, Type.GetValue());
	}
	if (AccessList.IsSet())
	{
		Writer->WriteIdentifierPrefix(TEXT("accessList")); WriteJsonValue(Writer, AccessList.GetValue());
	}
	if (MaxFeePerGas.IsSet())
	{
		Writer->WriteIdentifierPrefix(TEXT("maxFeePerGas")); WriteJsonValue(Writer, MaxFeePerGas.GetValue());
	}
	if (MaxPriorityFeePerGas.IsSet())
	{
		Writer->WriteIdentifierPrefix(TEXT("maxPriorityFeePerGas")); WriteJsonValue(Writer, MaxPriorityFeePerGas.GetValue());
	}
	if (CustomData.IsSet())
	{
		Writer->WriteIdentifierPrefix(TEXT("customData")); WriteJsonValue(Writer, CustomData.GetValue());
	}
	if (CcipReadEnabled.IsSet())
	{
		Writer->WriteIdentifierPrefix(TEXT("ccipReadEnabled")); WriteJsonValue(Writer, CcipReadEnabled.GetValue());
	}
	Writer->WriteObjectEnd();
}

bool APITransactionActionPopulatedTransactions::FromJson(const TSharedPtr<FJsonValue>& JsonValue)
{
	const TSharedPtr<FJsonObject>* Object;
	if (!JsonValue->TryGetObject(Object))
		return false;

	bool ParseSuccess = true;

	ParseSuccess &= TryGetJsonValue(*Object, TEXT("to"), To);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("from"), From);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("nonce"), Nonce);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("gasLimit"), GasLimit);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("gasPrice"), GasPrice);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("data"), Data);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("value"), Value);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("chainId"), ChainId);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("type"), Type);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("accessList"), AccessList);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("maxFeePerGas"), MaxFeePerGas);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("maxPriorityFeePerGas"), MaxPriorityFeePerGas);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("customData"), CustomData);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("ccipReadEnabled"), CcipReadEnabled);

	return ParseSuccess;
}

}
