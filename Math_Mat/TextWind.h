#pragma once
#include<Windows.h>
#include<string>
namespace CMath{
	constexpr auto G_TEXT_SHOW_WINDOW_CLASS_NAME = L"Text_Show_Window";
	class TextShowWind
	{
		static UINT g_Text_Show_Window_Count;
		static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

		HINSTANCE m_hInstance;
		HWND m_ControlhWnd;
		HWND m_EditWindow;
		HBRUSH m_hBrush;
		COLORREF m_TextColor;
		COLORREF m_TextBKColor;

		HFONT m_hFont;

		HICON m_hIcon;

		std::wstring ReplaceEnter(const std::wstring& str)const;
	public:

		TextShowWind(HINSTANCE hInstance, HWND parent);
		~TextShowWind();
		void MoveWind(int x, int y, int width, int Height)const;
		//设置最大字符数
		void SetMaxCharCount(int count)const;
		//获取所有字符
		std::wstring GetString()const;
		//设置字符串
		void SetString(const std::wstring& str);
		//设置默认横幅
		void SetCueBanner(const std::wstring& str);

		void AddString(const std::wstring& str);
		//获取指定行的长度
		int GetLineCharCount(int line)const;
		//获取指定行字符串
		std::wstring GetLineStr(int line)const;
		//通过字符位置获取行索引
		int GetLineFormChar(int charIndex)const;
		//通过行号获取行起始字符索引
		int GetLineStartIndex(int line)const;

		void GetSelection(DWORD startPos, DWORD endPos)const;

		void SetSelection(DWORD startPos, DWORD endPos)const;

		void SetFont(LOGFONT logFont);

		void SetBKColor(COLORREF color);

		void SetFontColor(COLORREF color);

		void UpdateWind()const;


	};
	constexpr auto G_TEXT_INPUT_WINDOW_CLASS_NAME = L"Text_Input_Window";
	class TextInputWind
	{
		static UINT g_Text_Input_Window_Count;
		static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

		HWND m_ControlWind;
		HWND m_EditWind;
		HWND m_ParentWind;

		HINSTANCE m_hInstance;

		HBRUSH m_hBrush;
		COLORREF m_TextColor;
		COLORREF m_TextBKColor;

		HFONT m_hFont;

		HICON m_hIcon;

		std::wstring m_InputStr;
	public:
		//发送给父窗口的消息
		enum InputWindowMessage
		{
			//用户确认输入（在单行文本中输入回车确定）
			//多行文本中输入回车后发送
			//WPARAM为wchar_t*,表示输入的文本
			//LPARAM为字符串长度
			IWM_USER_ENTER_INPUT=WM_USER+1,
			//每次输入时更新
			//WPARAM为wchar_t*,表示输入的文本
			//LPARAM为字符串长度
			IWM_USER_INPUT,

		};
		TextInputWind(HINSTANCE hInstance, HWND parent);
		~TextInputWind();

		void MoveWind(int x, int y, int width, int Height)const;

		const std::wstring& GetString()const;

		void SetFont(LOGFONT logFont);

		void SetBKColor(COLORREF color);

		void SetFontColor(COLORREF color);

		void UpdateWind()const;
	};
}

