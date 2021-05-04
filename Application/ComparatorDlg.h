#pragma once

#include "WFDFile.h"

#include <vector>

#define EQUAL 0
#define NOTEQUAL 1
#define LEFTtoRIGHT 2
#define RIGHTtoLEFT 3


struct ComparisonResult
{
	WFDFile firstFile;
	WFDFile SecondFile;
	int ratio;
	ComparisonResult(WFDFile f, WFDFile s, int r) :
		firstFile(f),
		SecondFile(s),
		ratio (r)
	{	
	
	}
};

// Диалоговое окно CComparatorDlg

class CComparatorDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CComparatorDlg)

	public:
		CComparatorDlg(std::vector<WFDFile>, std::vector<WFDFile>,
			CString, CString, CWnd* pParent = nullptr);   // стандартный конструктор
		virtual ~CComparatorDlg();

	// Данные диалогового окна
	#ifdef AFX_DESIGN_TIME
		enum { IDD = IDD_DIALOG2 };
	#endif

	protected:
		virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV
		virtual BOOL OnInitDialog();

		

		DECLARE_MESSAGE_MAP()

	public:
		CString FirstDirectory;
		CString SecondDirectory;

		std::vector<ComparisonResult> CompareAll(std::vector<WFDFile>, std::vector<WFDFile>,
			CString firstDir = L"", CString secondDir = L"");							// Передача строки - следствие непродуманности алгоритма 

		std::vector<ComparisonResult> Comparasions;

		ComparisonResult Compare(WFDFile, WFDFile);

		BOOL WithFolders;
		BOOL WithContent;
		BOOL WithoutDate;

		

		std::vector<WFDFile> FilesFirstList;
		std::vector<WFDFile> FilesSecondList;

		std::vector<WFDFile> ViewDirectory(CString path);

		void setWithFolders(BOOL);
		void setWithContent(BOOL);
		void setWithoutDate(BOOL);

		CListCtrl List;

		BOOL LeftToRight;
		BOOL Equal;
		BOOL NotEqual;
		BOOL RightToLeft;

		afx_msg void ChangeCheckBoxLeftToRight();

		afx_msg void ChangeCheckBoxEqual();

		afx_msg void ChangeCheckNotEqual();

		afx_msg void ChangeCheckRightToLeft();

		void UpdateList();
		afx_msg void SyncLeftToRight();
		afx_msg void SyncRightToLeft();
};

