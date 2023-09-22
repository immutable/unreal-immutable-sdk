#include "ImmutableAndroidJNI.h"

#if PLATFORM_ANDROID
JNI_METHOD void Java_com_epicgames_unreal_GameActivity_handleDeepLink(JNIEnv* env, jobject obj, jstring jDeeplink)
{
    if (env->IsSameObject(jDeeplink, NULL))
    {
        return;
    }

    const char *deeplinkCStr = env->GetStringUTFChars(jDeeplink, NULL);
    const FString deeplink = FString(UTF8_TO_TCHAR(deeplinkCStr));
    DeeplinkCallbackMethod(deeplink);
    env->ReleaseStringUTFChars(jDeeplink, deeplinkCStr);
}

void SetDeeplinkCallbackMethod(void (*CallbackMethod)(FString Deeplink))
{
    if (DeeplinkCallbackMethod == NULL)
    {
        DeeplinkCallbackMethod = CallbackMethod;
    }
}

#endif