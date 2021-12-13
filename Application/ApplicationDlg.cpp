#include "pch.h"
#include "GoogleDriveModule.h"
#include "Application.h"
#include "ApplicationDlg.h"
#include "AboutDlg.h"
#include "afxdialogex.h"
#include "WFDTranslator.h"
#include "HexEditorDlg.h"
#include "resource.h"
#include "ErrorDlg.h"

#include <fstream>
#include <locale>
#include <codecvt>
#include <fstream>
#include <iostream>
#include <io.h>
#include <fcntl.h>

#define SIZE_COL_COMP 35
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


CApplicationDlg::CApplicationDlg(CWnd* pParent)
	: CDialogEx(IDD_APPLICATION_DIALOG, pParent)
	, FirstDirectoryAddress(_T(""))
	, SecondDirectoryAddress(_T(""))
	, WithFolders(FALSE)
	, WithContent(FALSE)
	, WithoutDate(FALSE)
	, LeftToRight(TRUE)
	, Equal(TRUE)
	, NotEqual(TRUE)
	, RightToLeft(TRUE)
	, WithoutAttribute(FALSE)
{
	CFile file;
	int startOfSecondPath = 0;

	ScrollPosition = 0;
	ScrollMutex = NOBODY_SCROLL;
	
	// Установка иконки
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	// Чтение из файла путей из прошлой ссесии 
	try
	{
		wchar_t bufWchar[MAX_SIZE_PATH * 2 + 1];
		const std::locale utf8_locale = std::locale(std::locale(), new std::codecvt_utf8<wchar_t>());
		
		std::wifstream file(NAME_OF_FILE, std::ios::binary);
		file.imbue(utf8_locale);

		if (!file.is_open())
			return;

		file >> bufWchar;
		
		file.close();

		CString buffer(bufWchar);

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
		ErrorDlg error(L"Ошибка в чтении файла!");
		error.DoModal();
		FirstDirectoryAddress = L"";
		SecondDirectoryAddress = L"";
	}
}

CApplicationDlg::~CApplicationDlg()
{
	try
	{
		const std::locale utf8_locale = std::locale(std::locale(), new std::codecvt_utf8<wchar_t>());
		std::wofstream file(NAME_OF_FILE, std::ios::binary | std::ios::trunc);
		file.imbue(utf8_locale);

		if (!FirstDirectoryAddress.IsEmpty())
		{
			file << FirstDirectoryAddress.GetBuffer();
		}

		file << SEPARATOR_CHARACTER;

		if (!SecondDirectoryAddress.IsEmpty())
		{
			file << SecondDirectoryAddress.GetBuffer();
		}

		file.close();
	}
	catch (...)
	{ 
		ErrorDlg error(L"Ошибка с записью в файл!");
		error.DoModal();
		return;
	}
}

void CApplicationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, FirstDirectoryAddress);
	DDX_Text(pDX, IDC_EDIT2, SecondDirectoryAddress);
	DDX_Control(pDX, LIST1, ListFirstFolder);
	DDX_Control(pDX, LIST2, ListSecondFolder);
	DDX_Check(pDX, IDC_CHECK1, WithFolders);
	DDX_Check(pDX, IDC_CHECK2, WithContent);
	DDX_Check(pDX, IDC_CHECK3, WithoutDate);
	DDX_Check(pDX, IDC_CHECK4, LeftToRight);
	DDX_Check(pDX, IDC_CHECK5, Equal);
	DDX_Check(pDX, IDC_CHECK6, NotEqual);
	DDX_Check(pDX, IDC_CHECK7, RightToLeft);
	DDX_Control(pDX, BUTTON_LR, SynchronizeLeftToRightButton);
	DDX_Control(pDX, BUTTON_RL, SynchronizeRightToLeftButton);
	DDX_Control(pDX, IDC_GD_BUTTON, GoogleDriveButton);
	DDX_Control(pDX, LIST3, ListComparisonResults);

	DDX_Check(pDX, IDC_CHECK8, WithoutAttribute);
}

