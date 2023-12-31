#include "ImmutableAndroidJNI.h"
#include "Immutable/ImmutablePassport.h"

#if PLATFORM_ANDROID
JNI_METHOD void
Java_com_epicgames_unreal_GameActivity_handleDeepLink(JNIEnv *env, jobject obj,
                                                      jstring jDeeplink) {
  if (env->IsSameObject(jDeeplink, NULL)) {
    return;
  }

  const char *deeplinkCStr = env->GetStringUTFChars(jDeeplink, NULL);
  const FString deeplink = FString(UTF8_TO_TCHAR(deeplinkCStr));
  UImmutablePassport::HandleDeepLink(deeplink);
  env->ReleaseStringUTFChars(jDeeplink, deeplinkCStr);
}

JNI_METHOD void
Java_com_epicgames_unreal_GameActivity_handleOnCustomTabsDismissed(
    JNIEnv *env, jobject obj) {
  UImmutablePassport::HandleCustomTabsDismissed();
}
#endif