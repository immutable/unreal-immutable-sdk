#pragma once

#include "CoreMinimal.h"
#include "Misc/GeneratedTypeName.h"

IMMUTABLE_API DECLARE_LOG_CATEGORY_EXTERN(LogImmutable, Log, All);

// Disable for shipping builds
#if NO_LOGGING
IMMUTABLE_API DECLARE_LOG_CATEGORY_EXTERN(LogClean, Log, All);
#else
// Direct implementation of the DECLARE_LOG_CATEGORY_EXTERN macro
IMMUTABLE_API extern struct FLogCategoryLogClean : public FLogCategory<ELogVerbosity::Log, ELogVerbosity::All>
{
	FORCEINLINE FLogCategoryLogClean() : FLogCategory(TEXT(""))
	{
	}
} LogClean;
#endif

#define IMTBL_LOG_CLEAN(Format, ...)                                           \
  UE_LOG(LogClean, Log, TEXT(Format), ##__VA_ARGS__)

// IMMUTABLE_API DECLARE_LOG_CATEGORY_EXTERN(LogFuncSig, Log, All);

#define IMTBL_FUNCNAME __FUNCTION__
#if defined(_MSC_VER) && !defined(__clang__)
#define IMTBL_FUNCSIG __FUNCSIG__
#else
#define IMTBL_FUNCSIG __PRETTY_FUNCTION__
#endif

#define IMTBL_LOG_FUNCSIG                                                      \
  UE_LOG(LogImmutable, Log, TEXT("%s"), *FString(IMTBL_FUNCSIG))

#define IMTBL_LOG(Format, ...)                                                 \
  { UE_LOG(LogImmutable, Log, TEXT(Format), ##__VA_ARGS__) }

#define IMTBL_LOG_FUNC(Format, ...)                                            \
  {                                                                            \
    const FString _Msg = FString::Printf(TEXT(Format), ##__VA_ARGS__);         \
    UE_LOG(LogImmutable, Log, TEXT("%s: %s"), *FString(IMTBL_FUNCNAME), *_Msg) \
  }

#define IMTBL_DISPLAY(Format, ...)                                             \
  { UE_LOG(LogImmutable, Display, TEXT(Format), ##__VA_ARGS__) }

#define IMTBL_VERBOSE(Format, ...)                                             \
  { UE_LOG(LogImmutable, Verbose, TEXT(Format), ##__VA_ARGS__) }

#define IMTBL_VERBOSE_FUNC(Format, ...)                                        \
  {                                                                            \
    const FString _Msg = FString::Printf(TEXT(Format), ##__VA_ARGS__);         \
    UE_LOG(LogImmutable, Verbose, TEXT("%s: %s"), *FString(IMTBL_FUNCSIG),     \
           *_Msg)                                                              \
  }

#define IMTBL_WARN(Format, ...)                                                \
  { UE_LOG(LogImmutable, Warning, TEXT(Format), ##__VA_ARGS__) }

#define IMTBL_WARN_FUNC(Format, ...)                                           \
  {                                                                            \
    const FString _Msg = FString::Printf(TEXT(Format), ##__VA_ARGS__);         \
    UE_LOG(LogImmutable, Warning, TEXT("%s: %s"), *FString(IMTBL_FUNCNAME),    \
           *_Msg)                                                              \
  }

#define IMTBL_ERR(Format, ...)                                                 \
  { UE_LOG(LogImmutable, Error, TEXT(Format), ##__VA_ARGS__) }

#define IMTBL_ERR_FUNC(Format, ...)                                            \
  {                                                                            \
    const FString _Msg = FString::Printf(TEXT(Format), ##__VA_ARGS__);         \
    UE_LOG(LogImmutable, Error, TEXT("%s: %s"), *FString(IMTBL_FUNCNAME),      \
           *_Msg)                                                              \
  }
