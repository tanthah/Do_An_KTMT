#include "MainMenu.h"
#include "DecimalBinaryPage.h"
#include "DecimalHexPage.h"
#include "BinaryHexPage.h"
#include "../common/Resource.h"
#include "../common/Constants.h"

static HFONT hFontTitle = NULL;
static HFONT hFontButton = NULL;

void ShowMainMenu(HINSTANCE hInstance)
{
    (void)hInstance;
    HWND hwnd = FindWindow("MainMenuClass", NULL);
    if (hwnd)
    {
        ShowWindow(hwnd, SW_SHOW);
    }
}

LRESULT CALLBACK MainMenuProc(
    HWND hwnd,
    UINT msg,
    WPARAM wParam,
    LPARAM lParam)
{
    switch(msg)
    {
    case WM_CREATE:
        {
            // Create fonts
            hFontTitle = CreateFont(28, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
                OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
                DEFAULT_PITCH | FF_SWISS, "Segoe UI");

            hFontButton = CreateFont(20, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
                OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
                DEFAULT_PITCH | FF_SWISS, "Segoe UI");

            // Title Box: Static control with border
            HWND hwndTitle = CreateWindow(
                "STATIC",
                "HE THONG CHUYEN DOI HE CO SO",
                WS_CHILD | WS_VISIBLE | WS_BORDER | SS_CENTER | SS_CENTERIMAGE,
                200, 60, 500, 60,
                hwnd,
                NULL,
                NULL,
                NULL);
            if (hwndTitle) {
                SendMessage(hwndTitle, WM_SETFONT, (WPARAM)hFontTitle, TRUE);
            }

            // Buttons
            HWND hwndBtn1 = CreateWindow(
                "BUTTON",
                "Decimal <-> Binary",
                WS_CHILD | WS_VISIBLE | WS_BORDER,
                250, 180, 400, 50,
                hwnd,
                (HMENU)ID_BTN_DEC_BIN,
                NULL,
                NULL);
            
            HWND hwndBtn2 = CreateWindow(
                "BUTTON",
                "Decimal <-> Hex",
                WS_CHILD | WS_VISIBLE | WS_BORDER,
                250, 260, 400, 50,
                hwnd,
                (HMENU)ID_BTN_DEC_HEX,
                NULL,
                NULL);

            HWND hwndBtn3 = CreateWindow(
                "BUTTON",
                "Binary <-> Hex",
                WS_CHILD | WS_VISIBLE | WS_BORDER,
                250, 340, 400, 50,
                hwnd,
                (HMENU)ID_BTN_BIN_HEX,
                NULL,
                NULL);

            HWND hwndBtn4 = CreateWindow(
                "BUTTON",
                "Thoat",
                WS_CHILD | WS_VISIBLE | WS_BORDER,
                250, 420, 400, 50,
                hwnd,
                (HMENU)ID_BTN_EXIT,
                NULL,
                NULL);

            if (hwndBtn1) SendMessage(hwndBtn1, WM_SETFONT, (WPARAM)hFontButton, TRUE);
            if (hwndBtn2) SendMessage(hwndBtn2, WM_SETFONT, (WPARAM)hFontButton, TRUE);
            if (hwndBtn3) SendMessage(hwndBtn3, WM_SETFONT, (WPARAM)hFontButton, TRUE);
            if (hwndBtn4) SendMessage(hwndBtn4, WM_SETFONT, (WPARAM)hFontButton, TRUE);
        }
        break;

    case WM_CTLCOLORSTATIC:
        {
            HDC hdc = (HDC)wParam;
            SetTextColor(hdc, RGB(0, 0, 0));
            SetBkColor(hdc, RGB(255, 255, 255));
            return (INT_PTR)GetStockObject(WHITE_BRUSH);
        }

    case WM_COMMAND:
        {
            HINSTANCE hInstance = (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE);
            switch(LOWORD(wParam))
            {
            case ID_BTN_DEC_BIN:
                ShowWindow(hwnd, SW_HIDE);
                ShowDecimalBinary(hInstance);
                break;

            case ID_BTN_DEC_HEX:
                ShowWindow(hwnd, SW_HIDE);
                ShowDecimalHex(hInstance);
                break;

            case ID_BTN_BIN_HEX:
                ShowWindow(hwnd, SW_HIDE);
                ShowBinaryHex(hInstance);
                break;

            case ID_BTN_EXIT:
                DestroyWindow(hwnd);
                break;
            }
        }
        break;

    case WM_DESTROY:
        // Clean up fonts
        if (hFontTitle) DeleteObject(hFontTitle);
        if (hFontButton) DeleteObject(hFontButton);
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}