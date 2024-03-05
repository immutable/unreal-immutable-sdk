// Fill out your copyright notice in the Description page of Project Settings.

#include "Immutable/ImmutableRequests.h"

#include "JsonObjectConverter.h"
#include "JsonObjectWrapper.h"
#include "Immutable/Misc/ImtblLogging.h"
#include "Policies/CondensedJsonPrintPolicy.h"


FString FImxBatchNftTransferRequest::ToJsonString() const
{
	FString OutString;
	FJsonObjectWrapper Wrapper;
	Wrapper.JsonObject = MakeShared<FJsonObject>();
	FJsonObjectConverter::UStructToJsonObject(StaticStruct(), this, Wrapper.JsonObject.ToSharedRef(), 0, 0);

	if (!Wrapper.JsonObject.IsValid())
	{
		IMTBL_ERR("Could not convert FImxBatchNftTransferRequest to JSON")
		return "";
	}

	if (Wrapper.JsonObject->HasField("nftTransferDetails"))
	{
		const auto Writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&OutString);
		FJsonSerializer::Serialize(Wrapper.JsonObject->GetArrayField("nftTransferDetails"), Writer);
		IMTBL_LOG("FImxBatchNftTransferRequest Serialised: %s", *OutString);
		Writer->Close();
	}
	
	return OutString;
}
