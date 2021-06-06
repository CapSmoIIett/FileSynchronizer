// ErrorDlg.cpp: файл реализации
//

#include "pch.h"
#include "Application.h"
#include "ErrorDlg.h"
#include "afxdialogex.h"


// Диалоговое окно ErrorDlg

IMPLEMENT_DYNAMIC(ErrorDlg, CDialogEx)

ErrorDlg::ErrorDlg(CString text, CWnd* pParent)
	: CDialogEx(IDD_ERROR, pParent)
	, ErrorMessage(text)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

ErrorDlg::~ErrorDlg()
{
}

void ErrorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TEXT, ErrorMessage);
	DDX_Control(pDX, IDC_PICTURE, Picture);
}


BEGIN_MESSAGE_MAP(ErrorDlg, CDialogEx)
END_MESSAGE_MAP()

BOOL ErrorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	VERIFY(image.Load(IDB_ERROR));
	Picture.SetBitmap(image);

	return TRUE;
}

// Обработчики сообщений ErrorDlg
