#pragma once

#include "ApplicationDlg.h"


#define WHITE RGB(255, 255, 255)
#define GREY  RGB(245, 245, 245)


// Диалоговое окно CHexEditorDlg

class CHexEditorDlg : public CDialogEx
{
		DECLARE_DYNAMIC(CHexEditorDlg)
	public:
		CHexEditorDlg(WFDFile wfd, CWnd* pParent = nullptr);   // стандартный конструктор
		virtual ~CHexEditorDlg();

	// Данные диалогового окна

	#ifdef AFX_DESIGN_TIME
		enum { IDD = IDD_DIALOG1 };
	#endif

	protected:
		HICON m_hIcon;

		virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV
		virtual BOOL OnInitDialog();

		CString IntToHex(int i, int size = 0);

		DECLARE_MESSAGE_MAP()

		CListCtrl DataTable;
		WFDFile CurrentFile;

		HANDLE handle;

		unsigned char* pointer;

		afx_msg void OnGetdispinfoList(NMHDR* pNMHDR, LRESULT* pResult);

		void OnNMCustomdraw(NMHDR* pNMHDR, LRESULT* pResult);

		// Обработчик нажатий(убирает реакцию на Enter и Esc)
		BOOL PreTranslateMessage(MSG* pMsg);		
};
