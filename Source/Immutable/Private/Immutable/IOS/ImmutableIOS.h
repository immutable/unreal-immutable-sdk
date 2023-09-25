#pragma once

#import <Foundation/Foundation.h>
#import <AuthenticationServices/ASWebAuthenticationSession.h>

#if __IPHONE_OS_VERSION_MAX_ALLOWED >= 130000
@interface ImmutableIOS: NSObject<ASWebAuthenticationPresentationContextProviding>
+(ImmutableIOS*)instance;
-(void)launchUrl: (const char*)url;
@end
#endif