BEGIN_MESSAGE_MAP(CApplicationDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_EN_CHANGE(IDC_EDIT1, &CApplicationDlg::FirstDirectoryAddressEdit)
	ON_EN_CHANGE(IDC_EDIT2, &CApplicationDlg::SecondDirectoryAddressEdit)
	
	ON_BN_CLICKED(IDC_BUTTON1, &CApplicationDlg::ClickedButtonFirstView)
	ON_BN_CLICKED(IDC_BUTTON2, &CApplicationDlg::ClickedButtonSecondView)	
	ON_BN_CLICKED(SYNC_BUTTON, &CApplicationDlg::CompareFolders)

	ON_NOTIFY(LVN_ITEMACTIVATE, LIST1, &CApplicationDlg::SelectElementFirstTable)
	ON_NOTIFY(LVN_ITEMACTIVATE, LIST2, &CApplicationDlg::SelectElementSecondTable)
	ON_NOTIFY(LVN_ITEMACTIVATE, LIST3, &CApplicationDlg::SelectElementComparisonTable)
	
	ON_BN_CLICKED(BUTTON_LR, &CApplicationDlg::SynchronizeLeftToRight)
	ON_BN_CLICKED(BUTTON_RL, &CApplicationDlg::SynchronizeRightToLeft)

	ON_NOTIFY(LVN_BEGINSCROLL, LIST1, &CApplicationDlg::BeginScrollListFirst)
	ON_NOTIFY(LVN_BEGINSCROLL, LIST2, &CApplicationDlg::BeginScrollListSecond)
	
	ON_BN_CLICKED(IDC_CHECK4, &CApplicationDlg::ChangeCheckBoxLeftToRight)
	ON_BN_CLICKED(IDC_CHECK5, &CApplicationDlg::ChangeCheckBoxEqual)
	ON_BN_CLICKED(IDC_CHECK6, &CApplicationDlg::ChangeCheckNotEqual)
	ON_BN_CLICKED(IDC_CHECK7, &CApplicationDlg::ChangeCheckRightToLeft)

	ON_NOTIFY(NM_CUSTOMDRAW, LIST2, &CApplicationDlg::OnNMCustomdraw)
	ON_NOTIFY(NM_CUSTOMDRAW, LIST1, &CApplicationDlg::OnNMCustomdraw)
	ON_NOTIFY(NM_CUSTOMDRAW, LIST3, &CApplicationDlg::OnNMCustomdrawListComparnResult) 

	ON_WM_ACTIVATE(&CApplicationDlg::OnActivate)

	ON_COMMAND(ID_32771, &CApplicationDlg::MenuLeftToRight)
	ON_COMMAND(ID_32772, &CApplicationDlg::MenuRightToLeft)

	ON_BN_CLICKED(IDC_CHECK1, &CApplicationDlg::ChangeCheckBoxWithFolders)
	ON_BN_CLICKED(IDC_CHECK2, &CApplicationDlg::ChangeCheckBoxWithContent)
	ON_BN_CLICKED(IDC_CHECK3, &CApplicationDlg::ChangeCheckBoxWithoutDate)
	ON_BN_CLICKED(IDC_CHECK8, &CApplicationDlg::ChangeCheckBoxWithoutAttribute)
	ON_BN_CLICKED(IDC_GD_BUTTON, &CApplicationDlg::SyncronizeGD)
END_MESSAGE_MAP()

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

	PopupMenu.LoadMenu(IDR_MENU1);
	ASSERT(PopupMenu != NULL);

	SynchronizeLeftToRightButton.SetTextColor(GREEN);
	SynchronizeRightToLeftButton.SetTextColor(BLUE);

	// Блокировка кнопок синхронизации
	SynchronizeLeftToRightButton.EnableWindow(FALSE);
	SynchronizeRightToLeftButton.EnableWindow(FALSE);

	// Задает значок для этого диалогового окна.  Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок

	UpdateData(false);

	// Настройка списков
	ListFirstFolder.SetExtendedStyle(LVS_EX_DOUBLEBUFFER | LVS_EX_FULLROWSELECT);
	ListSecondFolder.SetExtendedStyle(LVS_EX_DOUBLEBUFFER | LVS_EX_FULLROWSELECT);
	ListComparisonResults.SetExtendedStyle(LVS_EX_DOUBLEBUFFER);

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

void CApplicationDlg::insertInList(CListCtrl& list, WFDFile file, int number, CString path)
{
	CString folder = L"";
	int item = 0;

	if (path != L"")
	{
		BOOL flag = 1;
		int i = 0;

		for (; i < path.GetLength(); i++)
			if (path[i] != file.fullName[i])
			{
				flag = 0;
				break;
			}

		if (flag)
		{
			CString temp = file.fullName;
			temp = temp.Right(temp.GetLength() - i - 1);
			folder = temp.Left(temp.Find(file.name));
		}
	}

	if (file.size == L"0")
	{
		item = list.InsertItem(number, folder + file.name + "\\", -1);
	}
	else
	{
		item = list.InsertItem(number, folder + file.name, -1);
	}

	list.SetItemText(item, 1, file.type);
	list.SetItemText(item, 2, file.size);
	list.SetItemText(item, 3, file.date);
	list.SetItemText(item, 4, file.attr);
}

void CApplicationDlg::UpdateList(CListCtrl& list, CString folder, std::vector<WFDFile> &files)
{
	WIN32_FIND_DATA wfd;
	int number = 0;

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


	// Записываем в список
	for (auto file : files) 
	{
		insertInList(list, file, number++);
	}

	FindClose(handle);
}

BOOL CApplicationDlg::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN) {
		switch (pMsg->wParam)
		{
		case VK_ESCAPE:
		{
			return TRUE;
		}
		case VK_RETURN:			// Нажате на Enter
		{
			UpdateAll();
			return TRUE;
		}
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CApplicationDlg::UpdateAll(BOOL ready)
{
	// Переключение режима
	ReadyToSync = ready;

	if (ReadyToSync)
	{
		// WE will remove all unnecessary elements thus, the error with the coloring will disappear
		std::vector<int> toRemove;

		SynchronizeLeftToRightButton.EnableWindow(TRUE);
		SynchronizeRightToLeftButton.EnableWindow(TRUE);

		UpdateComparisonList();

		ListFirstFolder.DeleteAllItems();
		ListSecondFolder.DeleteAllItems();

		int numList = 0;
		int numFile = 0;

		for (auto file : Comparasions)
		{	
			// We count them at the beginning, so as not to do it at each end
			numFile++;

			if (file.FirstFile.size == L"0" &&
				file.SecondFile.size == L"0")
			{
				insertInList(ListFirstFolder, file.FirstFile, numList, FirstDirectoryAddress);
				insertInList(ListSecondFolder, file.SecondFile, numList, SecondDirectoryAddress);
				numList++;
				continue;
			}
			
			// Проверка checkbox-ов
			switch (file.ratio)
			{
			case LEFTtoRIGHT:	
			{
				if (!LeftToRight)
				{
					toRemove.push_back(numFile - 1);
					continue;
				}
				break;
			}
			case EQUAL:			
			{
				if (!Equal) 
				{
					toRemove.push_back(numFile - 1);
					continue;
				}
				break;
			}
			case NOTEQUAL:		
			{
				if (!NotEqual) 
				{
					toRemove.push_back(numFile - 1);
					continue;
				}
				break;
			}
			case RIGHTtoLEFT:	
			{
				if (!RightToLeft) 
				{
					toRemove.push_back(numFile - 1);
					continue;
				}
				break;
			}
			}

			insertInList(ListFirstFolder, file.FirstFile, numList, FirstDirectoryAddress);
			insertInList(ListSecondFolder, file.SecondFile, numList, SecondDirectoryAddress);
			numList++;
		}

		for (int i = 0; i < toRemove.size(); i++)
			Comparasions.erase(Comparasions.begin() + (toRemove[i] - i));
	}
	else
	{
		SynchronizeLeftToRightButton.EnableWindow(FALSE);
		SynchronizeRightToLeftButton.EnableWindow(FALSE);

		Comparasions.clear();
		ListComparisonResults.DeleteAllItems();

		if (!FirstDirectoryAddress.IsEmpty())
			UpdateList(ListFirstFolder, FirstDirectoryAddress, FilesFirstList);
		if (!SecondDirectoryAddress.IsEmpty())
			UpdateList(ListSecondFolder, SecondDirectoryAddress, FilesSecondList);
	}

	for (int i = 0; i < ListFirstFolder.GetHeaderCtrl()->GetItemCount(); ++i)
		ListFirstFolder.SetColumnWidth(i, LVSCW_AUTOSIZE_USEHEADER);
	for (int i = 0; i < ListSecondFolder.GetHeaderCtrl()->GetItemCount(); ++i)
		ListSecondFolder.SetColumnWidth(i, LVSCW_AUTOSIZE_USEHEADER);

	UpdateData(false);
}

int  CApplicationDlg::GetItemHeight(CListCtrl& list)
{
	CRect ItemRect;
	list.GetSubItemRect(1, 1, LVIR_BOUNDS, ItemRect);
	return ItemRect.bottom - ItemRect.top;
}

void CApplicationDlg::SyncronizeGD()
{
	GoogleDriveModule::LogIn();
}
