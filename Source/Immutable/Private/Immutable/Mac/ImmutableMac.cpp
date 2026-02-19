#include "ImmutableMac.h"

#include "Engine/GameEngine.h"

#include "Immutable/ImmutablePassport.h"
#include "Immutable/ImmutableSubsystem.h"
#include "Immutable/Misc/ImtblLogging.h"

#if WITH_EDITOR
#include "Editor.h"
#endif

@interface ImmutableMac () {
	ASWebAuthenticationSession* _authSession;
}
@end

@implementation ImmutableMac

- (instancetype)init {
  self = [super init];
  return self;
}

+ (ImmutableMac *)instance {
  static ImmutableMac *staticImmutableMac;
  static dispatch_once_t once;
  dispatch_once(&once, ^{
    staticImmutableMac = [[self alloc] init];
  });
  return staticImmutableMac;
}

+ (UImmutablePassport*) getPassport {
	auto PassportFromWorld = [](UWorld* World) -> UImmutablePassport* {
		if (!World) return nullptr;
		UGameInstance* GameInstance = World->GetGameInstance();
		if (!GameInstance) return nullptr;
		UImmutableSubsystem* Subsystem = GameInstance->GetSubsystem<UImmutableSubsystem>();
		if (!Subsystem) return nullptr;
		return Subsystem->GetPassport().Get();
	};
#if WITH_EDITOR
	if (GEditor)
	{
		for (const FWorldContext& Context : GEditor->GetWorldContexts())
		{
			if (UImmutablePassport* Passport = PassportFromWorld(Context.World()))
			{
				return Passport;
			}
		}
	}
	return nullptr;
#else
	UWorld* World = nullptr;
	if (UGameEngine* GameEngine = Cast<UGameEngine>(GEngine))
	{
		World = GameEngine->GetGameWorld();
	}
	return PassportFromWorld(World);
#endif
}

- (void)launchUrl:(const char *)url forRedirectUri:(const char *)redirectUri {
  // For automation, use the browser-based method
  if (GIsAutomationTesting) {
    IMTBL_LOG("Using automation mode for authentication (GIsAutomationTesting is true)");
    [self launchUrlInBrowser:url];
    return;
  }

  if (@available(macOS 10.15, *)) {
    NSURL *URL =
        [NSURL URLWithString:[[NSString alloc] initWithUTF8String:url]];

    // Bundle identifier does not work like iOS, so using callback URL scheme
    // from redirect URI instead
    NSString *redirectUriString =
        [[NSString alloc] initWithUTF8String:redirectUri];
    NSString *callbackURLScheme =
        [[redirectUriString componentsSeparatedByString:@":"] objectAtIndex:0];

    ASWebAuthenticationSession *authSession =
        [[ASWebAuthenticationSession alloc]
                  initWithURL:URL
            callbackURLScheme:callbackURLScheme
            completionHandler:^(NSURL *_Nullable callbackURL,
                                NSError *_Nullable error) {
              _authSession = nil;

              if (callbackURL) {
                UImmutablePassport* passport = [ImmutableMac getPassport];

                if (passport) {
                  passport->HandleDeepLink(callbackURL.absoluteString);
                }
              } else {
              	IMTBL_ERR("callbackURL is empty");
                return;
              }
            }];

    _authSession = authSession;
    _authSession.presentationContextProvider = self;
    [_authSession start];
  }
}

- (void)launchUrlInBrowser:(const char *)url {
    // Add redundant check to ensure this only runs for automated testing
    if (!GIsAutomationTesting) {
        return;
    }

    // Create URL object
    NSURL *URL = [NSURL URLWithString:[[NSString alloc] initWithUTF8String:url]];
    
    // Open URL in default browser
    [[NSWorkspace sharedWorkspace] openURL:URL];
    IMTBL_LOG("Opened URL in browser for automation: %s", URL.absoluteString.UTF8String);
}

- (ASPresentationAnchor)presentationAnchorForWebAuthenticationSession:
    (ASWebAuthenticationSession *)session API_AVAILABLE(macos(10.15)) {
  return [[[NSApplication sharedApplication] windows] firstObject];
}

@end
