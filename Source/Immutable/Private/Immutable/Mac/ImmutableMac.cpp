#include "ImmutableMac.h"
#include "Immutable/ImmutablePassport.h"

ASWebAuthenticationSession *_authSession;

@implementation ImmutableMac

- (instancetype)init
{
	self = [super init];
	return self;
}

+ (ImmutableMac *)instance
{
	static ImmutableMac *staticImmutableMac;
	static dispatch_once_t once;
	dispatch_once(&once, ^{
		staticImmutableMac = [[self alloc] init];
	});
	return staticImmutableMac;
}


- (void) launchUrl:(const char*)url forRedirectUri:(const char*)redirectUri
{
	if (@available(macOS 10.15, *)) {
		NSURL * URL = [NSURL URLWithString: [[NSString alloc] initWithUTF8String:url]];

        // Bundle identifier does not work like iOS, so using callback URL scheme from redirect URI instead
        NSString *redirectUriString = [[NSString alloc] initWithUTF8String:redirectUri];
        NSString *callbackURLScheme = [[redirectUriString componentsSeparatedByString:@":"] objectAtIndex:0];
        
		ASWebAuthenticationSession* authSession = [[ASWebAuthenticationSession alloc] initWithURL:URL callbackURLScheme:callbackURLScheme completionHandler:^(NSURL * _Nullable callbackURL, NSError * _Nullable error) {
			_authSession = nil;
            
			if (callbackURL) {
 				UImmutablePassport::HandleDeepLink(callbackURL.absoluteString);
			} else {
				return;
			}
		}];
	    
		_authSession = authSession;
		_authSession.presentationContextProvider = self;
		[_authSession start];
	}
}

- (ASPresentationAnchor)presentationAnchorForWebAuthenticationSession:(ASWebAuthenticationSession *)session API_AVAILABLE(macos(10.15)) {
    return [NSApp mainWindow];
}

@end
