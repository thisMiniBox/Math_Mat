#include"MathController.h"
#include"resource.h"
#include"常用函数.hpp"
#include"pch.h"
namespace CMath {
	UINT MathController::g_MC_Count = 0;
	MathController::MathController(HINSTANCE hInstance) :
		m_Msg({ 0 })
	{
		if (hInstance == nullptr)
		{
			hInstance = GetModuleHandle(NULL);
		}
		m_hInstance = hInstance;
		if (g_MC_Count == 0)
		{
			WNDCLASSEX wc = { 0 };

			// 设置窗口类结构体的各个参数
			wc.cbSize = sizeof(WNDCLASSEX);
			wc.style = CS_HREDRAW | CS_VREDRAW;
			wc.lpfnWndProc = this->WndProc;
			wc.hInstance = hInstance;
			wc.lpszMenuName = MAKEINTRESOURCEW(IDR_MATH_WINDOW_MENU);
			wc.hCursor = LoadCursor(NULL, IDC_ARROW);
			wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
			wc.lpszClassName = G_MC_WINDOW_CLASS_NAME;

			if (!RegisterClassEx(&wc))
			{
				std::cout << "数学控制器窗口类注册失败" << std::endl;
			}
		}
		int screenWidth = GetSystemMetrics(SM_CXSCREEN);
		int screenHeight = GetSystemMetrics(SM_CYSCREEN);
		int windWidth = 800;
		int windHeight = 600;
		m_MainWind_hWnd = CreateWindowExW(
			WS_EX_WINDOWEDGE, G_MC_WINDOW_CLASS_NAME, L"数学窗口",
			WS_OVERLAPPEDWINDOW,
			(screenWidth - windWidth) / 2, (screenHeight - windHeight) / 2,
			windWidth, windHeight,
			nullptr, nullptr,
			hInstance, this
		);
		if (!m_MainWind_hWnd)
		{
			auto errorCode = GetLastError();
			char* errorMsg = nullptr;
			FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
				nullptr, errorCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), reinterpret_cast<LPSTR>(&errorMsg), 0, nullptr);

			std::cout << "窗口创建失败" << std::endl;
			std::cout << "错误代码：" << errorCode << std::endl;
			std::cout << errorMsg << std::endl;
		}
		else
		{
			ShowWindow(m_MainWind_hWnd, SW_SHOW);
		}
		g_MC_Count++;
		
		RECT rect = {};
		if (m_MainWind_hWnd)
		{
			GetClientRect(m_MainWind_hWnd, &rect);
		}
		windWidth = rect.right;
		windHeight = rect.bottom;
		m_TextShow = new TextShowWind(hInstance, m_MainWind_hWnd);

		if (!m_TextShow)
		{
			std::cout << "文本显示窗口创建失败" << std::endl;
		}
		else
		{
			m_TextShow->MoveWind(0, 0, windWidth, windHeight - TextShowWindHeight);
		}
		m_TextInput = new TextInputWind(hInstance, m_MainWind_hWnd);
		if (!m_TextInput)
		{
			std::cout << "文本输入窗口创建失败" << std::endl;
		}
		else
		{
			m_TextInput->MoveWind(0, windHeight - TextShowWindHeight, windWidth, TextShowWindHeight);
		}
		m_hAccelTable = LoadAccelerators(m_hInstance, MAKEINTRESOURCE(IDR_MATH_ACC));
	}
	bool MathController::Run()
	{
		if (!GetMessage(&m_Msg, nullptr, 0, 0))
			return false;
		if (!TranslateAccelerator(m_Msg.hwnd, m_hAccelTable, &m_Msg))
		{
			TranslateMessage(&m_Msg);
			DispatchMessage(&m_Msg);
		}
		return true;
	}
	MathController::~MathController()
	{
		g_MC_Count--;
		CloseWindow(m_MainWind_hWnd);
		if (g_MC_Count == 0)
		{
			UnregisterClass(G_MC_WINDOW_CLASS_NAME, m_hInstance);
		}
		if (m_TextShow)
		{
			delete m_TextShow;
		}
	}
	LRESULT CALLBACK MathController::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		static std::map<HWND, MathController*>g_WindowIndex;
		switch (message)
		{
		case TextInputWind::InputWindowMessage::IWM_USER_ENTER_INPUT:
		{
			MathController* mine = g_WindowIndex[hWnd];
			if (!mine)
				break;
			mine->m_TextShow->AddString((WCHAR*)wParam);
			break;
		}
		case WM_COMMAND:
		{
			MathController* mine = g_WindowIndex[hWnd];
			if (!mine)
				break;
			switch (LOWORD(wParam))
			{
			case ID_MAIN_WINDOW_BK_COLOR:
			{
				mine->m_TextShow->SetBKColor(ChooseCustomColor(hWnd));
				break;
			}
			case ID_MAIN_WINDOW_FONT:
			{
				auto lfont = ChooseLogFont_g(hWnd);
				mine->m_TextShow->SetFont(lfont);
				int lfHeight = lfont.lfHeight;
				HDC hdc;
				hdc = GetDC(hWnd);

				if (lfHeight <= 0) 
				{
					int dpiY = GetDeviceCaps(hdc, LOGPIXELSY);
					lfHeight= -MulDiv(lfHeight, dpiY, 72);
				}
				mine->TextShowWindHeight = lfHeight + 20;

				ReleaseDC(hWnd, hdc);
				RECT rect;
				GetClientRect(hWnd, &rect);
				LPARAM value = ((LPARAM)rect.bottom << 32) | (LPARAM)rect.right;
				SendMessage(hWnd, WM_SIZE, 0, value);
				
				break;
			}
			case ID_MAIN_WINDOW_FONT_COLOR:
			{
				mine->m_TextShow->SetFontColor(ChooseCustomColor(hWnd));
				break;
			}
			}
			break;
		}
		case WM_SIZE:
		{
			MathController* mine = g_WindowIndex[hWnd];
			auto w = LOWORD(lParam);
			auto h = HIWORD(lParam);
			if (mine && mine->m_TextShow)
			{
				mine->m_TextInput->MoveWind(0, h - mine->TextShowWindHeight, w, mine->TextShowWindHeight);
				mine->m_TextShow->MoveWind(0, 0, w, h - mine->TextShowWindHeight);
			}

			break;
		}
		case WM_DESTROY:
		{
			PostQuitMessage(0);
		}
		break;


		case WM_CREATE:
		{

			CREATESTRUCT* pCreateStruct = reinterpret_cast<CREATESTRUCT*>(lParam);
			LPVOID lpData = pCreateStruct->lpCreateParams;
			MathController* MC = static_cast<MathController*>(lpData);
			if (MC)
			{
				g_WindowIndex.insert(std::make_pair(hWnd, MC));
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