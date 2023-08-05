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
		//��������ַ���
		void SetMaxCharCount(int count)const;
		//��ȡ�����ַ�
		std::wstring GetString()const;
		//�����ַ���
		void SetString(const std::wstring& str);
		//����Ĭ�Ϻ��
		void SetCueBanner(const std::wstring& str);

		void AddString(const std::wstring& str);
		//��ȡָ���еĳ���
		int GetLineCharCount(int line)const;
		//��ȡָ�����ַ���
		std::wstring GetLineStr(int line)const;
		//ͨ���ַ�λ�û�ȡ������
		int GetLineFormChar(int charIndex)const;
		//ͨ���кŻ�ȡ����ʼ�ַ�����
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
		//���͸������ڵ���Ϣ
		enum InputWindowMessage
		{
			//�û�ȷ�����루�ڵ����ı�������س�ȷ����
			//�����ı�������س�����
			//WPARAMΪwchar_t*,��ʾ������ı�
			//LPARAMΪ�ַ�������
			IWM_USER_ENTER_INPUT=WM_USER+1,
			//ÿ������ʱ����
			//WPARAMΪwchar_t*,��ʾ������ı�
			//LPARAMΪ�ַ�������
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

