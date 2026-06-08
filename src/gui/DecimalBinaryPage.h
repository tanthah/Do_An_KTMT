#pragma once

#include <windows.h>

LRESULT CALLBACK DecimalBinaryProc(
    HWND hwnd,
    UINT msg,
    WPARAM wParam,
    LPARAM lParam
);

void ShowDecimalBinary(HINSTANCE hInstance);