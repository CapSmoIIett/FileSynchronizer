﻿// ComparatorDlg.cpp: файл реализации
//

#include "pch.h"
#include "Application.h"
#include "ComparatorDlg.h"
#include "afxdialogex.h"
#include "ApplicationDlg.h"
#include "WFDTranslator.h"
#include <vector>

#define SIZE_COL_NAME	300
#define SIZE_COL_STATUS 100

// Диалоговое окно CComparatorDlg

IMPLEMENT_DYNAMIC(CComparatorDlg, CDialogEx)

CComparatorDlg::CComparatorDlg(std::vector<WFDFile>FFL, std::vector<WFDFile> FSL, 
	CString first, CString second, CWnd* pParent) :
	FilesFirstList (FFL),
	FilesSecondList(FSL),
	FirstDirectory(first),
	SecondDirectory(second),
	CDialogEx(IDD_DIALOG2, pParent)
	, LeftToRight(FALSE)
	, Equal(FALSE)
	, NotEqual(FALSE)
	, RightToLeft(FALSE)
{
	WithFolders = FALSE;
	WithContent = FALSE;
	WithoutDate = FALSE;
}

CComparatorDlg::~CComparatorDlg()
{	}

BOOL CComparatorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

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
	List.InsertColumn(0, L"", LVCFMT_LEFT, SIZE_COL_NAME);
	List.InsertColumn(1, L"", LVCFMT_LEFT, SIZE_COL_STATUS);
	List.InsertColumn(2, L"", LVCFMT_LEFT, SIZE_COL_NAME);

	LeftToRight = TRUE;
	Equal		= TRUE;
	NotEqual	= TRUE;
	RightToLeft = TRUE;

	Comparasions = CompareAll(FilesFirstList, FilesSecondList);

	UpdateList();

	UpdateData(false);

	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}

void CComparatorDlg::UpdateList()
{
	List.DeleteAllItems();
	for (int i = 0; i < Comparasions.size(); i++)
	{
		CString ratio;
		int item = 0;

		if (Comparasions[i].FirstFile.size == L"0" &&
			Comparasions[i].SecondFile.size == L"0")
		{
			if (!Equal) continue;
			item = List.InsertItem(i, Comparasions[i].FirstFile.name + L"\\", -1);
			List.SetItemText(item, 2, Comparasions[i].SecondFile.name + L"\\");
			continue;
		}

		switch (Comparasions[i].ratio)
		{
		default:
			ratio = L"!=";
			if (!NotEqual) continue;
			break;
		case 0:
			ratio = L"=";
			if (!Equal) continue;
			break;
		case 2:
			ratio = L"=>";
			if (!LeftToRight) continue;
			break;
		case 3:
			ratio = L"<=";
			if (!RightToLeft) continue;
			break;
		}

		CString nameOne = Comparasions[i].FirstFile.name;
		if (nameOne == L"") nameOne = L"."; 
		else nameOne += L"." + Comparasions[i].FirstFile.type;

		CString nameTwo = Comparasions[i].FirstFile.name;
		if (nameTwo == L"") nameTwo = L".";
		else nameTwo += L"." + Comparasions[i].FirstFile.type;

		item = List.InsertItem(i, nameOne, -1);
		List.SetItemText(item, 1, ratio);
		List.SetItemText(item, 2, nameTwo);
	}
}


void CComparatorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, List);
	DDX_Check(pDX, IDC_CHECK1, LeftToRight);
	DDX_Check(pDX, IDC_CHECK3, Equal);
	DDX_Check(pDX, IDC_CHECK4, NotEqual);
	DDX_Check(pDX, IDC_CHECK2, RightToLeft);
}

