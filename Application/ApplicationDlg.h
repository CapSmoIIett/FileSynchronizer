#pragma once

#include <vector>
#include "WFDFile.h"
#include "WFDFile.h"
#include "ComparisonResult.h"

#define NOBODY_SCROLL 0
#define FIRST_SCROLL  1
#define SECOND_SCROLL 2


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
		afx_msg void Synchronize();
		afx_msg void BeginScrollListFirst(NMHDR* pNMHDR, LRESULT* pResult);
		afx_msg void BeginScrollListSecond(NMHDR* pNMHDR, LRESULT* pResult);

		afx_msg void SelectElementFirstTable(NMHDR* pNMHDR, LRESULT* pResult);
		afx_msg void SelectElementSecondTable(NMHDR* pNMHDR, LRESULT* pResult);

		afx_msg void CompareFolders();

		void insertInList(CListCtrl& list, WFDFile file, int number);
		// Обновление списка файлов
		void UpdateList(CListCtrl& list, CString folder, std::vector<WFDFile>&);
		void UpdateComparisonList(int begin = 0);

		void UpdateAll(BOOL ready = 0);

		// Функции синхронизации
		void synchronizeNotEqual(WFDFile first, WFDFile second);
		void synchronizeLeftToRight(WFDFile first, WFDFile second);
		void synchronizeRightToLeft(WFDFile first, WFDFile second);

		int GetItemHeight(CListCtrl&);


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
		

		BOOL WithFolders;
		BOOL WithContent;
		BOOL WithoutDate;

		BOOL LeftToRight;
		BOOL Equal;
		BOOL NotEqual;
		BOOL RightToLeft;

		BOOL ReadyToSync;

		CButton SynchronizeButton;

		std::vector<WFDFile> FilesFirstList;
		std::vector<WFDFile> FilesSecondList;

		// Результаты сравнения
		std::vector<ComparisonResult> Comparasions;
	
};
