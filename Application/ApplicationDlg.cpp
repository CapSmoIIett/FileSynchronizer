
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

#define SIZE_COL_NAME 150
#define SIZE_COL_TYPE 100
#define SIZE_COL_SIZE 100
#define SIZE_COL_DATE 110
#define SIZE_COL_ATTR 100

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Диалоговое окно CApplicationDlg


CApplicationDlg::CApplicationDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_APPLICATION_DIALOG, pParent)
	, FirstDirectoryAddress(_T(""))
	, SecondDirectoryAddress(_T(""))
	, WithFolders(FALSE)
	, WithContent(FALSE)
	, WithoutDate(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
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



void CApplicationDlg::ClickedButtonFirstView()
{
	CFolderPickerDialog dlg;
	auto answer = dlg.DoModal();
	if (answer == IDOK) {
		FirstDirectoryAddress = dlg.m_ofn.lpstrFile;
	}
	UpdateList(ListFirstFolder, FirstDirectoryAddress, FilesFirstList);
	UpdateData(false);		// из переменных в управление
}


void CApplicationDlg::ClickedButtonSecondView()
{
	CFolderPickerDialog dlg;
	auto answer = dlg.DoModal();
	if (answer == IDOK) {
		SecondDirectoryAddress = dlg.m_ofn.lpstrFile;
	}
	UpdateList(ListSecondFolder, SecondDirectoryAddress, FilesSecondList);
	UpdateData(false);		// из переменных в управление2
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
	for (auto file : files) {		
		int item = list.InsertItem(i, file.name, -1);
		list.SetItemText(item, 1, file.type);
		list.SetItemText(item, 2, file.size);
		list.SetItemText(item, 3, file.date);
		list.SetItemText(item, 4, file.attr);
	}

	FindClose(handle);
}


void CApplicationDlg::FirstDirectoryAddressEdit()
{
	if (FirstDirectoryAddress.IsEmpty()) return;
	UpdateData(true);
	UpdateList(ListFirstFolder, FirstDirectoryAddress, FilesFirstList);
	UpdateData(false);
}

void CApplicationDlg::SecondDirectoryAddressEdit()
{
	if (SecondDirectoryAddress.IsEmpty()) return;
	UpdateData(true);
	UpdateList(ListSecondFolder, SecondDirectoryAddress, FilesSecondList);
	UpdateData(false);
}


void CApplicationDlg::CompareFolders()
{
	UpdateData(true);
	if (FirstDirectoryAddress.IsEmpty() || SecondDirectoryAddress.IsEmpty()) 
		return;
	UpdateList(ListFirstFolder, FirstDirectoryAddress, FilesFirstList);
	UpdateList(ListSecondFolder, SecondDirectoryAddress, FilesSecondList);
	CComparatorDlg comparator(FilesFirstList, FilesSecondList, 
		FirstDirectoryAddress, SecondDirectoryAddress, this);
	comparator.setWithContent(WithContent);
	comparator.setWithFolders(WithFolders);
	comparator.setWithoutDate(WithoutDate);
	comparator.DoModal();
	UpdateData(false);
}


void CApplicationDlg::SelectElementFirstTable(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	WFDFile file = FilesFirstList[FilesFirstList.size() - pNMIA->iItem - 1];

	CHexEditorDlg editor(file, this);
	editor.DoModal();

	*pResult = 0;
}


void CApplicationDlg::SelectElementSecondTable(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	WFDFile file = FilesSecondList[FilesSecondList.size() - pNMIA->iItem - 1];

	CHexEditorDlg editor(file, this);
	//editor.SetWindowTextW(file.)
	editor.DoModal();

	*pResult = 0;
}
