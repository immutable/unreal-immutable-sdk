package com.immutable.unreal;

import android.app.Activity;
import android.content.ComponentName;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.content.pm.ResolveInfo;
import android.graphics.Insets;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.util.DisplayMetrics;
import android.view.WindowInsets;
import android.view.WindowMetrics;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.browser.customtabs.CustomTabsCallback;
import androidx.browser.customtabs.CustomTabsClient;
import androidx.browser.customtabs.CustomTabsIntent;
import androidx.browser.customtabs.CustomTabsService;
import androidx.browser.customtabs.CustomTabsServiceConnection;
import androidx.browser.customtabs.CustomTabsSession;

import java.util.ArrayList;
import java.util.List;

public class ImmutableAndroid {
    private static CustomTabsServiceConnection customTabsServiceConnection;

    private static int getCustomTabsHeight(Activity context) {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.R) {
            WindowMetrics windowMetrics = context.getWindowManager().getCurrentWindowMetrics();
            Insets insets = windowMetrics.getWindowInsets()
                    .getInsetsIgnoringVisibility(WindowInsets.Type.systemBars());
            return windowMetrics.getBounds().height() - insets.top - insets.bottom;
        } else {
            DisplayMetrics displayMetrics = new DisplayMetrics();
            context.getWindowManager().getDefaultDisplay().getMetrics(displayMetrics);
            return displayMetrics.heightPixels;
        }
    }

    public static void launchUrl(final Activity context, final String url) {
        // Get all apps that can support Custom Tabs Service
        // i.e. services that can handle ACTION_CUSTOM_TABS_CONNECTION intents
        PackageManager packageManager = context.getPackageManager();
        Intent serviceIntent = new Intent();
        serviceIntent.setAction(CustomTabsService.ACTION_CUSTOM_TABS_CONNECTION);
        List<ResolveInfo> resolvedList = packageManager.queryIntentServices(serviceIntent, 0);
        List<String> packageNames = new ArrayList<>();
        for (ResolveInfo info : resolvedList) {
            if (info.serviceInfo != null) {
                packageNames.add(info.serviceInfo.packageName);
            }
        }

        if (packageNames.size() > 0) {
            // Get the preferred Custom Tabs package
            String customTabsPackageName = CustomTabsClient.getPackageName(context, packageNames);
            if (customTabsPackageName == null) {
                // Could not get the preferred Custom Tab browser, so launch URL in any browser
                context.startActivity(new Intent(Intent.ACTION_VIEW, Uri.parse(url)));
            } else {
                customTabsServiceConnection = new CustomTabsServiceConnection() {
                    @Override
                    public void onServiceDisconnected(ComponentName name) {
                        customTabsServiceConnection = null;
                    }

                    @Override
                    public void onCustomTabsServiceConnected(@NonNull ComponentName name, @NonNull CustomTabsClient client) {
                        CustomTabsSession session = client.newSession(new CustomTabsCallback() {
                            @Override
                            public void onNavigationEvent(int navigationEvent, @Nullable Bundle extras) {
                                if (context instanceof Callback && navigationEvent == CustomTabsCallback.TAB_HIDDEN) {
                                    // Adding some delay before calling onCustomTabsDismissed as sometimes this gets called 
                                    // before the PKCE deeplink is triggered (by 100ms). This means PKCEResponseDelegate will be 
                                    // set to null before the SDK can use it to notify the consumer of the PKCE result.
                                    // See UImmutablePassport::HandleOnPKCEDismissed and UImmutablePassport::OnDeepLinkActivated
                                    final Handler handler = new Handler(Looper.getMainLooper());
                                    handler.postDelayed(new Runnable() {
                                        @Override
                                        public void run() {
                                            ((Callback) context).onCustomTabsDismissed(url);
                                        }
                                    }, 1000);
                                }
                            }
                        });
                        // Need to set the session to get custom tabs to show as a bottom sheet
                        CustomTabsIntent customTabsIntent = new CustomTabsIntent.Builder(session)
                                .setInitialActivityHeightPx(getCustomTabsHeight(context))
                                .setUrlBarHidingEnabled(true)
                                .build();
                        customTabsIntent.launchUrl(context, Uri.parse(url));
                    }
                };
                CustomTabsClient.bindCustomTabsService(context, customTabsPackageName, customTabsServiceConnection);
            }
        } else {
            // Custom Tabs not supported by any browser on the device so launch URL in any browser
            context.startActivity(new Intent(Intent.ACTION_VIEW, Uri.parse(url)));
        }
    }

    public interface Callback {
        void onCustomTabsDismissed(String url);
    }
}