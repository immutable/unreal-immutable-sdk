#include "ImmutableAndroidJNI.h"

#include "Immutable/ImmutablePassport.h"
#include "Immutable/ImmutableSubsystem.h"


#if PLATFORM_ANDROID

const UImmutablePassport* GetPassport()
{
  if (!GEngine)
  {
    return nullptr;
  }

  const auto GameWorld = GEngine->GetWorld();

  if (!GameWorld)
  {
    return nullptr;
  }

  auto ImmutableSubsystem = GameWorld->GetGameInstance()->GetSubsystem<UImmutableSubsystem>();

  if (!ImmutableSubsystem)
  {
    return nullptr;
  }

  auto Passport = ImmutableSubsystem->GetPassport();

  if (!Passport.IsValid())
  {
    return nullptr;
  }

  return Passport.Get();
}

JNI_METHOD void Java_com_epicgames_unreal_GameActivity_handleDeepLink(JNIEnv *env, jobject obj, jstring jDeeplink)
{
	if (env->IsSameObject(jDeeplink, NULL))
	{
		return;
	}

	const char *deeplinkCStr = env->GetStringUTFChars(jDeeplink, NULL);
	const FString deeplink = FString(UTF8_TO_TCHAR(deeplinkCStr));

	if (auto Passport = GetPassport())
	{
		Passport->HandleDeepLink(deeplink);
	}
	env->ReleaseStringUTFChars(jDeeplink, deeplinkCStr);
}

JNI_METHOD void Java_com_epicgames_unreal_GameActivity_handleOnCustomTabsDismissed(JNIEnv *env, jobject obj, jstring jUrl)
{
	if (env->IsSameObject(jUrl, NULL))
	{
		return;
	}

	if (auto Passport = GetPassport())
	{
		Passport->HandleCustomTabsDismissed(FString(UTF8_TO_TCHAR(env->GetStringUTFChars(jUrl, NULL))));
	}
}
#endif