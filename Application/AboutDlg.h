#pragma once

#include "Application.h"

// ���������� ���� CAboutDlg ������������ ��� �������� �������� � ����������

class CAboutDlg : public CDialogEx
{
	public:
		CAboutDlg();

		// ������ ����������� ����
	#ifdef AFX_DESIGN_TIME
		enum { IDD = IDD_ABOUTBOX };
	#endif

	protected:
		virtual void DoDataExchange(CDataExchange* pDX);    // ��������� DDX/DDV

	// ����������
	protected:
		DECLARE_MESSAGE_MAP()
	};