std::vector<ComparisonResult> CComparatorDlg::CompareAll(std::vector<WFDFile> FirstList, 
	std::vector<WFDFile> SecondList, CString firstDir, CString secondDir)
{
	if (firstDir == L"") firstDir = FirstDirectory;
	if (secondDir == L"") secondDir = SecondDirectory;

	std::vector<ComparisonResult> result;

	for (int i = 0; i < FirstList.size(); i++)
	{
		BOOL isAdd = FALSE;
		

		// Проверка директории 
		if (FirstList[i].size == L"0")		
		{
			if (!WithFolders) continue;

			BOOL found = FALSE;

			// Поиск одноименной директории
			int element = 0;
			for (; element < SecondList.size(); element++)
				if (FirstList[i].name == SecondList[element].name)
					if (FirstList[i].size == SecondList[element].size)
					{
						found = TRUE;
						break;
					}
			
			// Если справа нет подходящей директории
			if (!found)
			{
				// Если есть папка слева которой нет справа то мы просто при синхронизации копируем всю папку
				result.push_back(ComparisonResult(FirstList[i], WFDFile(secondDir), LEFTtoRIGHT));
				continue;
			}

			// Добавляем сами директории
			result.push_back(ComparisonResult(FirstList[i], SecondList[element], EQUAL));	

			// Просматриваем их содержимое
			std::vector<WFDFile> filesFirst = ViewDirectory(FirstList[i].fullName);
			std::vector<WFDFile> filesSecond = ViewDirectory(SecondList[element].fullName);;

			// Сравниваем директории 
			std::vector<ComparisonResult> temp = CompareAll(filesFirst, filesSecond, 
				FirstList[i].fullName, SecondList[element].fullName);
			for (int j = 0; j < temp.size(); j++)
			{
				result.push_back(temp[j]);
			}

			isAdd = TRUE;
		}
		
		// Сравнение идентичных файлов
		if (!isAdd)
			for (int j = 0; j < SecondList.size(); j++)
				if (FirstList[i].name == SecondList[j].name)
					if (FirstList[i].type == SecondList[j].type)
					{
						result.push_back(Compare(FirstList[i], SecondList[j]));
						isAdd = TRUE;
						break;
					}

		// Если во второй директории не был найден файл с тем же именем что и наш
		if (!isAdd)
			result.push_back(ComparisonResult(FirstList[i], WFDFile(secondDir), LEFTtoRIGHT));
	}
	for (int i = 0; i < SecondList.size(); i++)
	{
		bool hasAlready = false;
		
		for (int j = 0; j < result.size(); j++)
		{
			if (SecondList[i].fullName == result[j].SecondFile.fullName)
			{
				hasAlready = true;
				break;
			}
		}
		// Если такой файл еще не обрабатывался
		if (!hasAlready)
		{
			if (SecondList[i].size == L"0")
				if (!WithFolders) continue;
			result.push_back(ComparisonResult(WFDFile(firstDir), SecondList[i], RIGHTtoLEFT));	
		}
	}

	return result;
}

std::vector<WFDFile> CComparatorDlg::ViewDirectory(CString path)
{
	WIN32_FIND_DATA wfd;
	HANDLE const handle = FindFirstFileW(path + L"\\*", &wfd);
	std::vector<WFDFile> files; 

		do
		{
			if (CString(wfd.cFileName) == "." || CString(wfd.cFileName) == "..") continue;

			WFDTranslator translator(wfd, path);

			files.push_back(WFDFile(translator.getFullName(), translator.getName(),
				translator.getType(), translator.getSize(),
				translator.getDate(), translator.getAttr()));
		} while (NULL != FindNextFileW(handle, &wfd));

		FindClose(handle);
		return files;
}

