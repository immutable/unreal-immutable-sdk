// Copyright Epic Games, Inc. All Rights Reserved.

#include "ImmutableModule.h"

#include "Immutable/Misc/ImtblLogging.h"
#include "Interfaces/IPluginManager.h"
#if PLATFORM_WINDOWS
#include "Windows/AllowWindowsPlatformTypes.h"
#include <windows.h>
#include <psapi.h>
#include "Windows/HideWindowsPlatformTypes.h"
#endif

#define LOCTEXT_NAMESPACE "FImmutableModule"

void FImmutableModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact
	// timing is specified in the .uplugin file per-module

#if USING_BLUI_CEF
  if (IPluginManager::Get().FindPlugin("WebBrowserWidget") != nullptr &&
      IPluginManager::Get().FindPlugin("WebBrowserWidget")->IsEnabled()) {
    IMTBL_ERR("Cannot enable both BLUI and WebBrowserWidget plugin at the same "
              "time as it crashes.  In Immutable.uplugin file, "
              "'Plugins->WebBrowserWidget->Enabled' to 'false' and ensure the "
              "WebBrowserWidget is disabled in your project to use BLUI.")
  }
#endif
}

void FImmutableModule::ShutdownModule()
{
#if PLATFORM_WINDOWS && USING_BLUI_CEF
    DWORD aProcesses[1024], cbNeeded;
    unsigned int i;

    if (EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))
    {
        // Calculate how many process identifiers were returned.
        DWORD cProcesses = cbNeeded / sizeof(DWORD);

        // Print the name and process identifier for each process.
        for (i = 0; i < cProcesses; i++)
        {
            if (aProcesses[i] != 0)
            {
                TCHAR szProcessName[MAX_PATH] = TEXT("<unknown>");
                // Get a handle to the process.
                HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, 0, aProcesses[i]);
                DWORD errCode = GetLastError();

                IMTBL_LOG("PID %d : ERROR %d ", aProcesses[i], errCode);
                // Get the process name.
                if (errCode == 0 && hProcess != NULL)
                {
                    HMODULE hMod;

                    cbNeeded = 0;
                    if (EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeeded))
                    {
                        if (GetModuleBaseName(hProcess, hMod, szProcessName, sizeof(szProcessName) / sizeof(TCHAR)))
                        {
                            if (!_tcscmp(szProcessName, _T("blu_ue4_process.exe")))
                            {
                                if (TerminateProcess(hProcess, 0) == 0)
                                {
                                    IMTBL_ERR("Faild to shutdown BLUI executable process");
                                }
                                else
                                {
                                    IMTBL_LOG("BLUI executable process terminated");
                                }
                            }
                        }
                    }
                    errCode = GetLastError();

                    // Release the handle to the process.
                    CloseHandle(hProcess);
                }
            }
        }
    }
    
#endif
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FImmutableModule, Immutable)
