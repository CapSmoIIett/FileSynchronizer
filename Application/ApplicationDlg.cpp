
// ApplicationDlg.cpp: файл реализации
//

//#include <Windows.h>

#include "pch.h"
#include "framework.h"
#include "Application.h"
#include "ApplicationDlg.h"
#include "AboutDlg.h"
#include "afxdialogex.h"
#include "WFDTranslator.h"
#include "HexEditorDlg.h"
#include "ComparatorDlg.h"
//#include <tchar.h>

#include <fstream>
#include <locale>
#include <codecvt>
#include <fstream>
#include <iostream>
#include <io.h>
#include <fcntl.h>

#define SIZE_COL_COMP 30
#define SIZE_COL_NAME 150
#define SIZE_COL_TYPE 100
#define SIZE_COL_SIZE 100
#define SIZE_COL_DATE 110
#define SIZE_COL_ATTR 100
#define MAX_SIZE_PATH 32767

#define SEPARATOR_CHARACTER L"*"

#define NAME_OF_FILE  L"Aplication.bin"

#ifdef _DEBUG
//#define new DEBUG_NEW
#endif


// Диалоговое окно CApplicationDlg
CApplicationDlg::CApplicationDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_APPLICATION_DIALOG, pParent)
	, FirstDirectoryAddress(_T(""))
	, SecondDirectoryAddress(_T(""))
	, WithFolders(FALSE)
	, WithContent(FALSE)
	, WithoutDate(FALSE)
	, LeftToRight(FALSE)
	, Equal(FALSE)
	, NotEqual(FALSE)
	, RightToLeft(FALSE)
{
	CFile file;
	int startOfSecondPath = 0;

	ScrollPosition = 0;
	ScrollMutex = NOBODY_SCROLL;
	// Установка иконки
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);//L"res\\Application"


	// Чтение из файла
	try
	{
		wchar_t buf[MAX_SIZE_PATH * 2 + 1];
		const std::locale utf8_locale = std::locale(std::locale(), new std::codecvt_utf8<wchar_t>());
		
		std::wifstream f(NAME_OF_FILE, std::ios::binary);
		f.imbue(utf8_locale);
		
		f >> buf;
		
		f.close();

		CString buffer(buf);

		startOfSecondPath = buffer.Find(SEPARATOR_CHARACTER);
		if (startOfSecondPath == -1)
			return;
		if (startOfSecondPath != 0)
			FirstDirectoryAddress = buffer.Left(startOfSecondPath);
		if ((size_t)startOfSecondPath != buffer.GetLength() - 1)
			SecondDirectoryAddress = buffer.Right(buffer.GetLength() - 
				(size_t)startOfSecondPath - 1);	// - 1 для SEPARATOR_CHARACTER

		// проверка на существование директорий
		DWORD fileAttrFirst = GetFileAttributes(FirstDirectoryAddress);
		if (fileAttrFirst == 0xFFFFFFFF)	
			FirstDirectoryAddress = L"";
		DWORD fileAttrSecond = GetFileAttributes(SecondDirectoryAddress);
		if (fileAttrSecond == 0xFFFFFFFF)	
			SecondDirectoryAddress = L"";
		

	}
	catch (...)
	{
		FirstDirectoryAddress = L"";
		SecondDirectoryAddress = L"";
	}
}

CApplicationDlg::~CApplicationDlg()
{
	const std::locale utf8_locale = std::locale(std::locale(), new std::codecvt_utf8<wchar_t>());
	std::wofstream f(NAME_OF_FILE, std::ios::binary | std::ios::trunc);
	f.imbue(utf8_locale);

	if (!FirstDirectoryAddress.IsEmpty())
	{
		f << FirstDirectoryAddress.GetBuffer();
	}

	f << SEPARATOR_CHARACTER;

	if (!SecondDirectoryAddress.IsEmpty())
	{
		f << SecondDirectoryAddress.GetBuffer();
	}

	f.close();
}

void CApplicationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, FirstDirectoryAddress);
	DDX_Text(pDX, IDC_EDIT2, SecondDirectoryAddress);
	DDX_Control(pDX, IDC_LIST5, ListFirstFolder);
	DDX_Control(pDX, IDC_LIST4, ListSecondFolder);
	DDX_Check(pDX, IDC_CHECK1, WithFolders);
	DDX_Check(pDX, IDC_CHECK2, WithContent);
	DDX_Check(pDX, IDC_CHECK3, WithoutDate);
	DDX_Check(pDX, IDC_CHECK4, LeftToRight);
	DDX_Check(pDX, IDC_CHECK5, Equal);
	DDX_Check(pDX, IDC_CHECK6, NotEqual);
	DDX_Check(pDX, IDC_CHECK7, RightToLeft);
	DDX_Control(pDX, IDC_BUTTON4, SynchronizeButton);
	DDX_Control(pDX, IDC_LIST6, ListComparisonResults);
}


