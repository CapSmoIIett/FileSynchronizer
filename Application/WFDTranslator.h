#pragma once

#include "pch.h"
#include "framework.h"
#include "Application.h"
#include "ApplicationDlg.h"
#include "AboutDlg.h"
#include "afxdialogex.h"

// Класс обрабатывающий WIN32_FIND_DATA 
// и возвращающий строки с параметрами файлов
class WFDTranslator		
{
	public:
		WFDTranslator() = delete;
		WFDTranslator(WIN32_FIND_DATA, CString path = L"");

		
		CString getNameWithType();
		CString getFullName();
		CString getName(); 
		CString getType();
		CString getSize();
		CString getDate();
		CString getAttr();


	private:
		SYSTEMTIME UTC, time;
		CString FullName;
		CString name;
		CString type;
		CString size;
		CString date;
		CString attr;
};

