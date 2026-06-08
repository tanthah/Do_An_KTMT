#include <windows.h>
#include "../include/validators.h"
#include "../include/conversions.h"
#include "gui/MainMenu.h"
#include "gui/DecimalBinaryPage.h"
#include "gui/DecimalHexPage.h"
#include "gui/BinaryHexPage.h"

// Window classes
const char g_szMainMenuClass[] = "MainMenuClass";
const char g_szDecimalBinaryClass[] = "DecimalBinaryClass";
const char g_szDecimalHexClass[] = "DecimalHexClass";
const char g_szBinaryHexClass[] = "BinaryHexClass";

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    (void)hPrevInstance;
    (void)lpCmdLine;
    WNDCLASSEX wc;
    HWND hwnd;
    MSG Msg;

    // Register MainMenuClass
    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.style         = 0;
    wc.lpfnWndProc   = MainMenuProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = hInstance;
    wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = g_szMainMenuClass;
    wc.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);
    if(!RegisterClassEx(&wc)) {
        MessageBox(NULL, "Window Registration Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    // Register DecimalBinaryClass
    wc.lpfnWndProc   = DecimalBinaryProc;
    wc.lpszClassName = g_szDecimalBinaryClass;
    RegisterClassEx(&wc);

    // Register DecimalHexClass
    wc.lpfnWndProc   = DecimalHexProc;
    wc.lpszClassName = g_szDecimalHexClass;
    RegisterClassEx(&wc);

    // Register BinaryHexClass
    wc.lpfnWndProc   = BinaryHexProc;
    wc.lpszClassName = g_szBinaryHexClass;
    RegisterClassEx(&wc);

    // Create Main Menu Window
    hwnd = CreateWindowEx(
        0,
        g_szMainMenuClass,
        "HE THONG CHUYEN DOI HE CO SO",
        WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT, 900, 650,
        NULL, NULL, hInstance, NULL);

    if(hwnd == NULL) {
        MessageBox(NULL, "Window Creation Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    // Center window
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    int x = (screenWidth - 900) / 2;
    int y = (screenHeight - 650) / 2;
    SetWindowPos(hwnd, NULL, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    // Message Loop
    while(GetMessage(&Msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }
    return Msg.wParam;
}