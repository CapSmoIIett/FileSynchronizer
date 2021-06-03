#include "pch.h"
#include "framework.h"
#include "Application.h"
#include "ApplicationDlg.h"
#include "AboutDlg.h"
#include "afxdialogex.h"
#include "WFDTranslator.h"
#include "HexEditorDlg.h"


// ѕри добавлении кнопки свертывани€ в диалоговое окно нужно воспользоватьс€ приведенным ниже кодом,
// чтобы нарисовать значок.  ƒл€ приложений MFC, использующих модель документов или представлений,
// это автоматически выполн€етс€ рабочей областью.
void CApplicationDlg::OnPaint()
{
	UpdateAll(ReadyToSync);
	if (IsIconic())
	{
		CPaintDC dc(this); // контекст устройства дл€ рисовани€

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ¬ыравнивание значка по центру клиентского пр€моугольника
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;

		GetClientRect(&rect);

		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Ќарисуйте значок
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// —истема вызывает эту функцию дл€ получени€ отображени€ курсора при перемещении
// свернутого окна.
HCURSOR CApplicationDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CApplicationDlg::FirstDirectoryAddressEdit()
{
	if (FirstDirectoryAddress.IsEmpty()) return;

	UpdateData(true);
	UpdateAll();
	UpdateData(false);
}

void CApplicationDlg::SecondDirectoryAddressEdit()
{
	if (SecondDirectoryAddress.IsEmpty()) return;
	
	UpdateData(true);
	UpdateAll();
	UpdateData(false);
}

void CApplicationDlg::ClickedButtonFirstView()
{
	CFolderPickerDialog dlg;

	auto answer = dlg.DoModal();
	if (answer == IDOK)
	{
		FirstDirectoryAddress = dlg.m_ofn.lpstrFile;
	}

	UpdateAll();
	UpdateData(false);		// из переменных в управление
}

void CApplicationDlg::ClickedButtonSecondView()
{
	CFolderPickerDialog dlg;

	auto answer = dlg.DoModal();
	if (answer == IDOK)
	{
		SecondDirectoryAddress = dlg.m_ofn.lpstrFile;
	}

	UpdateAll();
	UpdateData(false);		// из переменных в управление
}

void CApplicationDlg::SelectElementFirstTable(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	if (!ReadyToSync)
	{
		WFDFile file = FilesFirstList[pNMIA->iItem];

		if (file.size == L"0")
		{
			FirstDirectoryAddress += L"\\" + file.name;
			UpdateAll();
			UpdateData(false);
		}
		else
		{
			CHexEditorDlg editor(file, this);
			editor.DoModal();
		}
	}

	*pResult = 0;
}

void CApplicationDlg::SelectElementSecondTable(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	if (!ReadyToSync)
	{
		WFDFile file = FilesSecondList[pNMIA->iItem];

		if (file.size == L"0")
		{
			SecondDirectoryAddress += L"\\" + file.name;
			UpdateList(ListSecondFolder, SecondDirectoryAddress, FilesSecondList);
			UpdateData(false);
		}
		else
		{
			CHexEditorDlg editor(file, this);
			editor.DoModal();
		}
	}

	*pResult = 0;
}

void CApplicationDlg::SelectElementCompaComparisonTable(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	if (ReadyToSync)
	{
		auto file = Comparasions[pNMIA->iItem - 1];

		switch (file.ratio)
		{
		default:
		{
			break;
		}
		case NOTEQUAL:
		{	
			POINT point;

			GetCursorPos(&point);

			numberSelectedItem = pNMIA->iItem - 1;
			PopupMenu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON,
				point.x, point.y, this);

			break;
		}
		case LEFTtoRIGHT:
		{
			syncLeftToRight(file.FirstFile, file.SecondFile);
			break;
		}
		case RIGHTtoLEFT:
		{
			syncRightToLeft(file.FirstFile, file.SecondFile);
			break;
		}
		}

		Comparasions = CompareAll(FilesFirstList, FilesSecondList);
		UpdateAll(ReadyToSync);
	}
}

void CApplicationDlg::CompareFolders()
{
	UpdateData(true);

	Comparasions = CompareAll(FilesFirstList, FilesSecondList);

	UpdateAll(TRUE);
	
	UpdateData(false);
}

void CApplicationDlg::SynchronizeLeftToRight()
{
	for (ComparisonResult comparasions : Comparasions)
	{
		switch (comparasions.ratio)
		{
		default: continue;

		case NOTEQUAL:
		{
			if (!NotEqual)break;								// ѕропускаем если это не выбранно пользователем (checkbox)
			syncLeftToRight(comparasions.FirstFile, comparasions.SecondFile);
			break;
		}

		case LEFTtoRIGHT:
		{
			if (!LeftToRight) break;
			syncLeftToRight(comparasions.FirstFile, comparasions.SecondFile);
			break;
		}
		}
	}

	Comparasions = CompareAll(FilesFirstList, FilesSecondList);
	UpdateAll(TRUE);
	UpdateData(false);
}

void CApplicationDlg::SynchronizeRightToLeft()
{
	for (ComparisonResult comparasions : Comparasions)
	{
		switch (comparasions.ratio)
		{
		default: continue;

		case NOTEQUAL:
		{
			if (!NotEqual)break;								// ѕропускаем если это не выбранно пользователем (checkbox)
			syncRightToLeft(comparasions.FirstFile, comparasions.SecondFile);
			break;
		}
		case RIGHTtoLEFT:
		{
			if (!RightToLeft) break;
			syncRightToLeft(comparasions.FirstFile, comparasions.SecondFile);
			break;
		}
		}
	}
	
	Comparasions = CompareAll(FilesFirstList, FilesSecondList);
	UpdateAll(TRUE);
	UpdateData(false);
}

void CApplicationDlg::BeginScrollListFirst(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	LPSCROLLINFO info = 0;

	if (ReadyToSync && !ScrollMutex)
	{
		ScrollMutex = FIRST_SCROLL;
		int offset = 0;
		int pos = ListFirstFolder.GetScrollPos(SB_VERT);

		
		offset = (pos - ScrollPosition) * GetItemHeight(ListSecondFolder);
		ListSecondFolder.Scroll(CSize(0, offset));
		ListSecondFolder.SetScrollPos(SB_VERT, pos);

		UpdateComparisonList(pos);

		ScrollPosition = pos;
		ScrollMutex = NOBODY_SCROLL;
	}

	*pResult = 0;
}

void CApplicationDlg::BeginScrollListSecond(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	LPSCROLLINFO info = 0;

	if (ReadyToSync && !ScrollMutex)
	{
		ScrollMutex = SECOND_SCROLL;
		int offset = 0;
		int pos = ListSecondFolder.GetScrollPos(SB_VERT);

		offset = (pos - ScrollPosition) * GetItemHeight(ListFirstFolder);
		ListFirstFolder.Scroll(CSize(0, offset));
		ListFirstFolder.SetScrollPos(SB_VERT, pos);

		UpdateComparisonList(pos);

		ScrollPosition = pos;
		ScrollMutex = NOBODY_SCROLL;
	}

	*pResult = 0;
}

void CApplicationDlg::ChangeCheckBoxLeftToRight()
{
	LeftToRight = !LeftToRight;
	Comparasions = CompareAll(FilesFirstList, FilesSecondList);
	UpdateAll(ReadyToSync);
}

void CApplicationDlg::ChangeCheckBoxEqual()
{
	Equal = !Equal;
	Comparasions = CompareAll(FilesFirstList, FilesSecondList);
	UpdateAll(ReadyToSync);
}

void CApplicationDlg::ChangeCheckNotEqual()
{
	NotEqual = !NotEqual;
	Comparasions = CompareAll(FilesFirstList, FilesSecondList);
	UpdateAll(ReadyToSync);
}

void CApplicationDlg::ChangeCheckRightToLeft()
{
	RightToLeft = !RightToLeft;
	Comparasions = CompareAll(FilesFirstList, FilesSecondList);
	UpdateAll(ReadyToSync);
}

void CApplicationDlg::OnNMCustomdraw(NMHDR* pNMHDR, LRESULT* pResult)
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
		if (pLVCD->nmcd.dwItemSpec % 2 == 0)
			pLVCD->clrTextBk = GREY;
		else
			pLVCD->clrTextBk = WHITE;

		if (ReadyToSync)
		{
			if (pLVCD->nmcd.dwItemSpec >= Comparasions.size())
				break;

			switch (Comparasions[pLVCD->nmcd.dwItemSpec].ratio)
			{
			case EQUAL:
			{
				pLVCD->clrText = BLACK;
				break;
			}
			case NOTEQUAL:
			{
				auto file = Comparasions[pLVCD->nmcd.dwItemSpec];
				if (file.isDifContent())		// If files has different content color all columns
				{
					pLVCD->clrText = RED;
					return;
				}
				if (file.isDifType() &&  pLVCD->iSubItem == 1)
					pLVCD->clrText = RED;
				else if (file.isDifSize() && pLVCD->iSubItem == 2)
					pLVCD->clrText = RED;
				else if (file.isDifDate() && pLVCD->iSubItem == 3)
					pLVCD->clrText = RED;
				else if (file.isDifAttr() && pLVCD->iSubItem == 4)
					pLVCD->clrText = RED;
				else
					pLVCD->clrText = BLACK;
				break;
			}
			case LEFTtoRIGHT:
			{
				pLVCD->clrText = GREEN;
				break;
			}
			case RIGHTtoLEFT:
			{
				pLVCD->clrText = BLUE;
				break;
			}
			}
		}
	}
	break;
	}
	return;
}

