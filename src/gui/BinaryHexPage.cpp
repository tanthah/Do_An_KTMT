#include "BinaryHexPage.h"
#include "../common/Resource.h"
#include "../common/Constants.h"
#include "../validation/InputValidator.h"
#include "../bridge/ConverterBridge.h"

static HFONT hFontTitle = NULL;
static HFONT hFontLabel = NULL;
static HFONT hFontInput = NULL;

void ShowBinaryHex(HINSTANCE hInstance)
{
    HWND hwnd = CreateWindow(
        "BinaryHexClass",
        "Binary <-> Hex",
        WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT,
        900, 650,
        NULL, NULL,
        hInstance, NULL
    );

    // Center window
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    int x = (screenWidth - 900) / 2;
    int y = (screenHeight - 650) / 2;
    SetWindowPos(hwnd, NULL, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);
}

LRESULT CALLBACK BinaryHexProc(
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

            hFontLabel = CreateFont(18, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
                OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
                DEFAULT_PITCH | FF_SWISS, "Segoe UI");

            hFontInput = CreateFont(20, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
                OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
                DEFAULT_PITCH | FF_SWISS, "Segoe UI");

            // Page Title Box
            HWND hwndTitle = CreateWindow(
                "STATIC",
                "Binary <-> Hex",
                WS_CHILD | WS_VISIBLE | WS_BORDER | SS_CENTER | SS_CENTERIMAGE,
                250, 25, 400, 50,
                hwnd,
                NULL,
                NULL,
                NULL);
            if (hwndTitle) SendMessage(hwndTitle, WM_SETFONT, (WPARAM)hFontTitle, TRUE);

            // Container for Radio Buttons (thin border box)
            CreateWindow(
                "STATIC",
                "",
                WS_CHILD | WS_VISIBLE | WS_BORDER,
                175, 95, 550, 45,
                hwnd,
                NULL,
                NULL,
                NULL);

            // Radio Button 1 (Binary -> Hex)
            HWND hwndRadio1 = CreateWindow(
                "BUTTON",
                "Binary -> Hex",
                WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | WS_GROUP,
                210, 102, 200, 30,
                hwnd,
                (HMENU)ID_RAD_BIN_HEX,
                NULL,
                NULL);
            if (hwndRadio1) {
                SendMessage(hwndRadio1, WM_SETFONT, (WPARAM)hFontInput, TRUE);
                // Select by default
                SendMessage(hwndRadio1, BM_SETCHECK, BST_CHECKED, 0);
            }

            // Radio Button 2 (Hex -> Binary)
            HWND hwndRadio2 = CreateWindow(
                "BUTTON",
                "Hex -> Binary",
                WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
                490, 102, 200, 30,
                hwnd,
                (HMENU)ID_RAD_HEX_BIN,
                NULL,
                NULL);
            if (hwndRadio2) SendMessage(hwndRadio2, WM_SETFONT, (WPARAM)hFontInput, TRUE);

            // Input Label
            HWND hwndInputLabel = CreateWindow(
                "STATIC",
                "SO CAN CHUYEN DOI",
                WS_CHILD | WS_VISIBLE,
                175, 160, 550, 25,
                hwnd,
                NULL,
                NULL,
                NULL);
            if (hwndInputLabel) SendMessage(hwndInputLabel, WM_SETFONT, (WPARAM)hFontLabel, TRUE);

            // Input Edit Control
            HWND hwndInputEdit = CreateWindow(
                "EDIT",
                "",
                WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
                175, 185, 550, 40,
                hwnd,
                (HMENU)ID_EDIT_INPUT,
                NULL,
                NULL);
            if (hwndInputEdit) SendMessage(hwndInputEdit, WM_SETFONT, (WPARAM)hFontInput, TRUE);

            // Buttons
            HWND hwndBtnConvert = CreateWindow(
                "BUTTON",
                "CHUYEN DOI",
                WS_CHILD | WS_VISIBLE | WS_BORDER,
                175, 255, 170, 45,
                hwnd,
                (HMENU)ID_BTN_CONVERT,
                NULL,
                NULL);

            HWND hwndBtnClear = CreateWindow(
                "BUTTON",
                "XOA",
                WS_CHILD | WS_VISIBLE | WS_BORDER,
                365, 255, 170, 45,
                hwnd,
                (HMENU)ID_BTN_CLEAR,
                NULL,
                NULL);

            HWND hwndBtnBack = CreateWindow(
                "BUTTON",
                "QUAY LAI",
                WS_CHILD | WS_VISIBLE | WS_BORDER,
                555, 255, 170, 45,
                hwnd,
                (HMENU)ID_BTN_BACK,
                NULL,
                NULL);

            if (hwndBtnConvert) SendMessage(hwndBtnConvert, WM_SETFONT, (WPARAM)hFontLabel, TRUE);
            if (hwndBtnClear) SendMessage(hwndBtnClear, WM_SETFONT, (WPARAM)hFontLabel, TRUE);
            if (hwndBtnBack) SendMessage(hwndBtnBack, WM_SETFONT, (WPARAM)hFontLabel, TRUE);

            // Result Label
            HWND hwndResultLabel = CreateWindow(
                "STATIC",
                "KET QUA",
                WS_CHILD | WS_VISIBLE,
                175, 325, 550, 25,
                hwnd,
                NULL,
                NULL,
                NULL);
            if (hwndResultLabel) SendMessage(hwndResultLabel, WM_SETFONT, (WPARAM)hFontLabel, TRUE);

            // Result Edit Control (Multline, Readonly)
            HWND hwndResultEdit = CreateWindow(
                "EDIT",
                "",
                WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE | ES_READONLY | WS_VSCROLL,
                175, 350, 550, 180,
                hwnd,
                (HMENU)ID_EDIT_RESULT,
                NULL,
                NULL);
            if (hwndResultEdit) SendMessage(hwndResultEdit, WM_SETFONT, (WPARAM)hFontInput, TRUE);
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
            switch(LOWORD(wParam))
            {
            case ID_BTN_CONVERT:
                {
                    char inputBuf[1024] = {0};
                    GetDlgItemText(hwnd, ID_EDIT_INPUT, inputBuf, sizeof(inputBuf) - 1);
                    std::string inputStr(inputBuf);

                    // Determine conversion mode
                    bool binToHexMode = (SendMessage(GetDlgItem(hwnd, ID_RAD_BIN_HEX), BM_GETCHECK, 0, 0) == BST_CHECKED);

                    std::string resultStr;
                    if (binToHexMode)
                    {
                        if (IsBinary(inputStr.c_str()))
                        {
                            resultStr = ConvertBinToHex(inputStr);
                        }
                        else
                        {
                            resultStr = "Bao loi du lieu khong hop le";
                        }
                    }
                    else
                    {
                        if (IsHex(inputStr.c_str()))
                        {
                            resultStr = ConvertHexToBin(inputStr);
                        }
                        else
                        {
                            resultStr = "Bao loi du lieu khong hop le";
                        }
                    }

                    SetDlgItemText(hwnd, ID_EDIT_RESULT, resultStr.c_str());
                }
                break;

            case ID_BTN_CLEAR:
                SetDlgItemText(hwnd, ID_EDIT_INPUT, "");
                SetDlgItemText(hwnd, ID_EDIT_RESULT, "");
                break;

            case ID_BTN_BACK:
                DestroyWindow(hwnd);
                break;
            }
        }
        break;

    case WM_DESTROY:
        if (hFontTitle) DeleteObject(hFontTitle);
        if (hFontLabel) DeleteObject(hFontLabel);
        if (hFontInput) DeleteObject(hFontInput);

        // Show MainMenu window
        {
            HWND hwndMain = FindWindow("MainMenuClass", NULL);
            if (hwndMain)
            {
                ShowWindow(hwndMain, SW_SHOW);
            }
            else
            {
                PostQuitMessage(0);
            }
        }
        break;

    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}