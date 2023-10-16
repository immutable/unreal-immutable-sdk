#pragma once

#import <Foundation/Foundation.h>
#import <AuthenticationServices/ASWebAuthenticationSession.h>

@interface ImmutableMac: NSObject<ASWebAuthenticationPresentationContextProviding>
+(ImmutableMac*)instance;
-(void)launchUrl: (const char*)url forRedirectUri:(const char*)redirectUri;
@end
