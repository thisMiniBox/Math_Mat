#pragma once
#include<Windows.h>
#include"TextWind.h"
namespace CMath {
	constexpr auto G_MC_WINDOW_CLASS_NAME = L"Math_Controller_Main_Window";
	class MathController
	{
		static UINT g_MC_Count;
		static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

		int TextShowWindHeight = 30;
		HWND m_MainWind_hWnd;
		HINSTANCE m_hInstance;
		HACCEL m_hAccelTable;
		MSG m_Msg;
		TextShowWind* m_TextShow;
		TextInputWind* m_TextInput;
	public:
		MathController(HINSTANCE hInstance = nullptr);
		bool Run();
		~MathController();
	};
}
