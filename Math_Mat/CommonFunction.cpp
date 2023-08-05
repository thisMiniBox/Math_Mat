#include"常用函数.hpp"

namespace CMath {


    //获取错误如窗口创建失败错误
    //要使用LocalFree(errorMsg)清理内存
    void GetSystemLastError(DWORD* ErrorCode, CHAR** ErrorMsg)
    {
        *ErrorCode = GetLastError();
        FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            nullptr, *ErrorCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), reinterpret_cast<LPSTR>(ErrorMsg), 0, nullptr);
    }
    //获取错误如窗口创建失败错误
    //要使用LocalFree(errorMsg)清理内存
    void GetSystemLastError(DWORD* ErrorCode, WCHAR** ErrorMsg)
    {
        *ErrorCode = GetLastError();
        FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            nullptr, *ErrorCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), reinterpret_cast<LPWSTR>(ErrorMsg), 0, nullptr);
    }
    LOGFONT ChooseLogFont_g(HWND hWnd)
    {
        // 显示字体选择对话框
        CHOOSEFONT chooseFont;
        LOGFONT logFont;

        ZeroMemory(&chooseFont, sizeof(CHOOSEFONT));
        ZeroMemory(&logFont, sizeof(LOGFONT));

        chooseFont.lStructSize = sizeof(CHOOSEFONT);
        chooseFont.Flags = CF_EFFECTS | CF_SCREENFONTS | CF_INITTOLOGFONTSTRUCT;
        chooseFont.hwndOwner = hWnd;
        chooseFont.lpLogFont = &logFont;
        chooseFont.nFontType = SCREEN_FONTTYPE;
        ChooseFontW(&chooseFont);
        return logFont;
    }
    COLORREF ChooseCustomColor(HWND hwnd)
    {
        CHOOSECOLOR cc;                 // 颜色选择对话框结构
        static COLORREF customColors[16];  // 自定义颜色数组

        // 初始化CHOOSECOLOR结构
        ZeroMemory(&cc, sizeof(cc));
        cc.lStructSize = sizeof(cc);
        cc.hwndOwner = hwnd;
        cc.lpCustColors = (LPDWORD)customColors;
        cc.Flags = CC_FULLOPEN | CC_RGBINIT;

        // 显示颜色选择对话框
        if (ChooseColor(&cc))
        {
            return cc.rgbResult;  // 返回选择的颜色值
        }

        return RGB(0, 0, 0);   // 如果取消选择，返回黑色
    }
}