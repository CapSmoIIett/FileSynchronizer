#pragma once


// Диалоговое окно CApplicationDlg
class CApplicationDlg : public CDialogEx
{
	public:
		// Стандартный конструктор
		CApplicationDlg(CWnd* pParent = nullptr);	

	// Данные диалогового окна
	#ifdef AFX_DESIGN_TIME
		enum { IDD = IDD_APPLICATION_DIALOG };
	#endif

	protected:
		virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV


	// Реализация
	protected:
		HICON m_hIcon;

		// Созданные функции схемы сообщений
		virtual BOOL OnInitDialog();
		afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
		afx_msg void OnPaint();
		afx_msg HCURSOR OnQueryDragIcon();
		DECLARE_MESSAGE_MAP()
	public:
		afx_msg void FirstDirectoryAddressEdit();
		afx_msg void SecondDirectoryAddressEdit();
		afx_msg void ClickedButtonFirstView();
		afx_msg void ClickedButtonSecondView();

		CString FirstDirectoryAddress;			// Адрес директории
		CString SecondDirectoryAddress;
		
		CListCtrl ListFirstFolder;				// Содержимое директори
		CListCtrl ListSecondFolder;

		void UpdateList(CListCtrl& list, CString folder);
		
};
