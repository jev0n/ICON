#pragma once

//#include <Windows.h>
#include "pch.h"
//#include "interception.h"
//#include <stdio.h>
//#include <iostream>
//#include <thread>
//#include <chrono>
#include "MyForm1.h"
//#include "xor.h"

extern int UIMain();

using namespace std;

DWORD WINAPI Main_Thread(LPVOID lpParam)
{
    UIMain();
    return 1;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call) {
        case DLL_PROCESS_ATTACH:
            DisableThreadLibraryCalls(hModule);
            CreateThread(0, 0, Main_Thread, 0, 0, 0);
        case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH:
        case DLL_PROCESS_DETACH:
            interception_destroy_context(context);
            if (screenData) {
                free(screenData);
            }
            break;
    }

    return TRUE;
}