#pragma once

#include <windows.h>

LRESULT CALLBACK BinaryHexProc(
    HWND hwnd,
    UINT msg,
    WPARAM wParam,
    LPARAM lParam
);

void ShowBinaryHex(HINSTANCE hInstance);