#pragma once
#include<Windows.h>

namespace CMath {

	//获取错误如窗口创建失败错误
	//要使用LocalFree(errorMsg)清理内存
	void GetSystemLastError(DWORD* ErrorCode, CHAR** ErrorMsg);
	//获取错误如窗口创建失败错误
	class GetSystemLastErrorClass
	{
		DWORD m_ErrorCode;
		char* m_ErrorMsg;
	public:
		GetSystemLastErrorClass()
		{
			m_ErrorCode = GetLastError();
			FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
				nullptr, m_ErrorCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), reinterpret_cast<LPSTR>(&m_ErrorMsg), 0, nullptr);
		}
		GetSystemLastErrorClass(DWORD* ErrorCode, CHAR** ErrorMsg)
		{
			*ErrorCode = GetLastError();
			m_ErrorCode = *ErrorCode;
			FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
				nullptr, *ErrorCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), reinterpret_cast<LPSTR>(ErrorMsg), 0, nullptr);
			m_ErrorMsg = *ErrorMsg;
		}
		DWORD GetErrorCode()
		{
			return m_ErrorCode;
		}
		char* GetErrorMsg()
		{
			return m_ErrorMsg;
		}
		~GetSystemLastErrorClass()
		{
			LocalFree(m_ErrorMsg);
		}
	};
	//获取错误如窗口创建失败错误
	//要使用LocalFree(errorMsg)清理内存
	void GetSystemLastError(DWORD* ErrorCode, WCHAR** ErrorMsg);
	//获取错误如窗口创建失败错误
	class GetSystemLastErrorClassW
	{
		DWORD m_ErrorCode;
		WCHAR* m_ErrorMsg;
	public:
		GetSystemLastErrorClassW()
		{
			m_ErrorCode = GetLastError();
			FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
				nullptr, m_ErrorCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), reinterpret_cast<LPWSTR>(&m_ErrorMsg), 0, nullptr);
		}
		GetSystemLastErrorClassW(DWORD* ErrorCode, WCHAR** ErrorMsg)
		{
			*ErrorCode = GetLastError();
			m_ErrorCode = *ErrorCode;
			FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
				nullptr, *ErrorCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), reinterpret_cast<LPWSTR>(ErrorMsg), 0, nullptr);
			m_ErrorMsg = *ErrorMsg;
		}
		DWORD GetErrorCode()
		{
			return m_ErrorCode;
		}
		wchar_t* GetErrorMsg()
		{
			return m_ErrorMsg;
		}
		~GetSystemLastErrorClassW()
		{
			LocalFree(m_ErrorMsg);
		}

	};
	LOGFONT ChooseLogFont_g(HWND hWnd);
	COLORREF ChooseCustomColor(HWND hwnd);
}