// HexEditorDlg.cpp: файл реализации
#include <fstream>
#include "pch.h"
#include "Application.h"
#include "HexEditorDlg.h"
#include "afxdialogex.h"

#include <stack>

#define AMOUNT_COL		18		// 8 - линейка, 1 - 16-ое, 8 - 2-ое, 1 - ascii представления
#define COL_RULER		8
#define COL_BIN			8		
#define FFFFFFFF		4294967295	// 0FFFFFFFFh in dec
#define SIZE_COL		30

// Диалоговое окно CHexEditorDlg

IMPLEMENT_DYNAMIC(CHexEditorDlg, CDialogEx)

CHexEditorDlg::CHexEditorDlg(WFDFile wfd, CWnd* pParent /*=nullptr*/) :
	CDialogEx(IDD_DIALOG1, pParent),
	CurrentFile(wfd)
{	}

CHexEditorDlg::~CHexEditorDlg()
{	}

BOOL CHexEditorDlg::OnInitDialog() {
	CDialogEx::OnInitDialog();
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

	for (int i = 0; i < AMOUNT_COL; i++)	// Создание колонок таблицы
		DataTable.InsertColumn(i, L"A", LVCFMT_LEFT, SIZE_COL);

	// В зависимости от размера файла выбираем спосокб его обработки
	BOOL result; 
	if (_ttoi(CurrentFile.size) < 10000)
		result = ReadFileHex(CurrentFile, &DataTable);		// CFile - имеет свои обработчики которым стоит довериться
	else
		result = ReadFileHexMapp(CurrentFile, &DataTable);	// маппинг быстрее но я не знаю всех искдючительных ситуаций

	if (!result) 
	{
		// Сообщение о ошибке
		SendMessage(WM_CLOSE);	// Закрываем окно
	}
	
	UpdateData(false);
	return TRUE;
}


BOOL CHexEditorDlg::ReadFileHex(WFDFile wfdFile, CListCtrl* list) 
{
	CFile file;
	if (file.Open(wfdFile.fullName, CFile::modeRead | CFile::typeBinary) == 0) 
		return FALSE;
	

	long int counter = 0;
	long int len = _ttoi(wfdFile.size);
	unsigned char* buffer = new unsigned char[len];
	unsigned char  c = 0;

	if (file.Read((void*)buffer, sizeof(char) * len) < len) 
		return FALSE;

	for (; counter < len; counter++) {

		int numberCol = 0;
		int item = 0;

		CString ruler;
		CString hex;
		CString bin;

		if (counter > FFFFFFFF) break;
		
		c = buffer[counter];

		ruler = IntToHex(counter, COL_RULER);
		
		item = list->InsertItem(counter, CString(ruler[numberCol++]), -1);

		for (; numberCol < COL_RULER; numberCol++)
			list->SetItemText(item, numberCol, CString(ruler[numberCol]));

		hex = IntToHex(c);
		list->SetItemText(item, numberCol++, hex);

		bin = IntToBin(c, COL_BIN);

		for (int i = 0; i < COL_BIN; i++)
			list->SetItemText(item, numberCol++, CString(bin[i]));

		list->SetItemText(item, numberCol, CString((char)c));	
	}
	delete[] buffer;
	file.Close();
	return true;
}