void CApplicationDlg::OnNMCustomdrawListComparnResult(NMHDR* pNMHDR, LRESULT* pResult)
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
		if (ReadyToSync)
		{	
			if (pLVCD->nmcd.dwItemSpec >= Comparasions.size() + 1 ||
				pLVCD->nmcd.dwItemSpec == 0)								
				break;													// Central list has one empty row (0) (it's instead head of table)

			switch (Comparasions[pLVCD->nmcd.dwItemSpec - 1].ratio)
			{
			case EQUAL:
			{
				pLVCD->clrText = BLACK;
				break;
			}
			case NOTEQUAL:
			{
				pLVCD->clrText = RED;
				break;
			}
			case LEFTtoRIGHT:
			{
				pLVCD->clrText = GREEN;
				break;
			}
			case RIGHTtoLEFT:
			{
				pLVCD->clrText = BLUE;
				break;
			}
			}
		}
	}
	break;
	}
	return;
}

void CApplicationDlg::OnActivate(UINT, CWnd*, BOOL)
{
	if (ReadyToSync)
		Comparasions = CompareAll(FilesFirstList, FilesSecondList);

	UpdateAll(ReadyToSync);
}

void CApplicationDlg::MenuLeftToRight()
{
	auto file = Comparasions[numberSelectedItem];

	syncLeftToRight(file.FirstFile, file.SecondFile);

	numberSelectedItem = 0;

	Comparasions = CompareAll(FilesFirstList, FilesSecondList);
	UpdateAll(ReadyToSync);
}

void CApplicationDlg::MenuRightToLeft()
{
	auto file = Comparasions[numberSelectedItem];

	syncRightToLeft(file.FirstFile, file.SecondFile);

	numberSelectedItem = 0;

	Comparasions = CompareAll(FilesFirstList, FilesSecondList);
	UpdateAll(ReadyToSync);
}

void CApplicationDlg::ChangeCheckBoxWithFolders()
{
	WithFolders = !WithFolders;

	Comparasions = CompareAll(FilesFirstList, FilesSecondList);
	UpdateAll(ReadyToSync);
}

void CApplicationDlg::ChangeCheckBoxWithContent()
{
	WithContent = !WithContent;

	Comparasions = CompareAll(FilesFirstList, FilesSecondList);
	UpdateAll(ReadyToSync);
}

void CApplicationDlg::ChangeCheckBoxWithoutDate()
{
	WithoutDate = !WithoutDate;

	Comparasions = CompareAll(FilesFirstList, FilesSecondList);
	UpdateAll(ReadyToSync);
}
