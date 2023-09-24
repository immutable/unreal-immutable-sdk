#pragma once

#if PLATFORM_ANDROID
#include "Android/AndroidJNI.h"

extern "C"
{
    static void (*DeeplinkCallbackMethod)(FString Deeplink);
    
    JNI_METHOD void Java_com_epicgames_unreal_GameActivity_handleDeepLink(JNIEnv*, jobject, jstring);

    void SetDeeplinkCallbackMethod(void (*callbackMethod)(FString Deeplink));
}

#endif