BOOL CHexEditorDlg::ReadFileHexMapp(WFDFile wfdFile, CListCtrl* list)
{
	HANDLE handle = CreateFile(wfdFile.fullName, GENERIC_READ, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (handle == INVALID_HANDLE_VALUE) 
	{
		return FALSE;
	}

	HANDLE mappFile = CreateFileMapping(handle, NULL, PAGE_READONLY, 0, 0, NULL);
	if (mappFile == NULL) 
	{
		return FALSE;
	}

	unsigned char* pointer = (unsigned char* )MapViewOfFile(mappFile, FILE_MAP_READ, 0, 0, 0);
	if (pointer == NULL) 
	{
		return FALSE;
	}

	long int counter = 0;
	long int len = _ttoi(wfdFile.size);
	unsigned char c = 0;

	for (; counter < len; counter++) 
	{

		int numberCol = 0;
		int item = 0;

		CString ruler;
		CString hex;
		CString bin;

		if (counter > FFFFFFFF) break;

		c = pointer[counter];

		ruler = IntToHex(counter, COL_RULER);
		item = list->InsertItem(counter, CString(ruler[numberCol++]), -1);
		for (; numberCol < COL_RULER; numberCol++)
			list->SetItemText(item, numberCol, CString(ruler[numberCol]));

		hex = IntToHex(c);
		list->SetItemText(item, numberCol++, hex);

		bin = IntToBin(c, COL_BIN);

		for (int i = 0; i < COL_BIN; i++)
			list->SetItemText(item, numberCol++, CString(bin[i]));

		list->SetItemText(item, numberCol, CString((char)c));
	}
	CloseHandle(handle);
	return TRUE;
}


CString CHexEditorDlg::IntToHex(int number, int size)
{
	CString answer;

	if (number < 0) 
	{
		number *= -1;
	}

	std::stack<char> st;

	while (number > 0)
	{
		int n = number % 16;
		switch (n)
		{
		case 15:
			st.push('F');
			break;
		case 14:
			st.push('E');
			break;
		case 13:
			st.push('D');
			break;
		case 12:
			st.push('C');
			break;
		case 11:
			st.push('B');
			break;
		case 10:
			st.push('A');
			break;
		default:
			st.push('0' + n);
		}
		number /= 16;
	}
	while (!st.empty()) {
		answer = answer + CString(st.top());
		st.pop();
	}

	if (size > answer.GetLength())
	{
		CString temp;
		temp.Format(L"%d", 0);
		while (size - answer.GetLength() > 0)
		{
			answer = temp + answer;
		}
	}

	return answer;
}

CString CHexEditorDlg::IntToBin(int number, int size) {
	CString answer;

	if (number < 0) 
	{
		number *= -1;
	}

	std::stack<char> st;

	while (number > 0)
	{
		int n = number % 2;
		st.push('0' + n);
		number /= 2;
	}
	while (!st.empty()) 
	{
		answer = answer + CString(st.top());
		st.pop();
	}

	if (size > answer.GetLength())
	{
		CString temp;
		temp.Format(L"%d", 0);
		while (size - answer.GetLength() > 0)
		{
			answer = temp + answer;
		}
	}
	return answer;
}

void CHexEditorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, DataTable);
}


BEGIN_MESSAGE_MAP(CHexEditorDlg, CDialogEx)
END_MESSAGE_MAP()


// Обработчики сообщений CHexEditorDlg

/*
CFile file;
	if (file.Open(wfdFile.fullName, CFile::modeRead | CFile::typeBinary | CFile::osNoBuffer) == 0) {
		//return FALSE;
	}
	if(file.Read((void*)&c, sizeof(char)) < 1) break;
	file.Close();
*/

/*
* 	WIN32_FIND_DATAW wfdW;
	HANDLE handle = CreateFileW(wfdFile.fullName, GENERIC_READ, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);//FindFirstFileW(wfdFile.fullName, &wfdW);


	if (INVALID_HANDLE_VALUE == handle) return FALSE;

	OVERLAPPED over = { 0 };
	over.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	over.Offset = 0;
	over.OffsetHigh = 0;
	if (!over.hEvent) return FALSE;

	if (counter > FFFF) break;

		int amountRead = 0;

		BYTE temp;

		bool result = ReadFile(handle, &temp, sizeof(char), (LPDWORD)&amountRead, &over);
		if (!result)
		{
			CloseHandle(handle);
			return FALSE;
		}
		if (result && amountRead == 0)
		{
			CloseHandle(handle);
			return TRUE;
		}
		c = temp;

		CloseHandle(handle);
*/