BEGIN_MESSAGE_MAP(CApplicationDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_EN_CHANGE(IDC_EDIT1, &CApplicationDlg::FirstDirectoryAddressEdit)
	ON_EN_CHANGE(IDC_EDIT2, &CApplicationDlg::SecondDirectoryAddressEdit)
	
	ON_BN_CLICKED(IDC_BUTTON1, &CApplicationDlg::ClickedButtonFirstView)
	ON_BN_CLICKED(IDC_BUTTON2, &CApplicationDlg::ClickedButtonSecondView)	
	ON_BN_CLICKED(IDC_BUTTON3, &CApplicationDlg::CompareFolders)

	ON_NOTIFY(LVN_ITEMACTIVATE, IDC_LIST5, &CApplicationDlg::SelectElementFirstTable)
	ON_NOTIFY(LVN_ITEMACTIVATE, IDC_LIST4, &CApplicationDlg::SelectElementSecondTable)
	ON_BN_CLICKED(IDC_BUTTON4, &CApplicationDlg::Synchronize)
	ON_NOTIFY(LVN_BEGINSCROLL, IDC_LIST5, &CApplicationDlg::BeginScrollListFirst)
	ON_NOTIFY(LVN_BEGINSCROLL, IDC_LIST4, &CApplicationDlg::BeginScrollListSecond)
	
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

	SynchronizeButton.EnableWindow(FALSE);

	// Задает значок для этого диалогового окна.  Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок

	LeftToRight = TRUE;
	Equal = TRUE;
	NotEqual = TRUE;
	RightToLeft = TRUE;

	UpdateData(false);


	//ListComparisonResults.EnableScrollBar(LVS_NOSCROLL);
	//ListComparisonResults.ModifyStyle(LVS_NOSCROLL, 0);
	ListComparisonResults.InsertColumn(0, L"Comparison", LVCFMT_LEFT, SIZE_COL_COMP);

	ListFirstFolder.InsertColumn(0, L"Имя",		 LVCFMT_LEFT, SIZE_COL_NAME);
	ListFirstFolder.InsertColumn(1, L"Тип",		 LVCFMT_LEFT, SIZE_COL_TYPE);
	ListFirstFolder.InsertColumn(2, L"Размер",	 LVCFMT_LEFT, SIZE_COL_SIZE);
	ListFirstFolder.InsertColumn(3, L"Дата",	 LVCFMT_LEFT, SIZE_COL_DATE);
	ListFirstFolder.InsertColumn(4, L"Атрибуты", LVCFMT_LEFT, SIZE_COL_ATTR);

	ListSecondFolder.InsertColumn(0, L"Имя",	 LVCFMT_LEFT, SIZE_COL_NAME);
	ListSecondFolder.InsertColumn(1, L"Тип",	 LVCFMT_LEFT, SIZE_COL_TYPE);
	ListSecondFolder.InsertColumn(2, L"Размер",  LVCFMT_LEFT, SIZE_COL_SIZE);
	ListSecondFolder.InsertColumn(3, L"Дата",	 LVCFMT_LEFT, SIZE_COL_DATE);
	ListSecondFolder.InsertColumn(4, L"Атрибуты",LVCFMT_LEFT, SIZE_COL_ATTR);

	UpdateAll();

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

void CApplicationDlg::insertInList(CListCtrl& list, WFDFile file, int number)
{
	int item = 0;
	if (file.size == L"0")
	{
		item = list.InsertItem(number, file.name + "\\", -1);
	}
	else
	{
		item = list.InsertItem(number, file.name, -1);
	}
	list.SetItemText(item, 1, file.type);
	list.SetItemText(item, 2, file.size);
	list.SetItemText(item, 3, file.date);
	list.SetItemText(item, 4, file.attr);
}

void CApplicationDlg::UpdateList(CListCtrl& list, CString folder, std::vector<WFDFile> &files)
{
	WIN32_FIND_DATA wfd;

	HANDLE const handle = FindFirstFileW(folder + L"\\*", &wfd);

	if (INVALID_HANDLE_VALUE == handle) return;

	list.DeleteAllItems();
	files.clear();
	
	do
	{
		if (CString(wfd.cFileName) == "." || CString(wfd.cFileName) == "..") continue;

		WFDTranslator translator(wfd, folder);

		files.push_back(WFDFile(translator.getFullName(), translator.getName(),
			translator.getType(), translator.getSize(),
			translator.getDate(), translator.getAttr()));
	} while (NULL != FindNextFileW(handle, &wfd));

	// Можно отсортировать вектор

	// Записываем в список
	int i = 0;
	for (auto file : files) 
	{
		insertInList(list, file, i++);
	}

	FindClose(handle);
}

BOOL CApplicationDlg::PreTranslateMessage(MSG* pMsg) {
	if (pMsg->message == WM_KEYDOWN) {
		if (pMsg->wParam == VK_ESCAPE)
		{
			return TRUE;
		}
		if (pMsg->wParam == VK_RETURN)											// Нажате на Enter
		{
			UpdateList(ListFirstFolder, FirstDirectoryAddress, FilesFirstList);
			UpdateList(ListSecondFolder, SecondDirectoryAddress, FilesSecondList);
			return TRUE;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CApplicationDlg::UpdateAll(BOOL ready)
{
	ReadyToSync = ready;
	if (ReadyToSync)
	{
		SynchronizeButton.EnableWindow(TRUE);
		UpdateComparisonList();

		ListFirstFolder.DeleteAllItems();
		ListSecondFolder.DeleteAllItems();

		int i = 0;
		for (auto file : Comparasions)
		{
			insertInList(ListFirstFolder, file.FirstFile, i);
			insertInList(ListSecondFolder, file.SecondFile, i);
			i++;
		}
	}
	else
	{
		SynchronizeButton.EnableWindow(FALSE);
		Comparasions.clear();
		ListComparisonResults.DeleteAllItems();
		if (!FirstDirectoryAddress.IsEmpty())
			UpdateList(ListFirstFolder, FirstDirectoryAddress, FilesFirstList);
		if (!SecondDirectoryAddress.IsEmpty())
			UpdateList(ListSecondFolder, SecondDirectoryAddress, FilesSecondList);
	}
	UpdateData(false);
}

int CApplicationDlg::GetItemHeight(CListCtrl& list)
{
	CRect ItemRect;
	list.GetSubItemRect(1, 1, LVIR_BOUNDS, ItemRect);
	return ItemRect.bottom - ItemRect.top;
}

