
// ApplicationDlg.cpp: файл реализации
//

//#include <Windows.h>

#include "pch.h"
#include "framework.h"
#include "Application.h"
#include "ApplicationDlg.h"
#include "AboutDlg.h"
#include "afxdialogex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Диалоговое окно CApplicationDlg


CApplicationDlg::CApplicationDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_APPLICATION_DIALOG, pParent)
	, FirstDirectoryAddress(_T(""))
	, SecondDirectoryAddress(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CApplicationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT2, FirstDirectoryAddress);
	DDX_Text(pDX, IDC_EDIT1, SecondDirectoryAddress);
	DDX_Control(pDX, IDC_LIST5, ListFirstFolder);
	DDX_Control(pDX, IDC_LIST4, ListSecondFolder);
}

BEGIN_MESSAGE_MAP(CApplicationDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_EN_CHANGE(IDC_EDIT1, &CApplicationDlg::OnEnChangeEdit1)
	ON_BN_CLICKED(IDC_BUTTON1, &CApplicationDlg::ClickedButtonFirstView)
	ON_BN_CLICKED(IDC_BUTTON2, &CApplicationDlg::ClickedButtonSecondView)
END_MESSAGE_MAP()


// Обработчики сообщений CApplicationDlg

BOOL CApplicationDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Добавление пункта "О программе..." в системное меню.

	// IDM_ABOUTBOX должен быть в пределах системной команды.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Задает значок для этого диалогового окна.  Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок

	ListFirstFolder.InsertColumn(0, L"Имя", LVCFMT_LEFT, 50);
	ListFirstFolder.InsertColumn(1, L"Тип", LVCFMT_LEFT, 50);
	ListFirstFolder.InsertColumn(2, L"Размер", LVCFMT_LEFT, 60);
	ListFirstFolder.InsertColumn(3, L"Дата", LVCFMT_LEFT, 50);
	ListFirstFolder.InsertColumn(4, L"Атрибуты", LVCFMT_LEFT, 70);

	ListSecondFolder.InsertColumn(0, L"Имя", LVCFMT_LEFT, 50);
	ListSecondFolder.InsertColumn(1, L"Тип", LVCFMT_LEFT, 50);
	ListSecondFolder.InsertColumn(2, L"Размер", LVCFMT_LEFT, 60);
	ListSecondFolder.InsertColumn(3, L"Дата", LVCFMT_LEFT, 50);
	ListSecondFolder.InsertColumn(4, L"Атрибуты", LVCFMT_LEFT, 70);

	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}

void CApplicationDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// При добавлении кнопки свертывания в диалоговое окно нужно воспользоваться приведенным ниже кодом,
//  чтобы нарисовать значок.  Для приложений MFC, использующих модель документов или представлений,
//  это автоматически выполняется рабочей областью.

void CApplicationDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // контекст устройства для рисования

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Выравнивание значка по центру клиентского прямоугольника
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Нарисуйте значок
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// Система вызывает эту функцию для получения отображения курсора при перемещении
//  свернутого окна.
HCURSOR CApplicationDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CApplicationDlg::OnEnChangeEdit1()
{
	// TODO:  Если это элемент управления RICHEDIT, то элемент управления не будет
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// функция и вызов CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Добавьте код элемента управления
}


void CApplicationDlg::ClickedButtonFirstView()
{
	CFolderPickerDialog dlg;
	auto answer = dlg.DoModal();
	if (answer == IDOK) {
		FirstDirectoryAddress = dlg.m_ofn.lpstrFile;
	}
	UpdateList(ListFirstFolder, FirstDirectoryAddress);
	UpdateData(false);		// из переменных в управление
}


void CApplicationDlg::ClickedButtonSecondView()
{
	CFolderPickerDialog dlg;
	auto answer = dlg.DoModal();
	if (answer == IDOK) {
		SecondDirectoryAddress = dlg.m_ofn.lpstrFile;
	}
	UpdateList(ListSecondFolder, SecondDirectoryAddress);
	UpdateData(false);		// из переменных в управление
}

void CApplicationDlg::UpdateList(CListCtrl& list, CString folder)
{
	WIN32_FIND_DATAW wfd;

	HANDLE const hFind = FindFirstFileW(folder + L"\\*", &wfd);

	if (INVALID_HANDLE_VALUE != hFind)
	{
		list.DeleteAllItems();
		int i = 0;
		do
		{
			CString name = CString(wfd.cFileName);
			if (name == "." || name == "..") continue;
			list.InsertItem(i++, name, -1);
		} while (NULL != FindNextFileW(hFind, &wfd));

		FindClose(hFind);
	}

}
