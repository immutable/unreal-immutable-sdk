#pragma once

#import <AuthenticationServices/ASWebAuthenticationSession.h>
#import <Foundation/Foundation.h>

@interface ImmutableIOS
    : NSObject <ASWebAuthenticationPresentationContextProviding>
+ (ImmutableIOS *)instance;
- (void)launchUrl:(const char *)url;
@end