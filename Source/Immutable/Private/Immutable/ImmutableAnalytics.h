#pragma once

#include "Immutable/ImmutablePassport.h"

#include "ImmutableAnalytics.generated.h"

USTRUCT(BlueprintType)
struct FEventData
{
	GENERATED_BODY()

	UPROPERTY()
	FString moduleName;

	UPROPERTY()
	FString eventName;

	UPROPERTY()
	FString properties;
};

/**
 * Immutable bridge sdk analytics utility
 */
UCLASS()
class IMMUTABLE_API UImmutableAnalytics : public UObject
{
	GENERATED_BODY()
	
public:

/**
 * Event names to track
 */
#define EVENT_NAME_LIST \
CONVERT(INIT_PASSPORT, "initialisedPassport") \
CONVERT(START_LOGIN, "startedLogin") \
CONVERT(COMPLETE_LOGIN, "performedLogin") \
CONVERT(START_LOGIN_PKCE, "startedLoginPkce") \
CONVERT(COMPLETE_LOGIN_PKCE, "performedLoginPkce") \
CONVERT(COMPLETE_RELOGIN, "performedRelogin") \
CONVERT(START_CONNECT_IMX, "startedConnectImx") \
CONVERT(COMPLETE_CONNECT_IMX, "performedConnectImx") \
CONVERT(START_CONNECT_IMX_PKCE, "startedConnectImxPkce") \
CONVERT(COMPLETE_CONNECT_IMX_PKCE, "performedConnectImxPkce") \
CONVERT(COMPLETE_RECONNECT, "performedReconnect") \
CONVERT(COMPLETE_LOGOUT, "performedLogout") \
CONVERT(COMPLETE_LOGOUT_PKCE, "performedLogoutPkce")

	enum class EEventName: uint8
	{
		#define CONVERT(name, nameString) name,
		EVENT_NAME_LIST
		#undef CONVERT
	};

public:
	void Setup(TWeakObjectPtr<class UImtblJSConnector> Connector);
    /**
     * Performs the call to game bridge track method
     * @param EventName Name that will be tracked
     * @param Success Single event data record that track "succeeded" field     
     * @param EventData Map with customed data, converted to json object 
     */
	void Track(EEventName EventName);
	void Track(EEventName EventName, bool Success);
	void Track(EEventName EventName, TMap<FString, FString>& EventData);

private:
    // Convert enum to string
	FString GetEventName(EEventName EventName);
	// Callback method for Track from bridge 
	void OnTrackResponse(FImtblJSResponse Response);
	
private:
	TWeakObjectPtr<class UImtblJSConnector> JSConnector;

};