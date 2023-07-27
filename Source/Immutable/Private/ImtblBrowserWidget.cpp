// Fill out your copyright notice in the Description page of Project Settings.


#include "ImtblBrowserWidget.h"

#include "ImtblJSConnector.h"
#include "ImtblLogging.h"
#include "SWebBrowser.h"
#include "Interfaces/IPluginManager.h"


UImtblBrowserWidget::UImtblBrowserWidget()
{
	IMTBL_LOG_FUNCSIG

	// WebBrowserWidget->LoadString("<html><head><title>Test</title></head><body><h1>Test</h1></body></html>", TEXT("http://www.google.com"));
	// InitialURL = TEXT("http://www.google.com");
	// InitialURL = TEXT("chrome://version");
    IPluginManager& PluginManager = IPluginManager::Get();
    if (const TSharedPtr<IPlugin> Plugin = PluginManager.FindPlugin("Immutable"))
    {
        InitialURL = FString::Printf(TEXT("%s%s"), TEXT("file:///"), *FPaths::ConvertRelativePathToFull(FPaths::Combine(Plugin->GetContentDir(), TEXT("index.html"))));
        IMTBL_LOG("Loading initial url: %s", *InitialURL)
    }
}


void UImtblBrowserWidget::BindConnector()
{
    if (JSConnector && JSConnector->IsBound())
        return;

    IMTBL_LOG("Setting up %s...", *UImtblJSConnector::StaticClass()->GetName())

    GetJSConnector();

    if (JSConnector)
    {
        if (BindUObject(UImtblJSConnector::JSObjectName(), JSConnector))
            JSConnector->SetBound();
    }
}


TWeakObjectPtr<UImtblJSConnector> UImtblBrowserWidget::GetJSConnector()
{
    if (!JSConnector)
        JSConnector = NewObject<UImtblJSConnector>(this);
    return JSConnector;
}


bool UImtblBrowserWidget::IsPageLoaded() const
{
    return WebBrowserWidget.IsValid() && WebBrowserWidget->IsLoaded();
}


void UImtblBrowserWidget::ExecuteJS(const FString& ScriptText) const
{
    if (WebBrowserWidget.IsValid())
    {
        WebBrowserWidget->ExecuteJavascript(ScriptText);
    }
}


bool UImtblBrowserWidget::BindUObject(const FString& Name, UObject* Object, const bool bIsPermanent) const
{
    if (!WebBrowserWidget)
    {
        IMTBL_WARN_FUNC("Could not bind UObject '%s' to browser, WebBrowserWidget is null", *Object->GetName())
        return false;
    }

    WebBrowserWidget->BindUObject(Name, Object, bIsPermanent);
    return true;
}


void UImtblBrowserWidget::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	WebBrowserWidget.Reset();
}


TSharedRef<SWidget> UImtblBrowserWidget::RebuildWidget()
{
	if (IsDesignTime())
	{
		return SNew(SBox)
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock)
				.Text(NSLOCTEXT("Immutable", "Immutable Web Browser", "Immutable Web Browser"))
			];
	}
	else
	{
		WebBrowserWidget = SNew(SWebBrowser)
			.InitialURL(InitialURL)
			.ShowControls(false)
			.SupportsTransparency(bSupportsTransparency)
	        .ShowInitialThrobber(bShowInitialThrobber)
#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 1
	        .OnConsoleMessage(BIND_UOBJECT_DELEGATE(FOnConsoleMessageDelegate, HandleOnConsoleMessage))
#endif
	    ;

		return WebBrowserWidget.ToSharedRef();
	}
}


void UImtblBrowserWidget::OnWidgetRebuilt()
{
    Super::OnWidgetRebuilt();
    BindConnector();
}


#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 1
void UImtblBrowserWidget::HandleOnConsoleMessage(const FString& Message, const FString& Source, int32 Line, EWebBrowserConsoleLogSeverity Severity)
{
    // TODO: add severity to log and callback
    IMTBL_LOG("Browser console message: %s, Source: %s, Line: %d", *Message, *Source, Line);
	OnConsoleMessage.Broadcast(Message, Source, Line);
}
#endif
