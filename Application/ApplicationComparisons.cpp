#include "pch.h"
#include "framework.h"
#include "Application.h"
#include "ApplicationDlg.h"
#include "AboutDlg.h"
#include "afxdialogex.h"
#include "WFDTranslator.h"
#include "HexEditorDlg.h"

std::vector<ComparisonResult> CApplicationDlg::CompareAll(std::vector<WFDFile> FirstList,
	std::vector<WFDFile> SecondList, CString firstDir, CString secondDir)
{
	if (firstDir == L"") firstDir = FirstDirectoryAddress;
	if (secondDir == L"") secondDir = SecondDirectoryAddress;

	FirstList = ViewDirectory(firstDir);
	SecondList = ViewDirectory(secondDir);

	std::vector<ComparisonResult> result;

	for (int i = 0; (size_t)i < FirstList.size(); i++)
	{
		BOOL isAdd = FALSE;

		// �������� ���������� 
		if (FirstList[i].size == L"0")
		{
			if (!WithFolders) continue;

			BOOL found = FALSE;
			int element = 0;

			// ����� ����������� ���������� � ������ �����
			for (; (size_t)element < SecondList.size(); element++)
				if (FirstList[i].name == SecondList[element].name)
					if (FirstList[i].size == SecondList[element].size)
					{
						found = TRUE;
						break;
					}

			// ���� ������ ��� ���������� ����������
			if (!found)
			{
				// ���� ���� ����� ����� ������� ��� ������ �� �� ������ ��� ������������� �������� ��� �����
				result.push_back(ComparisonResult(FirstList[i], WFDFile(secondDir), LEFTtoRIGHT));
				continue;
			}

			// ��������� ���� ����������
			result.push_back(ComparisonResult(FirstList[i], SecondList[element], EQUAL));

			// ������������� �� ����������
			std::vector<WFDFile> filesFirst = ViewDirectory(FirstList[i].fullName);
			std::vector<WFDFile> filesSecond = ViewDirectory(SecondList[element].fullName);;

			// ���������� ���������� 
			std::vector<ComparisonResult> temp = CompareAll(filesFirst, filesSecond,
				FirstList[i].fullName, SecondList[element].fullName);
			// ������ ��������� � ����� ������
			for (int j = 0; (size_t)j < temp.size(); j++)
			{
				result.push_back(temp[j]);
			}

			isAdd = TRUE;
		}

		// ��������� ���������� ������
		if (!isAdd)
			for (int j = 0; (size_t)j < SecondList.size(); j++)
				if (FirstList[i].name == SecondList[j].name)
					if (FirstList[i].type == SecondList[j].type)
					{
						result.push_back(Compare(FirstList[i], SecondList[j]));
						isAdd = TRUE;
						break;
					}

		// ���� �� ������ ���������� �� ��� ������ ���� � ��� �� ������ ��� � ���
		if (!isAdd)
			result.push_back(ComparisonResult(FirstList[i], WFDFile(secondDir), LEFTtoRIGHT));
	}
	// �������� ������ ������� ���� � ������ ���������� �� ������� ��� � ������
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
		// ���� ����� ���� ��� �� �������������
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
	ComparisonResult answer(first, second, EQUAL);
	if (!WithoutDate)
		if (first.date != second.date)
		{
			answer.ratio = NOTEQUAL;
			answer.difference |= DATE;
		}

	if (first.size != second.size)
	{
		answer.ratio = NOTEQUAL;
		answer.difference |= SIZE;
	}
	if (first.attr != second.attr)
	{
		answer.ratio = NOTEQUAL;
		answer.difference |= ATTR;
	}

	if (WithContent)
	{
		if (first.size != second.size)
		{
			answer.ratio = NOTEQUAL;
			answer.difference |= CONTENT;
			return answer;
		}
		int len = _ttoi(first.size);		// ������� ���������

		try
		{
			CFile fileFirst;
			CFile fileSecond;

			unsigned char* pointerFirst = new unsigned char[len];
			unsigned char* pointerSecond = new unsigned char[len];

			if (fileFirst.Open(first.fullName, CFile::modeRead | CFile::typeBinary) == 0)
			{
				answer.ratio = NOTEQUAL;
				answer.difference |= CONTENT;
				return answer;
			}

			if (fileSecond.Open(second.fullName, CFile::modeRead | CFile::typeBinary) == 0)
			{
				answer.ratio = NOTEQUAL;
				answer.difference |= CONTENT;
				return answer;
			}

			if (fileFirst.Read((void*)pointerFirst, sizeof(char) * len) < (size_t)len)
			{
				answer.ratio = NOTEQUAL;
				answer.difference |= CONTENT;
				return answer;
			}
			if (fileSecond.Read((void*)pointerSecond, sizeof(char) * len) < (size_t)len)
			{
				answer.ratio = NOTEQUAL;
				answer.difference |= CONTENT;
				return answer;
			}

			for (int i = 0; i < len; i++)
				if (pointerFirst[i] != pointerSecond[i])
				{
					fileFirst.Close();
					fileSecond.Close();

					answer.ratio = NOTEQUAL;
					answer.difference |= CONTENT;
					return answer;
				}

			fileFirst.Close();
			fileSecond.Close();
		}
		catch (...)
		{
			answer.ratio = NOTEQUAL;
			answer.difference |= CONTENT;
			return answer;
		}
	}

	return answer;
}