ComparisonResult CComparatorDlg::Compare(WFDFile first, WFDFile second)
{
	if (!WithoutDate) 
		if (first.date != second.date)
			return ComparisonResult(first, second, NOTEQUAL);
	
	if (first.size > second.size)
		return ComparisonResult(first, second, LEFTtoRIGHT);
	if (first.size < second.size)
		return ComparisonResult(first, second, RIGHTtoLEFT);
	if (first.attr != second.attr)
		return ComparisonResult(first, second, NOTEQUAL);
	
	if (WithContent)
	{
		int len = _ttoi(first.size);		// Размеры одинаковы

		CFile fileFirst;
		if (fileFirst.Open(first.fullName, CFile::modeRead | CFile::typeBinary) == 0)
		{
			return ComparisonResult(first, second, NOTEQUAL);
		}

		CFile fileSecond;
		if (fileSecond.Open(second.fullName, CFile::modeRead | CFile::typeBinary) == 0)
		{
			return ComparisonResult(first, second, NOTEQUAL);
		}

		unsigned char* pointerFirst  = new unsigned char[len];
		unsigned char* pointerSecond = new unsigned char[len];

		if (fileFirst.Read((void*)pointerFirst, sizeof(char) * len) < len) 
			return ComparisonResult(first, second, NOTEQUAL);
		if (fileSecond.Read((void*)pointerSecond, sizeof(char) * len) < len)
			return ComparisonResult(first, second, NOTEQUAL);

		for (int i = 0; i < len; i++)
			if (pointerFirst[i] != pointerSecond[i])
			{
				fileFirst.Close();
				fileSecond.Close();
				return ComparisonResult(first, second, NOTEQUAL);
			}
		fileFirst.Close();
		fileSecond.Close();
	}
	
	return ComparisonResult(first, second, EQUAL);
}



void CComparatorDlg::setWithFolders(BOOL b)
{
	WithFolders = b;
}
void CComparatorDlg::setWithContent(BOOL b)
{
	WithContent = b;
}
void CComparatorDlg::setWithoutDate(BOOL b)
{
	WithoutDate = b;
}

BEGIN_MESSAGE_MAP(CComparatorDlg, CDialogEx)
	ON_BN_CLICKED(IDC_CHECK1, &CComparatorDlg::ChangeCheckBoxLeftToRight)
	ON_BN_CLICKED(IDC_CHECK2, &CComparatorDlg::ChangeCheckRightToLeft)
	ON_BN_CLICKED(IDC_CHECK3, &CComparatorDlg::ChangeCheckBoxEqual)
	ON_BN_CLICKED(IDC_CHECK4, &CComparatorDlg::ChangeCheckNotEqual)
	ON_BN_CLICKED(IDC_BUTTON1, &CComparatorDlg::SyncLeftToRight)
	ON_BN_CLICKED(IDC_BUTTON2, &CComparatorDlg::SyncRightToLeft)
END_MESSAGE_MAP()



// Обработчики сообщений CComparatorDlg

void CComparatorDlg::ChangeCheckBoxLeftToRight()
{
	LeftToRight = !LeftToRight;
	UpdateList();
	UpdateData(false);
}

void CComparatorDlg::ChangeCheckBoxEqual()
{
	Equal = !Equal;
	UpdateList();
	UpdateData(false);
}

void CComparatorDlg::ChangeCheckNotEqual()
{
	NotEqual = !NotEqual;
	UpdateList();
	UpdateData(false);
}

void CComparatorDlg::ChangeCheckRightToLeft()
{
	RightToLeft = !RightToLeft;
	UpdateList();
	UpdateData(false);
}


