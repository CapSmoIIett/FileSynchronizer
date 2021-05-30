#pragma once

#ifndef __AFXWIN_H__
	#error "включить pch.h до включения этого файла в PCH"
#endif

#include "resource.h"		// основные символы


// CApplication:
class CApplication : public CWinApp
{
	public:
		CApplication();

	// Переопределение
	public:
		virtual BOOL InitInstance();

	// Реализация

		DECLARE_MESSAGE_MAP()
};

extern CApplication theApp;
