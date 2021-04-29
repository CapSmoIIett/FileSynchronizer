#pragma once

#include "Application.h"

// Диалоговое окно CAboutDlg используется для описания сведений о приложении

class CAboutDlg : public CDialogEx
{
	public:
		CAboutDlg();

		// Данные диалогового окна
	#ifdef AFX_DESIGN_TIME
		enum { IDD = IDD_ABOUTBOX };
	#endif

	protected:
		virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

	// Реализация
	protected:
		DECLARE_MESSAGE_MAP()
	};