void CApplicationDlg::UpdateComparisonList(int begin)
{
	ListComparisonResults.DeleteAllItems();

	// ������ (������ ����� �������)
	ListComparisonResults.InsertItem(0,  L" ", -1);
	
	for (int comparisonNumber = begin, listNumber = 0; 
		(size_t)comparisonNumber < Comparasions.size(); 
		comparisonNumber++, listNumber++)
	{
		CString ratio;
		int item = 0;

		if (Comparasions[comparisonNumber].FirstFile.size == L"0" &&
			Comparasions[comparisonNumber].SecondFile.size == L"0")
		{
			// �� �� ������� + 1
			ListComparisonResults.InsertItem(listNumber + 1, L" ", -1);
			continue;
		}

		switch (Comparasions[comparisonNumber].ratio)
		{
		default:
			ratio = L"!=";
			if (!NotEqual) continue;
			break;
		case EQUAL:
			ratio = L"=";
			if (!Equal) continue;
			break;
		case LEFTtoRIGHT:
			ratio = L"=>";
			if (!LeftToRight) continue;
			break;
		case RIGHTtoLEFT:
			ratio = L"<=";
			if (!RightToLeft) continue;
			break;
		}

		// �� �� ������� + 1
		ListComparisonResults.InsertItem(listNumber + 1, ratio, -1);
	}//*/
}

void CApplicationDlg::syncLeftToRight(WFDFile first, WFDFile second)
{
	BOOL FailIfExists = FALSE;
	CString secondFileName;

	// ���� ����� ����������� ����������
	if (first.size == L"0")
	{
		// �������� ��� �����
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

	// ���� ����� ����������� ����
	// ���� ������ ����� ���, �������� ��� ��� 
	if (second.name == L"")
	{
		if (second.fullName != L"")
			secondFileName = second.fullName + "\\" + first.name;
		if (first.type != L"" && first.type != L" ")
			secondFileName += L"." + first.type;
	}
	else
	{
		secondFileName = second.fullName;
	}

	CopyFile(first.fullName, secondFileName, FailIfExists);
}

void CApplicationDlg::syncRightToLeft(WFDFile first, WFDFile second)
{
	BOOL FailIfExists = FALSE;
	CString firstFileName;

	// ���� ����� ����������� ����������
	if (second.size == L"0")
	{
		// �������� ��� �����
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

	// ���� ����� ����������� ����
	// ���� ������ ����� ���, �������� ��� ��� 
	if (first.name == L"")
	{
		if (first.fullName != L"")
			firstFileName = first.fullName + "\\" + second.name;
		if (second.type != L"" && second.type != L" ")
			firstFileName += L"." + second.type;
	}
	else
	{
		firstFileName = first.fullName;
	}
	
	CopyFile(second.fullName, firstFileName, FailIfExists);
}