void CComparatorDlg::SyncLeftToRight()
{
	for (ComparisonResult i : Comparasions)
	{
		switch (i.ratio)
		{
		default: continue;

		case NOTEQUAL:
			{
				BOOL FailIfExists = FALSE;
				CopyFile(i.FirstFile.fullName, i.SecondFile.fullName, FailIfExists);
				if (!WithoutDate)
				{
					// чтобы было одинаковое время последнего доступа
					CopyFile(i.SecondFile.fullName, i.FirstFile.fullName, FailIfExists);
				}
				break;
			}
			
		case LEFTtoRIGHT:
			{
				BOOL FailIfExists = FALSE;
				CString secondFileName;

				if (i.FirstFile.size == L"0")
				{
					// копируем всю папку
					i.FirstFile.fullName += L"\\\0\0";
					secondFileName = i.SecondFile.fullName + L"\\\0\0";

					SHFILEOPSTRUCT fos = { 0 };
					fos.wFunc = FO_COPY;

					TCHAR newFrom[MAX_PATH];
					_tcscpy_s(newFrom, i.FirstFile.fullName.GetBuffer());
					newFrom[_tcsclen(i.FirstFile.fullName.GetBuffer()) + 1] = NULL;
					fos.pFrom = newFrom;

					TCHAR newTo[MAX_PATH];
					_tcscpy_s(newTo, secondFileName.GetBuffer());
					newTo[_tcsclen(secondFileName.GetBuffer()) + 1] = NULL;
					fos.pTo = newTo;

					fos.fFlags = FOF_SILENT | FOF_NOCONFIRMATION | FOF_NOERRORUI | FOF_NOCONFIRMMKDIR;
					SHFileOperation(&fos);

					break;
				}

				if (i.SecondFile.name == L"")
				{
					if (i.SecondFile.fullName != L"")
						secondFileName = i.SecondFile.fullName + "\\" + i.FirstFile.name;
					if (i.FirstFile.type != L"" && i.FirstFile.type != L" ")
						secondFileName += L"." + i.FirstFile.type;	
				}
				CopyFile(i.FirstFile.fullName, secondFileName, FailIfExists);
				if (!WithoutDate)
				{
					// чтобы было одинаковое время последнего доступа
					CopyFile(secondFileName, i.FirstFile.fullName, FailIfExists);
				}
				break;
			}
			
		}
	}
	/*Comparasions = CompareAll(FilesFirstList, FilesSecondList);
	UpdateList();
	UpdateData(false);*/
	SendMessage(WM_CLOSE);
}


void CComparatorDlg::SyncRightToLeft()
{
	for (ComparisonResult i : Comparasions)
	{
		switch (i.ratio)
		{
		default: continue;

		case NOTEQUAL:
		{
			BOOL FailIfExists = FALSE;
			CopyFile(i.FirstFile.fullName, i.SecondFile.fullName, FailIfExists);
			if (!WithoutDate)
			{
				// чтобы было одинаковое время последнего доступа
				CopyFile(i.SecondFile.fullName, i.FirstFile.fullName, FailIfExists);
			}
			break;
		}

		case RIGHTtoLEFT:
		{
			BOOL FailIfExists = FALSE;
			CString firstFileName;

			if (i.SecondFile.size == L"0")
			{
				// копируем всю папку
				i.SecondFile.fullName += L"\\\0\0";
				firstFileName = i.FirstFile.fullName + L"\\\0\0";

				SHFILEOPSTRUCT fos = { 0 };
				fos.wFunc = FO_COPY;

				TCHAR newFrom[MAX_PATH];
				_tcscpy_s(newFrom, i.SecondFile.fullName.GetBuffer());
				newFrom[_tcsclen(i.SecondFile.fullName.GetBuffer()) + 1] = NULL;
				fos.pFrom = newFrom;

				TCHAR newTo[MAX_PATH];
				_tcscpy_s(newTo, firstFileName.GetBuffer());
				newTo[_tcsclen(firstFileName.GetBuffer()) + 1] = NULL;
				fos.pTo = newTo;

				fos.fFlags = FOF_SILENT | FOF_NOCONFIRMATION | FOF_NOERRORUI | FOF_NOCONFIRMMKDIR;
				SHFileOperation(&fos);

				break;
			}

			if (i.FirstFile.name == L"")
			{
				if (i.FirstFile.fullName != L"")
					firstFileName = i.FirstFile.fullName + "\\" + i.SecondFile.name;
				if (i.SecondFile.type != L"" && i.SecondFile.type != L" ")
					firstFileName += L"." + i.SecondFile.type;
			}
			CopyFile(i.SecondFile.fullName, firstFileName, FailIfExists);
			if (!WithoutDate)
			{
				// чтобы было одинаковое время последнего доступа
				CopyFile(firstFileName, i.SecondFile.fullName, FailIfExists);
			}
			break;
		}

		}
	}
	SendMessage(WM_CLOSE);
}
