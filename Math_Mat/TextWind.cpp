#include"TextWind.h"
#include"常用函数.hpp"
#include"MacroDefinition.h"
#include"pch.h"
namespace CMath {

	UINT TextShowWind::g_Text_Show_Window_Count = 0;

	TextShowWind::TextShowWind(HINSTANCE hInstance, HWND parent) :
		m_TextBKColor(RGB(200, 200, 200)), m_TextColor(RGB(0, 0, 0)), m_hIcon(nullptr)
	{
		if (hInstance == nullptr)
		{
			hInstance = GetModuleHandle(NULL);
		}
		m_hInstance = hInstance;
		g_Text_Show_Window_Count++;
		if (g_Text_Show_Window_Count == 1)
		{
			WNDCLASSEX wc = { 0 };

			// 设置窗口类结构体的各个参数
			wc.cbSize = sizeof(WNDCLASSEX);
			wc.style = CS_HREDRAW | CS_VREDRAW;
			wc.lpfnWndProc = this->WndProc;
			wc.hInstance = hInstance;
			wc.hCursor = LoadCursor(NULL, IDC_ARROW);
			wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
			wc.lpszClassName = G_TEXT_SHOW_WINDOW_CLASS_NAME;

			if (!RegisterClassEx(&wc))
			{
				std::cout << "文本显示窗口类注册失败" << std::endl;
			}
		}

		m_ControlhWnd = CreateWindowExW(
			WS_EX_WINDOWEDGE, G_TEXT_SHOW_WINDOW_CLASS_NAME, L"文本显示窗口",
			WS_CHILD | WS_VISIBLE,
			0, 0, 100, 100,
			parent, nullptr,
			hInstance, this
		);
		if (!m_ControlhWnd)
		{
			auto errorCode = GetLastError();
			char* errorMsg = nullptr;
			FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
				nullptr, errorCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), reinterpret_cast<LPSTR>(&errorMsg), 0, nullptr);
			std::cout << "文本显示窗口创建失败" << std::endl;
			std::cout << "错误代码：" << errorCode << std::endl;
			std::cout << errorMsg << std::endl;
			LocalFree(errorMsg);
		}
		m_EditWindow = CreateWindowExW(
			NULL, L"EDIT", NULL,
			WS_CHILD | WS_VISIBLE | WS_VSCROLL|ES_READONLY| ES_MULTILINE,
			0, 0, 100, 100,
			m_ControlhWnd, 0, hInstance, nullptr);
		if (!m_EditWindow)
		{
			GetSystemLastErrorClass Error;
			std::cout << "窗口创建失败" << std::endl;
			std::cout << "错误代码：" << Error.GetErrorCode() << std::endl;
			std::cout << Error.GetErrorMsg() << std::endl;
		}
		else {

			SendMessage(m_EditWindow, EM_FMTLINES, TRUE, 0);
			SendMessage(m_EditWindow, EM_LIMITTEXT, 0, 0);
		}
		m_hFont = CreateFontA(
			20,  // 字体高度
			0,   // 字体宽度，自动选择
			0,   // 无旋转
			0,   // 无旋转
			FW_NORMAL,  // 正常粗细
			FALSE,     // 非斜体
			FALSE,     // 无下划线
			FALSE,     // 无删除线
			ANSI_CHARSET,     // ANSI字符集
			OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY,  // 默认质量
			DEFAULT_PITCH,    // 默认字体类型
			"Arial"           // 字体名称
		);
		m_hBrush = CreateSolidBrush(RGB(200, 200, 200));
	}
	TextShowWind::~TextShowWind()
	{
		g_Text_Show_Window_Count--;
		CloseWindow(m_EditWindow);
		CloseWindow(m_ControlhWnd);
		if (g_Text_Show_Window_Count == 0)
		{
			UnregisterClass(G_TEXT_SHOW_WINDOW_CLASS_NAME, m_hInstance);
		}
		DeleteObject(m_hFont);
		DeleteObject(m_hBrush);
	}
	std::wstring TextShowWind::ReplaceEnter(const std::wstring& str)const
	{
		std::wstring wstr(str);
		size_t pos = wstr.find(L'\n');

		while (pos != wstr.npos)
		{
			wstr.replace(pos, 1, L"\r\n");
			pos = wstr.find(pos, L'\n');
		}
		return wstr;
	}
	void TextShowWind::MoveWind(int x, int y, int width, int Height)const
	{
		MoveWindow(m_ControlhWnd, x, y, width, Height, true);
		MoveWindow(m_EditWindow, 0, 0, width, Height, true);
		UpdateWind();
	}
	std::wstring TextShowWind::GetString()const
	{
		// 获取文本长度
		int len = (int)SendMessage(m_EditWindow, WM_GETTEXTLENGTH, 0, 0);

		// 创建缓冲区
		wchar_t* buffer = new wchar_t[len + 1];

		// 获取文本
		GetWindowText(m_EditWindow, buffer, len);

		// 转换为字符串
		std::wstring text(buffer);

		// 释放缓冲区内存
		delete[] buffer;

		return text;
	}
	void TextShowWind::AddString(const std::wstring& str)
	{

		// 获取编辑框的当前文本长度
		int nTextLength = GetWindowTextLength(m_EditWindow);

		// 将新文本追加到编辑框文本的末尾，并在末尾插入回车符
		SendMessage(m_EditWindow, EM_SETSEL, (WPARAM)nTextLength, (LPARAM)nTextLength);
		SendMessage(m_EditWindow, EM_REPLACESEL, 0, (LPARAM)ReplaceEnter(str).c_str());
	}
	void TextShowWind::SetString(const std::wstring& str)
	{
		SetWindowText(m_EditWindow, ReplaceEnter(str).c_str());
	}
	void TextShowWind::SetCueBanner(const std::wstring& str)
	{
		SendMessage(m_EditWindow, EM_SETCUEBANNER, true, (LPARAM)ReplaceEnter(str).c_str());
	}
	int TextShowWind::GetLineCharCount(int line)const
	{
		return SendMessage(m_EditWindow, EM_LINELENGTH, line, 0);
	}
	std::wstring TextShowWind::GetLineStr(int line)const
	{
		wchar_t* buffer = new wchar_t[GetLineCharCount(line) + 1];
		Edit_GetFileLine(m_EditWindow, line, buffer);
		std::wstring lineText(buffer);
		delete[] buffer;
		return lineText;
	}
	int TextShowWind::GetLineFormChar(int charIndex)const
	{
		return SendMessage(m_EditWindow, EM_LINEFROMCHAR, charIndex, 0);
	}
	int TextShowWind::GetLineStartIndex(int line)const
	{
		return SendMessage(m_EditWindow, EM_LINEINDEX,line, 0);
	}
	void TextShowWind::GetSelection(DWORD startPos, DWORD endPos)const
	{
		SendMessage(m_EditWindow, EM_GETSEL, (WPARAM)&startPos, (LPARAM)&endPos);
	}
	void TextShowWind::SetSelection(DWORD startPos, DWORD endPos)const
	{
		
		SendMessage(m_EditWindow, EM_SETSEL, (WPARAM)&startPos, (LPARAM)&endPos);
	}
	void TextShowWind::SetFont(LOGFONT font)
	{
		if (m_hFont)
			DeleteObject(m_hFont);
		m_hFont = CreateFontIndirect(&font);
		SendMessage(m_EditWindow, WM_SETFONT, (WPARAM)m_hFont, MAKELPARAM(TRUE, 0));
	}
	void TextShowWind::SetBKColor(COLORREF color)
	{
		m_TextBKColor = color;
		if (m_hBrush)
			DeleteObject(m_hBrush);
		m_hBrush = CreateSolidBrush(color);
		InvalidateRect(m_EditWindow, NULL, true);
	}
	void TextShowWind::SetFontColor(COLORREF color)
	{
		m_TextColor = color;
		InvalidateRect(m_EditWindow, NULL, true);
	}
	void TextShowWind::UpdateWind()const
	{
		InvalidateRect(m_EditWindow, NULL, true);
	}
	void TextShowWind::SetMaxCharCount(int count)const
	{
		SendMessage(m_EditWindow, EM_LIMITTEXT, count, 0);
	}
	LRESULT CALLBACK TextShowWind::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		static std::map<HWND, TextShowWind*>g_TextWindIndex;
		switch (message)
		{
		case WM_CTLCOLORSTATIC:
		{
			HDC hdcStatic = (HDC)wParam;
			HWND hWndStatic = (HWND)lParam;

			TextShowWind* TSW = g_TextWindIndex[hWnd];
			if(TSW)
			{
				SetTextColor(hdcStatic, TSW->m_TextColor);
				SetBkColor(hdcStatic, TSW->m_TextBKColor);
				return (LRESULT)TSW->m_hBrush;
			}

			break;
		}
		case WM_DESTROY:
		{
			TextShowWind* TSW = g_TextWindIndex[hWnd];
			if (TSW)
			{
			}
			PostQuitMessage(0);
			break;
		}
		case WM_CREATE:
		{
			CREATESTRUCT* pCreateStruct = reinterpret_cast<CREATESTRUCT*>(lParam);
			LPVOID lpData = pCreateStruct->lpCreateParams;
			TextShowWind* TSW = (TextShowWind*)(lpData);
			if (TSW)
			{
				g_TextWindIndex.insert(std::make_pair(hWnd, TSW));
			}
			break;
		}
		break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
			break;
		}
		return 0;
	}

	/**
	 * _ooOoo_
	 * o8888888o
	 * 88" . "88
	 * (| -_- |)
	 *  O\ = /O
	 * ___/`---'\____
	 * .   ' \\| |// `.
	 * / \\||| : |||// \
	 * / _||||| -:- |||||- \
	 * | | \\\ - /// | |
	 * | \_| ''\---/'' | |
	 * \ .-\__ `-` ___/-. /
	 * ___`. .' /--.--\ `. . __
	 * ."" '< `.___\_<|>_/___.' >'"".
	 * | | : `- \`.;`\ _ /`;.`/ - ` : | |
	 * \ \ `-. \_ __\ /__ _/ .-` / /
	 * ======`-.____`-.___\_____/___.-`____.-'======
	 * `=---='
	 *          .............................................
	 *           佛曰：bug泛滥，我已瘫痪！
	 */

	UINT TextInputWind::g_Text_Input_Window_Count = 0;
	TextInputWind::TextInputWind(HINSTANCE hInstance, HWND parent):
		m_ParentWind(parent),m_hInstance(hInstance),
		m_TextBKColor(RGB(200, 200, 200)), m_TextColor(RGB(0, 0, 0)), m_hIcon(nullptr)
	{
		if (hInstance == nullptr)
		{
			hInstance = GetModuleHandle(NULL);
		}
		m_hInstance = hInstance;
		g_Text_Input_Window_Count++;
		if (g_Text_Input_Window_Count == 1)
		{
			WNDCLASSEX wc = { 0 };

			// 设置窗口类结构体的各个参数
			wc.cbSize = sizeof(WNDCLASSEX);
			wc.style = CS_HREDRAW | CS_VREDRAW;
			wc.lpfnWndProc = this->WndProc;
			wc.hInstance = hInstance;
			wc.hCursor = LoadCursor(NULL, IDC_ARROW);
			wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
			wc.lpszClassName = G_TEXT_INPUT_WINDOW_CLASS_NAME;

			if (!RegisterClassEx(&wc))
			{
				std::cout << "文本输入窗口类注册失败" << std::endl;
				GetSystemLastErrorClass error;
				std::cout << "错误代码：" << error.GetErrorCode() << std::endl;
				std::cout << error.GetErrorMsg() << std::endl;
			}
		}

		m_ControlWind = CreateWindowExW(
			WS_EX_WINDOWEDGE, G_TEXT_INPUT_WINDOW_CLASS_NAME, L"文本输入窗口",
			WS_CHILD | WS_VISIBLE,
			0, 0, 100, 100,
			parent, nullptr,
			hInstance, this
		);
		if (!m_ControlWind)
		{
			auto errorCode = GetLastError();
			char* errorMsg = nullptr;
			FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
				nullptr, errorCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), reinterpret_cast<LPSTR>(&errorMsg), 0, nullptr);
			std::cout << "文本输入窗口创建失败" << std::endl;
			std::cout << "错误代码：" << errorCode << std::endl;
			std::cout << errorMsg << std::endl;
			LocalFree(errorMsg);
		}
		m_EditWind = CreateWindowExW(
			NULL, L"EDIT", NULL,
			WS_CHILD | WS_VISIBLE| ES_MULTILINE|WS_VSCROLL,
			0, 0, 100, 100,
			m_ControlWind, (HMENU)G_INPUT_WINDOW_EDIT_WINDOW_ID, hInstance, nullptr);

		m_hFont = CreateFontA(
			20,  // 字体高度
			0,   // 字体宽度，自动选择
			0,   // 无旋转
			0,   // 无旋转
			FW_NORMAL,  // 正常粗细
			FALSE,     // 非斜体
			FALSE,     // 无下划线
			FALSE,     // 无删除线
			ANSI_CHARSET,     // ANSI字符集
			OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY,  // 默认质量
			DEFAULT_PITCH,    // 默认字体类型
			"Arial"           // 字体名称
		);
		m_hBrush = CreateSolidBrush(RGB(200, 200, 200));
	}
	TextInputWind::~TextInputWind()
	{
		g_Text_Input_Window_Count--;
		CloseWindow(m_EditWind);
		CloseWindow(m_ControlWind);
		if (g_Text_Input_Window_Count == 0)
		{
			UnregisterClass(G_TEXT_SHOW_WINDOW_CLASS_NAME, m_hInstance);
		}
		DeleteObject(m_hFont);
		DeleteObject(m_hBrush);
	}
	void TextInputWind::MoveWind(int x, int y, int width, int Height)const
	{
		MoveWindow(m_ControlWind, x, y, width, Height, true);
		MoveWindow(m_EditWind, 0, 0, width, Height, true);
		UpdateWind();
	}

	const std::wstring& TextInputWind::GetString()const
	{
		return m_InputStr;
	}

	void TextInputWind::SetFont(LOGFONT logFont)
	{
		if (m_hFont)
		{
			DeleteObject(m_hFont);
		}
		m_hFont = CreateFontIndirect(&logFont);
		SendMessage(m_EditWind, WM_SETFONT, (WPARAM)m_hFont, MAKELPARAM(TRUE, 0));
	}

	void TextInputWind::SetBKColor(COLORREF color)
	{
		m_TextBKColor = color;
		if (m_hBrush)
			DeleteObject(m_hBrush);
		m_hBrush = CreateSolidBrush(color);
		InvalidateRect(m_EditWind, NULL, true);
	}

	void TextInputWind::SetFontColor(COLORREF color)
	{
		m_TextColor = color;
		InvalidateRect(m_EditWind, NULL, true);
	}

	void TextInputWind::UpdateWind()const
	{
		InvalidateRect(m_ControlWind, NULL, true);
		InvalidateRect(m_EditWind, NULL, true);
	}
	LRESULT CALLBACK TextInputWind::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		static std::map<HWND, TextInputWind*>g_TextWindIndex;
		switch (message)
		{
		case WM_COMMAND:
		{
			switch (LOWORD(wParam))
			{
			case WindowsID::G_INPUT_WINDOW_EDIT_WINDOW_ID:
			{
				switch (HIWORD(wParam))
				{
				case EN_UPDATE:
				{
					TextInputWind* TIW = g_TextWindIndex[hWnd];
					if (!TIW)
					{
						break;
					}
					const int maxStrSize = 256;
					WCHAR* buffur = new WCHAR[maxStrSize];

					if (Edit_GetText((HWND)lParam, buffur, maxStrSize))
					{

						TIW->m_InputStr = buffur;

						if (TIW->m_InputStr.find_last_of(L"\r\n") != std::wstring::npos)
						{
							SendMessage(GetParent(hWnd), InputWindowMessage::IWM_USER_ENTER_INPUT, (WPARAM)TIW->m_InputStr.c_str(), TIW->m_InputStr.size());
							SetWindowText((HWND)lParam, L"");
						}
						SendMessage(GetParent(hWnd), InputWindowMessage::IWM_USER_INPUT, (WPARAM)TIW->m_InputStr.c_str(), TIW->m_InputStr.size());
					}
					delete[] buffur;

					break;
				}
				default:
					return DefWindowProc(hWnd, message, wParam, lParam);
					break;
				}
				break;
			}
			default:
				return DefWindowProc(hWnd, message, wParam, lParam);
				break;
			}
			break;
		}
		case WM_CTLCOLOREDIT:
		{
			HDC hdcStatic = (HDC)wParam;
			HWND hWndStatic = (HWND)lParam;

			TextInputWind* TIW = g_TextWindIndex[hWnd];
			if (TIW)
			{
				SetTextColor(hdcStatic, TIW->m_TextColor);
				SetBkColor(hdcStatic, TIW->m_TextBKColor);
				return (LRESULT)TIW->m_hBrush;
			}

			break;
		}
		case WM_DESTROY:
		{
			TextInputWind* TIW = g_TextWindIndex[hWnd];
			if (TIW)
			{
			}
			PostQuitMessage(0);
			break;
		}
		case WM_CREATE:
		{
			CREATESTRUCT* pCreateStruct = reinterpret_cast<CREATESTRUCT*>(lParam);
			LPVOID lpData = pCreateStruct->lpCreateParams;
			TextInputWind* TIW = (TextInputWind*)(lpData);
			if (TIW)
			{
				g_TextWindIndex.insert(std::make_pair(hWnd, TIW));
			}
			break;
		}
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
			break;
		}
		return 0;
	}
}