#pragma once

#include <windows.h>

LRESULT CALLBACK DecimalHexProc(
    HWND hwnd,
    UINT msg,
    WPARAM wParam,
    LPARAM lParam
);

void ShowDecimalHex(HINSTANCE hInstance);