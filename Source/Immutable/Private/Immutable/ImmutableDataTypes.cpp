// Fill out your copyright notice in the Description page of Project Settings.

#include "Immutable/ImmutableDataTypes.h"


FString FImmutablePassportInitData::ToJsonString() const
{
	FString OutString;
	FJsonObjectWrapper Wrapper;
	Wrapper.JsonObject = MakeShared<FJsonObject>();
	FJsonObjectConverter::UStructToJsonObject(StaticStruct(), this, Wrapper.JsonObject.ToSharedRef(), 0, 0);

	if (!Wrapper.JsonObject.IsValid())
	{
		IMTBL_ERR("Could not convert FImmutablePassportInitData to JSON")
		return "";
	}
	// Remove redirectUri field if it's empty so that the bridge doesn't try to
	// use it
	if (Wrapper.JsonObject->HasField("redirectUri") && Wrapper.JsonObject->GetStringField("redirectUri").IsEmpty())
	{
		Wrapper.JsonObject->RemoveField("redirectUri");
	}
	Wrapper.JsonObjectToString(OutString);

	return OutString;
}

TOptional<FImmutablePassportInitDeviceFlowData> FImmutablePassportInitDeviceFlowData::FromJsonString(const FString& JsonObjectString)
{
	FImmutablePassportInitDeviceFlowData PassportConnect;
	
	if (!FJsonObjectConverter::JsonObjectStringToUStruct(JsonObjectString, &PassportConnect, 0, 0))
	{
		IMTBL_WARN("Could not parse response from JavaScript into the expected " "Passport connect format")
		return TOptional<FImmutablePassportInitDeviceFlowData>();
	}
	
	return PassportConnect;
}

FString FImmutablePassportZkEvmRequestAccountsData::ToJsonString() const
{
	FString OutString;
	
	FJsonObjectConverter::UStructToJsonObjectString(*this, OutString, 0, 0, 0, nullptr, false);

	return OutString;
}

TOptional<FImmutablePassportZkEvmRequestAccountsData> FImmutablePassportZkEvmRequestAccountsData::FromJsonString(const FString& JsonObjectString)
{
	FImmutablePassportZkEvmRequestAccountsData RequestAccounts;
	if (!FJsonObjectConverter::JsonObjectStringToUStruct(JsonObjectString, &RequestAccounts, 0, 0))
	{
		IMTBL_WARN("Could not parse response from JavaScript into the expected " "Passport ZkEvm request accounts format")
		return TOptional<FImmutablePassportZkEvmRequestAccountsData>();
	}
	
	return RequestAccounts;
}

TOptional<FImmutablePassportZkEvmRequestAccountsData> FImmutablePassportZkEvmRequestAccountsData::FromJsonObject(const TSharedPtr<FJsonObject>& JsonObject)
{
	if (!JsonObject.IsValid()) { return TOptional<FImmutablePassportZkEvmRequestAccountsData>(); }

	// Parse the JSON
	FImmutablePassportZkEvmRequestAccountsData RequestAccounts;
	if (!FJsonObjectConverter::JsonObjectToUStruct(JsonObject.ToSharedRef(), &RequestAccounts, 0, 0))
	{
		IMTBL_ERR("Could not parse response from JavaScript into the expected " "Passport ZkEvm request accounts format")
		return TOptional<FImmutablePassportZkEvmRequestAccountsData>();
	}
	
	return RequestAccounts;
}

FString FImmutablePassportZkEvmGetBalanceData::ToJsonString() const
{
	FString OutString;

	FJsonObjectWrapper Wrapper;
	Wrapper.JsonObject = MakeShared<FJsonObject>();
	FJsonObjectConverter::UStructToJsonObject(StaticStruct(), this, Wrapper.JsonObject.ToSharedRef(), 0, 0);

	if (!Wrapper.JsonObject.IsValid())
	{
		IMTBL_ERR("Could not convert FImmutablePassportZkEvmGetBalanceData to JSON")
		return "";
	}
	Wrapper.JsonObjectToString(OutString);

	return OutString;
}
