#pragma once

#include <windows.h>

LRESULT CALLBACK MainMenuProc(
    HWND hwnd,
    UINT msg,
    WPARAM wParam,
    LPARAM lParam
);

void ShowMainMenu(HINSTANCE hInstance);