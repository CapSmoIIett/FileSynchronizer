#pragma once

#include <vector>
#include "WFDFile.h"


// Диалоговое окно CApplicationDlg
class CApplicationDlg : public CDialogEx
{
	public:
		// Стандартный конструктор
		CApplicationDlg(CWnd* pParent = nullptr);	
		~CApplicationDlg();

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


		afx_msg void FirstDirectoryAddressEdit();
		afx_msg void SecondDirectoryAddressEdit();
		afx_msg void ClickedButtonFirstView();
		afx_msg void ClickedButtonSecondView();

		CString FirstDirectoryAddress;			// Адрес директории
		CString SecondDirectoryAddress;

		CListCtrl ListFirstFolder;				// Содержимое директори
		CListCtrl ListSecondFolder;

		BOOL WithFolders;
		BOOL WithContent;
		BOOL WithoutDate;

		std::vector<WFDFile> FilesFirstList;
		std::vector<WFDFile> FilesSecondList;

		void UpdateList(CListCtrl& list, CString folder, std::vector<WFDFile>&);

		afx_msg void CompareFolders();

		afx_msg void SelectElementFirstTable(NMHDR* pNMHDR, LRESULT* pResult);
		afx_msg void SelectElementSecondTable(NMHDR* pNMHDR, LRESULT* pResult);
		
};
