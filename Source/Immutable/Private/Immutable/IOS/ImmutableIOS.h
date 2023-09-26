#pragma once

#import <Foundation/Foundation.h>
#import <AuthenticationServices/ASWebAuthenticationSession.h>

@interface ImmutableIOS: NSObject<ASWebAuthenticationPresentationContextProviding>
+(ImmutableIOS*)instance;
-(void)launchUrl: (const char*)url;
@end