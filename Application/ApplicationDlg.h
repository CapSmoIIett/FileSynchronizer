#pragma once

#include <vector>
#include "WFDFile.h"
#include "WFDFile.h"
#include "ComparisonResult.h"
//#include "GoogleDriveModule.h"

#define NOBODY_SCROLL 0
#define FIRST_SCROLL  1
#define SECOND_SCROLL 2

#define GREEN	RGB(0, 128, 0)
#define RED		RGB(205, 0, 0)
#define BLUE	RGB(0, 0, 205)
#define BLACK	RGB(0, 0, 0)

// Главное диалоговое окно 
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

		// Функции схемы сообщений
		virtual BOOL OnInitDialog();
		afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
		afx_msg void OnPaint();
		afx_msg HCURSOR OnQueryDragIcon();

		DECLARE_MESSAGE_MAP()

		afx_msg void FirstDirectoryAddressEdit();
		afx_msg void SecondDirectoryAddressEdit();

		afx_msg void ClickedButtonFirstView();
		afx_msg void ClickedButtonSecondView();

		afx_msg void SynchronizeLeftToRight();
		afx_msg void SynchronizeRightToLeft();

		afx_msg void BeginScrollListFirst(NMHDR* pNMHDR, LRESULT* pResult);
		afx_msg void BeginScrollListSecond(NMHDR* pNMHDR, LRESULT* pResult);

		afx_msg void ChangeCheckBoxLeftToRight();
		afx_msg void ChangeCheckBoxEqual();
		afx_msg void ChangeCheckNotEqual();
		afx_msg void ChangeCheckRightToLeft();
		afx_msg void ChangeCheckBoxWithFolders();
		afx_msg void ChangeCheckBoxWithContent();
		afx_msg void ChangeCheckBoxWithoutDate();
		afx_msg void ChangeCheckBoxWithoutAttribute();

		afx_msg void SelectElementFirstTable(NMHDR* pNMHDR, LRESULT* pResult);
		afx_msg void SelectElementSecondTable(NMHDR* pNMHDR, LRESULT* pResult);
		afx_msg void SelectElementComparisonTable(NMHDR* pNMHDR, LRESULT* pResult);

		afx_msg void CompareFolders();

		afx_msg void OnNMCustomdraw(NMHDR* pNMHDR, LRESULT* pResult);
		afx_msg void OnNMCustomdrawListComparnResult(NMHDR* pNMHDR, LRESULT* pResult);
		
		// For PopupMenu
		afx_msg void MenuLeftToRight();
		afx_msg void MenuRightToLeft();

		afx_msg void OnActivate(UINT, CWnd*, BOOL);

		// Обновление списка файлов
		void UpdateList(CListCtrl& list, CString folder, std::vector<WFDFile>&);
		void insertInList(CListCtrl& list, WFDFile file, int number, CString path = L"");
		void UpdateComparisonList(int begin = 0);

		void UpdateAll(BOOL ready = 0);

		// Функции синхронизации
		void syncLeftToRight(WFDFile first, WFDFile second);
		void syncRightToLeft(WFDFile first, WFDFile second);

		int GetItemHeight(CListCtrl&);

		afx_msg void SyncronizeGD();

protected:

		// For PopupMenu
		int numberSelectedItem;

		// Обработчик нажатий (для отключения Enter и Esc)
		BOOL PreTranslateMessage(MSG* pMsg);	
		
		// Функция просмотра директории
		std::vector<WFDFile> ViewDirectory(CString path);

		// Функция сравнения двух директорий
		std::vector<ComparisonResult> CompareAll(std::vector<WFDFile>, 
			std::vector<WFDFile>, CString firstDir = L"", CString secondDir = L"");		// Передача строки - следствие непродуманности алгоритма

		// Срванение двух файлов
		ComparisonResult Compare(WFDFile, WFDFile);
		
		int ScrollPosition;
		int ScrollMutex;

		// Адрес директории
		CString FirstDirectoryAddress;			
		CString SecondDirectoryAddress;


		CListCtrl ListFirstFolder;				
		CListCtrl ListSecondFolder;
		CListCtrl ListComparisonResults;
		
		CMenu PopupMenu;

		BOOL WithFolders;
		BOOL WithContent;
		BOOL WithoutDate;
		BOOL WithoutAttribute;

		BOOL LeftToRight;
		BOOL Equal;
		BOOL NotEqual;
		BOOL RightToLeft;

		BOOL ReadyToSync;

		CMFCButton SynchronizeLeftToRightButton;
		CMFCButton SynchronizeRightToLeftButton;
		CMFCButton GoogleDriveButton;
		CToolBar m_wndToolBar;
		
		std::vector<WFDFile> FilesFirstList;
		std::vector<WFDFile> FilesSecondList;

		// Результаты сравнения
		std::vector<ComparisonResult> Comparasions;

};
