#pragma once

namespace ImmutablePassportAction
{
	const FString INIT = TEXT("init");
	const FString INIT_DEVICE_FLOW = TEXT("initDeviceFlow");
	const FString RELOGIN = TEXT("relogin");
	const FString LOGIN_CONFIRM_CODE = TEXT("loginConfirmCode");
	const FString CONNECT_CONFIRM_CODE = TEXT("connectConfirmCode");
	const FString RECONNECT = TEXT("reconnect");
	const FString Logout = TEXT("logout");
	const FString Connect = TEXT("connect");
	const FString ConnectEvm = TEXT("connectEvm");
	const FString ZkEvmRequestAccounts = TEXT("zkEvmRequestAccounts");
	const FString ZkEvmGetBalance = TEXT("zkEvmGetBalance");
	const FString ZkEvmSendTransaction = TEXT("zkEvmSendTransaction");
	const FString zkEvmSendTransactionWithConfirmation = TEXT("zkEvmSendTransactionWithConfirmation");
	const FString ZkEvmGetTransactionReceipt = TEXT("zkEvmGetTransactionReceipt");

#if PLATFORM_ANDROID | PLATFORM_IOS | PLATFORM_MAC
	const FString GetPKCEAuthUrl = TEXT("getPKCEAuthUrl");
	const FString LOGIN_PKCE = TEXT("loginPKCE");; //+
	const FString CONNECT_PKCE = TEXT("connectPKCE");
#endif

	const FString GetAddress = TEXT("getAddress");
	const FString GetEmail = TEXT("getEmail");
	const FString GetAccessToken = TEXT("getAccessToken");
	const FString GetIdToken = TEXT("getIdToken");
	const FString GetLinkedAddresses = TEXT("getLinkedAddresses");
	const FString ImxTransfer = TEXT("imxTransfer");
	const FString ImxBatchNftTransfer = TEXT("imxBatchNftTransfer");
	const FString EnvSandbox = TEXT("sandbox");
	const FString EnvProduction = TEXT("production");
	const FString ImxIsRegisteredOffchain = TEXT("isRegisteredOffchain");
	const FString ImxRegisterOffchain = TEXT("registerOffchain");

	const FString TRACK = TEXT("track");
} // namespace ImmutablePassportAction
