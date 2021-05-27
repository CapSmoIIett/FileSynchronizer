#include "pch.h"
#include "framework.h"
#include "Application.h"
#include "ApplicationDlg.h"
#include "AboutDlg.h"
#include "afxdialogex.h"
#include "WFDTranslator.h"
#include "HexEditorDlg.h"
#include "ComparatorDlg.h"

std::vector<ComparisonResult> CApplicationDlg::CompareAll(std::vector<WFDFile> FirstList,
	std::vector<WFDFile> SecondList, CString firstDir, CString secondDir)
{
	if (firstDir == L"") firstDir = FirstDirectoryAddress;
	if (secondDir == L"") secondDir = SecondDirectoryAddress;

	std::vector<ComparisonResult> result;

	for (int i = 0; (size_t)i < FirstList.size(); i++)
	{
		BOOL isAdd = FALSE;


		// Проверка директории 
		if (FirstList[i].size == L"0")
		{
			if (!WithFolders) continue;

			BOOL found = FALSE;

			// Поиск одноименной директории
			int element = 0;
			for (; (size_t)element < SecondList.size(); element++)
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
			for (int j = 0; (size_t)j < temp.size(); j++)
			{
				result.push_back(temp[j]);
			}

			isAdd = TRUE;
		}

		// Сравнение идентичных файлов
		if (!isAdd)
			for (int j = 0; (size_t)j < SecondList.size(); j++)
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
	for (int i = 0; (size_t)i < SecondList.size(); i++)
	{
		bool hasAlready = false;

		for (int j = 0; (size_t)j < result.size(); j++)
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

std::vector<WFDFile> CApplicationDlg::ViewDirectory(CString path)
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

ComparisonResult CApplicationDlg::Compare(WFDFile first, WFDFile second)
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

		unsigned char* pointerFirst = new unsigned char[len];
		unsigned char* pointerSecond = new unsigned char[len];

		if (fileFirst.Read((void*)pointerFirst, sizeof(char) * len) < (size_t)len)
			return ComparisonResult(first, second, NOTEQUAL);
		if (fileSecond.Read((void*)pointerSecond, sizeof(char) * len) < (size_t)len)
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

void CApplicationDlg::UpdateComparisonList(int begin)
{
	ListComparisonResults.DeleteAllItems();

	// Отступ (вместо шапки таблицы)
	ListComparisonResults.InsertItem(0,  L" ", -1);
	

	// TODO заменить i и k на подходящие имена
	// i - номер элемента в списке
	// k - номер эл. в таблице
	for (int i = begin, k = 0; (size_t)i < Comparasions.size(); i++, k++)
	{
		CString ratio;
		int item = 0;

		if (Comparasions[i].FirstFile.size == L"0" &&
			Comparasions[i].SecondFile.size == L"0")
		{
			ListComparisonResults.InsertItem(k + 1, L" ", -1);
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

		// Из за отступа
		ListComparisonResults.InsertItem(k + 1, ratio, -1);
	}//*/
}

void CApplicationDlg::synchronizeNotEqual(WFDFile first, WFDFile second)
{
	BOOL FailIfExists = FALSE;
	CopyFile(first.fullName, second.fullName, FailIfExists);
	if (!WithoutDate)
	{
		// чтобы было одинаковое время последнего доступа
		CopyFile(second.fullName, first.fullName, FailIfExists);
	}
}

void CApplicationDlg::synchronizeLeftToRight(WFDFile first, WFDFile second)
{
	BOOL FailIfExists = FALSE;
	CString secondFileName;

	if (first.size == L"0")
	{
		// копируем всю папку
		first.fullName += L"\\\0\0";
		secondFileName = second.fullName + L"\\\0\0";

		SHFILEOPSTRUCT fos = { 0 };
		fos.wFunc = FO_COPY;

		TCHAR newFrom[MAX_PATH];
		_tcscpy_s(newFrom, first.fullName.GetBuffer());
		newFrom[_tcsclen(first.fullName.GetBuffer()) + 1] = NULL;
		fos.pFrom = newFrom;

		TCHAR newTo[MAX_PATH];
		_tcscpy_s(newTo, secondFileName.GetBuffer());
		newTo[_tcsclen(secondFileName.GetBuffer()) + 1] = NULL;
		fos.pTo = newTo;

		fos.fFlags = FOF_SILENT | FOF_NOCONFIRMATION | FOF_NOERRORUI | FOF_NOCONFIRMMKDIR;
		SHFileOperation(&fos);

		return;
	}

	if (second.name == L"")
	{
		if (second.fullName != L"")
			secondFileName = second.fullName + "\\" + first.name;
		if (first.type != L"" && first.type != L" ")
			secondFileName += L"." + first.type;
	}
	CopyFile(first.fullName, secondFileName, FailIfExists);
	if (!WithoutDate)
	{
		// чтобы было одинаковое время последнего доступа
		CopyFile(secondFileName, first.fullName, FailIfExists);
	}
}

void CApplicationDlg::synchronizeRightToLeft(WFDFile first, WFDFile second)
{
	BOOL FailIfExists = FALSE;
	CString firstFileName;

	if (second.size == L"0")
	{
		// копируем всю папку
		second.fullName += L"\\\0\0";
		firstFileName = first.fullName + L"\\\0\0";

		SHFILEOPSTRUCT fos = { 0 };
		fos.wFunc = FO_COPY;

		TCHAR newFrom[MAX_PATH];
		_tcscpy_s(newFrom, second.fullName.GetBuffer());
		newFrom[_tcsclen(second.fullName.GetBuffer()) + 1] = NULL;
		fos.pFrom = newFrom;

		TCHAR newTo[MAX_PATH];
		_tcscpy_s(newTo, firstFileName.GetBuffer());
		newTo[_tcsclen(firstFileName.GetBuffer()) + 1] = NULL;
		fos.pTo = newTo;

		fos.fFlags = FOF_SILENT | FOF_NOCONFIRMATION | FOF_NOERRORUI | FOF_NOCONFIRMMKDIR;
		SHFileOperation(&fos);

		return;
	}

	if (first.name == L"")
	{
		if (first.fullName != L"")
			firstFileName = first.fullName + "\\" + second.name;
		if (second.type != L"" && second.type != L" ")
			firstFileName += L"." + second.type;
	}
	CopyFile(second.fullName, firstFileName, FailIfExists);
	if (!WithoutDate)
	{
		// чтобы было одинаковое время последнего доступа
		CopyFile(firstFileName, second.fullName, FailIfExists);
	}
}