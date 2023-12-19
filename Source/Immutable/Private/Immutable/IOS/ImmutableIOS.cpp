#include "ImmutableIOS.h"
#include "Immutable/ImmutablePassport.h"
#include "Immutable/ImmutableSubsystem.h"
#include "Engine/GameEngine.h"

API_AVAILABLE(ios(12.0))
ASWebAuthenticationSession *_authSession;

@implementation ImmutableIOS

- (instancetype)init {
  self = [super init];
  return self;
}

+ (ImmutableIOS *)instance {
  static ImmutableIOS *staticImmutableIOS;
  static dispatch_once_t once;
  dispatch_once(&once, ^{
    staticImmutableIOS = [[self alloc] init];
  });
  return staticImmutableIOS;
}

+ (UImmutablePassport*) getPassport {
	UGameEngine* GameEngine = Cast<UGameEngine>(GEngine);

	if (!GameEngine) {
		return nil;
	}

	UWorld* World = GameEngine ? GameEngine->GetGameWorld() : NULL;

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

- (void)launchUrl:(const char *)url {
  NSURL *URL = [NSURL URLWithString:[[NSString alloc] initWithUTF8String:url]];
  NSString *bundleIdentifier = [[NSBundle mainBundle] bundleIdentifier];

  ASWebAuthenticationSession *authSession = [[ASWebAuthenticationSession alloc]
            initWithURL:URL
      callbackURLScheme:bundleIdentifier
      completionHandler:^(NSURL *_Nullable callbackURL,
                          NSError *_Nullable error) {
        _authSession = nil;
        if (callbackURL) {
          UImmutablePassport* passport = [ImmutableIOS getPassport];

          if (passport) {
            passport->HandleDeepLink(callbackURL.absoluteString);
          }
        } else {
          return;
        }
      }];

  _authSession = authSession;
  _authSession.presentationContextProvider = (id)self;

  [_authSession start];
}

- (nonnull ASPresentationAnchor)presentationAnchorForWebAuthenticationSession:
    (nonnull ASWebAuthenticationSession *)session API_AVAILABLE(ios(13.0)) {
  return [[[UIApplication sharedApplication] windows] firstObject];
}

@end
