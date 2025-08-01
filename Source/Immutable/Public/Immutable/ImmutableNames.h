#pragma once

namespace ImmutablePassportAction
{
	const FString INIT = TEXT("init");
	const FString Logout = TEXT("logout");
	const FString Connect = TEXT("connect");
	const FString ConnectEvm = TEXT("connectEvm");
	const FString ZkEvmRequestAccounts = TEXT("zkEvmRequestAccounts");
	const FString ZkEvmGetBalance = TEXT("zkEvmGetBalance");
	const FString ZkEvmSendTransaction = TEXT("zkEvmSendTransaction");
	const FString zkEvmSendTransactionWithConfirmation = TEXT("zkEvmSendTransactionWithConfirmation");
	const FString ZkEvmGetTransactionReceipt = TEXT("zkEvmGetTransactionReceipt");
	const FString ZkEvmSignTypedDataV4 = TEXT("zkEvmSignTypedDataV4");

#if PLATFORM_ANDROID | PLATFORM_IOS | PLATFORM_MAC | PLATFORM_WINDOWS
	const FString GetPKCEAuthUrl = TEXT("getPKCEAuthUrl");
	const FString LOGIN_PKCE = TEXT("loginPKCE");
	const FString CONNECT_PKCE = TEXT("connectPKCE");
#endif

	const FString GetAddress = TEXT("getAddress");
	const FString GetEmail = TEXT("getEmail");
	const FString GetAccessToken = TEXT("getAccessToken");
	const FString GetIdToken = TEXT("getIdToken");
	const FString GetLinkedAddresses = TEXT("getLinkedAddresses");
	const FString ImxTransfer = TEXT("imxTransfer");
	const FString ImxBatchNftTransfer = TEXT("imxBatchNftTransfer");
	const FString ImxIsRegisteredOffchain = TEXT("isRegisteredOffchain");
	const FString ImxRegisterOffchain = TEXT("registerOffchain");

	const FString TRACK = TEXT("track");
} // namespace ImmutablePassportAction

namespace ImmutablePassportEnvironmentConstants
{
	const FString EnvironmentSandbox = TEXT("sandbox");
	const FString EnvironmentProduction = TEXT("production");
}