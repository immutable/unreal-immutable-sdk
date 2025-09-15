#include "Immutable/Browser/ImmutableJSConnectorBrowserWidget.h"

#include "Immutable/ImtblJSConnector.h"

void UImmutableJSConnectorBrowserWidget::PostInitProperties()
{
	Super::PostInitProperties();

	if (!IsTemplate())
	{
		JSConnector = NewObject<UImtblJSConnector>(this);
		JSConnector->ExecuteJs.BindUObject(this, &ThisClass::ExecuteJavaScript);
	}
}

UImtblJSConnector* UImmutableJSConnectorBrowserWidget::GetJSConnector() const
{
	return JSConnector;
}

void UImmutableJSConnectorBrowserWidget::ExecuteJavaScript(const FString& ScriptText) const
{
#if USING_BUNDLED_CEF
	if (WebBrowserWidget.IsValid())
	{
		WebBrowserWidget->ExecuteJavascript(ScriptText);
	}
#endif
}

bool UImmutableJSConnectorBrowserWidget::BindUObject(const FString& Name, UObject* Object, bool bIsPermanent) const
{
#if USING_BUNDLED_CEF
	if (!WebBrowserWidget.IsValid() || !Object)
	{
		UE_LOG(LogImmutable, Warning, TEXT("Could not bind UObject '%s' to browser, WebBrowserWidget is null"), *Object->GetName());
		return false;
	}

	WebBrowserWidget->BindUObject(Name, Object, bIsPermanent);
	return true;
#endif
	return false;
}

void UImmutableJSConnectorBrowserWidget::OnBrowserCreated()
{
	Super::OnBrowserCreated();

	SetupJavaScriptBindings();
}

void UImmutableJSConnectorBrowserWidget::SetupJavaScriptBindings()
{
	if (JSConnector && JSConnector->IsBound())
	{
		return;
	}

	if (JSConnector)
	{
		if (BindUObject(UImtblJSConnector::JSObjectName(), JSConnector))
		{
			JSConnector->Init(IsPageLoaded());
		}
	}
}