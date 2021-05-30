#include "pch.h"
#include "framework.h"
#include "Application.h"
#include "ApplicationDlg.h"
#include "AboutDlg.h"
#include "afxdialogex.h"
#include "WFDTranslator.h"
#include "HexEditorDlg.h"
#include "ComparatorDlg.h"



// ��� ���������� ������ ����������� � ���������� ���� ����� ��������������� ����������� ���� �����,
// ����� ���������� ������.  ��� ���������� MFC, ������������ ������ ���������� ��� �������������,
// ��� ������������� ����������� ������� ��������.
void CApplicationDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �������� ���������� ��� ���������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ������������ ������ �� ������ ����������� ��������������
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;

		GetClientRect(&rect);

		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ��������� ������
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ������� �������� ��� ������� ��� ��������� ����������� ������� ��� �����������
// ���������� ����.
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
	UpdateData(false);		// �� ���������� � ����������
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
	UpdateData(false);		// �� ���������� � ����������2
}

void CApplicationDlg::SelectElementFirstTable(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	//WFDFile file = FilesFirstList[FilesFirstList.size() - pNMIA->iItem - 1];
	WFDFile file = FilesFirstList[pNMIA->iItem];

	if (!ReadyToSync)
	{
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

	//WFDFile file = FilesSecondList[FilesSecondList.size() - pNMIA->iItem - 1];
	WFDFile file = FilesSecondList[pNMIA->iItem];

	if (!ReadyToSync)
	{
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

void CApplicationDlg::CompareFolders()
{
	UpdateData(true);
	Comparasions = CompareAll(FilesFirstList, FilesSecondList);
	UpdateAll(TRUE);
	UpdateData(false);
	/*
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
	UpdateData(false);\*/
}

void CApplicationDlg::SynchronizeLeftToRight()
{
	for (ComparisonResult i : Comparasions)
	{
		switch (i.ratio)
		{
		default: continue;

		case NOTEQUAL:
		{
			if (!NotEqual)break;								// ���������� ���� ��� �� �������� ������������� (checkbox)
			syncLeftToRight(i.FirstFile, i.SecondFile);
			break;
		}

		case LEFTtoRIGHT:
		{
			if (!LeftToRight) break;
			syncLeftToRight(i.FirstFile, i.SecondFile);
			break;
		}
		case RIGHTtoLEFT:
		{
			
		}
		}
	}
	/*Comparasions.clear();
	ListComparisonResults.DeleteAllItems();*/

	Comparasions = CompareAll(FilesFirstList, FilesSecondList);
	UpdateAll(TRUE);
	UpdateData(false);
}

void CApplicationDlg::SynchronizeRightToLeft()
{
	for (ComparisonResult i : Comparasions)
	{
		switch (i.ratio)
		{
		default: continue;

		case NOTEQUAL:
		{
			if (!NotEqual)break;								// ���������� ���� ��� �� �������� ������������� (checkbox)
			syncRightToLeft(i.FirstFile, i.SecondFile);
			break;
		}

		case LEFTtoRIGHT:
		{
		
		}
		case RIGHTtoLEFT:
		{
			if (!RightToLeft) break;
			syncRightToLeft(i.FirstFile, i.SecondFile);
			break;
		}
		}
	}
	/*Comparasions.clear();
	ListComparisonResults.DeleteAllItems();*/

	
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
	UpdateAll(ReadyToSync);

}

void CApplicationDlg::ChangeCheckBoxEqual()
{
	Equal = !Equal;
	UpdateAll(ReadyToSync);
}

void CApplicationDlg::ChangeCheckNotEqual()
{
	NotEqual = !NotEqual;
	UpdateAll(ReadyToSync);
}

void CApplicationDlg::ChangeCheckRightToLeft()
{
	RightToLeft = !RightToLeft;
	UpdateAll(ReadyToSync);
}

void CApplicationDlg::OnNMCustomdraw(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLVCUSTOMDRAW pLVCD = reinterpret_cast<LPNMLVCUSTOMDRAW>(pNMHDR);
	NMTVCUSTOMDRAW* pNVCD = reinterpret_cast<NMTVCUSTOMDRAW*>(pNMHDR);
	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	if (ReadyToSync)
	{
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
			/*static int n = 0;//TODO ����� �������� ����� ��������
			if (n >= (Comparasions.size() - 1) || n < 0)
			{
				n = 0;
			}
			else
			{
				n++;
			}
			switch (Comparasions[n].ratio)		// ��� ��� � ������� 5 ���������
			{
			case LEFTtoRIGHT:
			{
				pLVCD->clrText = RGB(0, 255, 0);
				break;
			}
			case NOTEQUAL:
			{
				pLVCD->clrText = RGB(255, 0, 0);
				break;
			}
			case RIGHTtoLEFT:
			{
				pLVCD->clrText = RGB(0, 0, 255);
				break;
			}
			default:
			{
				break;
			}
			}*/
		}
		break;
		}
	}//*/
	return;
}