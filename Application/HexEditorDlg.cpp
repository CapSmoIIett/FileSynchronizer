#include <fstream>
#include "pch.h"
#include "Application.h"
#include "HexEditorDlg.h"
#include "afxdialogex.h"

#include <sys/stat.h>
#include <stack>

#define AMOUNT_COL		48		// 16 - линейка, 16 - ascii представления, 16 - 16-ое значение 
#define COL_RULER		16
#define COL_ASCII		16
#define COL_HEX			16
#define FFFFFFFF		4294967295	// 0FFFFFFFFh in dec
#define SIZE_COL		20


IMPLEMENT_DYNAMIC(CHexEditorDlg, CDialogEx)


CHexEditorDlg::CHexEditorDlg(WFDFile wfd, CWnd* pParent) :
	CDialogEx(IDD_DIALOG1, pParent),
	CurrentFile(wfd)
{	
	// Установка иконки
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	try
	{
		handle = CreateFile(CurrentFile.fullName, GENERIC_READ, 0, NULL,
			OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (handle == INVALID_HANDLE_VALUE) return;


		HANDLE mappFile = CreateFileMapping(handle, NULL, PAGE_READONLY, 0, 0, NULL);
		if (mappFile == NULL)
		{
			CloseHandle(handle);
			return;
		}

		pointer = (unsigned char*)MapViewOfFile(mappFile, FILE_MAP_READ, 0, 0, 0);
		if (pointer == NULL)
		{
			CloseHandle(handle);
			return;
		}
	}
	catch(...){
		SendMessage(WM_CLOSE);
		return;
	}
}

CHexEditorDlg::~CHexEditorDlg()
{	
	CloseHandle(handle);
}

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

	// Задает значок для этого диалогового окна.  Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок

	DataTable.SetExtendedStyle(LVS_EX_DOUBLEBUFFER);

	// Наша таблица - виртуальная (это необходимо для ускорения обработки данных)
	
	// Устанавливаем количество элементов

	int amountOfRow = 0;

	if (_ttoi(CurrentFile.size) % COL_RULER == 0)
		amountOfRow = _ttoi(CurrentFile.size) / COL_RULER;
	else
		amountOfRow = _ttoi(CurrentFile.size) / COL_RULER + 1;
		
	DataTable.SetItemCount(amountOfRow);

	for (int i = 0; i < AMOUNT_COL; i++)	// Создание колонок таблицы
		DataTable.InsertColumn(i, L"", LVCFMT_LEFT, SIZE_COL);

	UpdateData(false);

	return TRUE;
}

CString CHexEditorDlg::IntToHex(int number, int size)
{
	CString answer;

	if (number < 0) 
	{
		number *= -1;
	}

	std::stack<char> stack;

	while (number > 0)
	{
		int n = number % 16;
		switch (n)
		{
		case 15:
			stack.push('F');
			break;
		case 14:
			stack.push('E');
			break;
		case 13:
			stack.push('D');
			break;
		case 12:
			stack.push('C');
			break;
		case 11:
			stack.push('B');
			break;
		case 10:
			stack.push('A');
			break;
		default:
			stack.push('0' + n);
		}
		number /= 16;
	}

	while (!stack.empty()) {
		answer = answer + CString(stack.top());
		stack.pop();
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
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST1, &CHexEditorDlg::OnGetdispinfoList)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST1, &CHexEditorDlg::OnNMCustomdraw)
END_MESSAGE_MAP()

void CHexEditorDlg::OnGetdispinfoList(NMHDR* pNMHDR, LRESULT* pResult)
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

	//Create a pointer to the item
	LV_ITEM* pItem = &(pDispInfo)->item;

	//Which item number?
	int itemid = pItem->iItem;

	
	unsigned char symbol = 0;

	//Do the list need text information?
	if (pItem->mask & LVIF_TEXT)
	{
		CString text;

		// определяем какая информация нужна 
		if (pItem->iSubItem >= 0 &&
			pItem->iSubItem < COL_RULER)
		{
			CString ruler = IntToHex(pItem->iItem, COL_RULER);
			text = ruler[pItem->iSubItem];
		}

		if (pItem->iSubItem >= COL_RULER &&
			pItem->iSubItem < COL_RULER + COL_ASCII)
		{
			int numberOfSymbol = (pItem->iItem * 16) + (pItem->iSubItem - COL_RULER);
			if (numberOfSymbol < _ttoi(CurrentFile.size))
			{
				symbol = pointer[numberOfSymbol];
				text = CString((wchar_t)symbol);
			}
			else
			{
				text = L" ";
			}
		}

		if (pItem->iSubItem >= COL_RULER + COL_ASCII &&
			pItem->iSubItem < COL_RULER + COL_ASCII + COL_HEX)
		{
			int numberOfSymbol = (pItem->iItem * 16) + (pItem->iSubItem - COL_RULER - COL_ASCII);
			if (numberOfSymbol < _ttoi(CurrentFile.size))
			{
				symbol = pointer[numberOfSymbol];
				text = IntToHex(symbol);
			}
			else
			{
				text = L" ";
			}
		}

		//Copy the text to the LV_ITEM structure
		//Maximum number of characters is in pItem->cchTextMax
		lstrcpyn(pItem->pszText, text, pItem->cchTextMax);
	}
	*pResult = 0;
}

BOOL CHexEditorDlg::PreTranslateMessage(MSG* pMsg) {
	if (pMsg->message == WM_KEYDOWN) {
		if (pMsg->wParam == VK_ESCAPE)  return TRUE;
		if (pMsg->wParam == VK_RETURN)  return TRUE;
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CHexEditorDlg::OnNMCustomdraw(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLVCUSTOMDRAW pLVCD = reinterpret_cast<LPNMLVCUSTOMDRAW>(pNMHDR);

	switch (pLVCD->nmcd.dwDrawStage)
	{
	case CDDS_PREPAINT:
		*pResult = CDRF_NOTIFYITEMDRAW;
		break;

	case CDDS_ITEMPREPAINT:
		*pResult = CDRF_NOTIFYSUBITEMDRAW;
		break;

	case (CDDS_ITEMPREPAINT | CDDS_SUBITEM):
	{
		if (pLVCD->iSubItem >= 0 &&
			pLVCD->iSubItem < COL_RULER)
		{
			pLVCD->clrTextBk = RGB(245, 245, 245);
			break;
		}
		if (pLVCD->iSubItem >= COL_RULER &&
			pLVCD->iSubItem < COL_RULER + COL_ASCII)
		{
			pLVCD->clrTextBk = RGB(255, 255, 255);
			break;
		}
		if (pLVCD->iSubItem >= COL_RULER + COL_ASCII &&
			pLVCD->iSubItem < COL_RULER + COL_ASCII + COL_HEX)
		{
			pLVCD->clrTextBk = RGB(245, 245, 245);
			break;
		}

	}
	break;
	}
	return;
}