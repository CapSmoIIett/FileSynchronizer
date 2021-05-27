#pragma once

#include "WFDFile.h"
#include "ComparisonResult.h"

#include <vector>


// Диалоговое окно CComparatorDlg

class CComparatorDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CComparatorDlg)

	
	void setWithFolders(BOOL);
	void setWithContent(BOOL);
	void setWithoutDate(BOOL);

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


	afx_msg void ChangeCheckBoxLeftToRight();
	afx_msg void ChangeCheckBoxEqual();
	afx_msg void ChangeCheckNotEqual();
	afx_msg void ChangeCheckRightToLeft();

	afx_msg void SyncLeftToRight();
	afx_msg void SyncRightToLeft();

	void UpdateList();

	DECLARE_MESSAGE_MAP()

	CString FirstDirectory;
	CString SecondDirectory;
	
	std::vector<ComparisonResult> CompareAll(std::vector<WFDFile>, std::vector<WFDFile>,
		CString firstDir = L"", CString secondDir = L"");							// Передача строки - следствие непродуманности алгоритма 

	std::vector<ComparisonResult> Comparasions;

	ComparisonResult Compare(WFDFile, WFDFile);

	BOOL WithFolders;
	BOOL WithContent;
	BOOL WithoutDate;

	BOOL LeftToRight;
	BOOL Equal;
	BOOL NotEqual;
	BOOL RightToLeft;

	std::vector<WFDFile> FilesFirstList;
	std::vector<WFDFile> FilesSecondList;

	std::vector<WFDFile> ViewDirectory(CString path);

	CListCtrl List;
};

