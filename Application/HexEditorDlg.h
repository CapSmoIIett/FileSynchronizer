#pragma once

#include "ApplicationDlg.h"

// Диалоговое окно CHexEditorDlg

class CHexEditorDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CHexEditorDlg)

public:
	// Необходимо Вводить ApplicationDlg для получения адрессов папок
	CHexEditorDlg(WFDFile wfd, CWnd* pParent = nullptr);   // стандартный конструктор
	virtual ~CHexEditorDlg();

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV
	virtual BOOL OnInitDialog();

	BOOL ReadFileHex(WFDFile, CListCtrl*);
	BOOL ReadFileHexMapp(WFDFile, CListCtrl*);
	CString IntToHex(int i, int size = 0);
	CString IntToBin(int i, int size = 0);

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl DataTable;
	WFDFile CurrentFile;
};
