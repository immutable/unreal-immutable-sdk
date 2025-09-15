#pragma once

#include "ImmutableBaseBrowserWidget.h"

#include "ImmutableJSConnectorBrowserWidget.generated.h"

UCLASS()
class IMMUTABLE_API UImmutableJSConnectorBrowserWidget : public UImmutableBaseBrowserWidget
{
	GENERATED_BODY()

public:
	/** UObject: @Interface @Begin */
	virtual void PostInitProperties() override;
	/** UObject: @Interface @End */

	UFUNCTION(BlueprintPure)
	UImtblJSConnector* GetJSConnector() const;

	/**
	 * Execute JavaScript in the browser context
	 * @param ScriptText The JavaScript code to execute
	 */
	UFUNCTION(BlueprintCallable, Category = "Immutable|Browser")
	virtual void ExecuteJavaScript(const FString& ScriptText) const;

	/**
	 * Bind a UObject to be accessible from JavaScript
	 * @param Name The name to use in JavaScript (window.Name)
	 * @param Object The object to bind
	 * @param bIsPermanent Whether the binding persists across page loads
	 * @return True if binding was successful
	 */
	virtual bool BindUObject(const FString& Name, UObject* Object, bool bIsPermanent = true) const;

protected:
	/** UImmutableBaseBrowserWidget: @Interface @Begin */
	virtual void OnBrowserCreated() override;
	/** UImmutableBaseBrowserWidget: @Interface @End */

	/**
	 * Set up JavaScript bindings
	 * Override to bind custom objects
	 */
	virtual void SetupJavaScriptBindings();

protected:
	UPROPERTY(Transient)
	TObjectPtr<UImtblJSConnector> JSConnector;
};