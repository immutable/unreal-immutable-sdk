#include "ImmutableMac.h"
#include "Immutable/ImmutablePassport.h"
#include "Immutable/ImmutableSubsystem.h"
#include "Engine/GameEngine.h"

#if WITH_EDITOR
#include "Editor.h"
#endif

ASWebAuthenticationSession *_authSession;

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
    UWorld* World = nullptr;

#if WITH_EDITOR
	if (GEditor)
	{
		for (const auto& Context : GEditor->GetWorldContexts())
		{
			if (Context.WorldType == EWorldType::PIE && Context.World())
			{
				World = Context.World();
				break;
			}
		}
	}
#else
	if (UGameEngine* GameEngine = Cast<UGameEngine>(GEngine))
	{
		World = GameEngine->GetGameWorld();
	}
#endif

	if (!World) {
		return nil;
	}

	auto ImmutableSubsystem = World->GetGameInstance()->GetSubsystem<UImmutableSubsystem>();

	if (!ImmutableSubsystem) {
		return nil;
	}

	auto Passport = ImmutableSubsystem->GetPassport();

	if (!Passport.IsValid()) {
		return nil;
	}

	return Passport.Get();
}

- (void)launchUrl:(const char *)url forRedirectUri:(const char *)redirectUri {
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

- (ASPresentationAnchor)presentationAnchorForWebAuthenticationSession:
    (ASWebAuthenticationSession *)session API_AVAILABLE(macos(10.15)) {
  return [[[NSApplication sharedApplication] windows] firstObject];
}

@end
