#pragma once

#import <AuthenticationServices/ASWebAuthenticationSession.h>
#import <Foundation/Foundation.h>

@interface ImmutableMac
    : NSObject <ASWebAuthenticationPresentationContextProviding>
+ (ImmutableMac *)instance;
- (void)launchUrl:(const char *)url forRedirectUri:(const char *)redirectUri;
- (void)launchUrlInBrowser:(const char *)url;
@end
