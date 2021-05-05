#include "pch.h"
#include "WFDTranslator.h"


WFDTranslator::WFDTranslator(WIN32_FIND_DATA wfd, CString path)
{
	name = CString(wfd.cFileName);

	FullName = path + L"\\" + name;

	size.Format(L"%d", ((long)wfd.nFileSizeHigh * (MAXDWORD + 1)) + wfd.nFileSizeLow);

	int n = 0;
	if (size != L"0")
	{
		int temp = 0;
		do
		{
			n = temp;
			temp = name.Find(L'.', n + 1);
		} while (temp != -1);
	}


	if (n == 0) {				// Если не нашли тип
		type = " ";
	}
	else {			
		type = name.Right(name.GetLength() - n - 1);	// -1 убираем точку из типа
		name = name.Left(n);
	}

	FileTimeToSystemTime(&wfd.ftLastAccessTime, &UTC);
	SystemTimeToTzSpecificLocalTime(NULL, &UTC, &time);
	date.Format(L"%02d/%02d/%d %02d:%02d", time.wMonth, time.wDay, time.wYear,
		time.wHour, time.wMinute);

	

	attr += (wfd.dwFileAttributes & FILE_ATTRIBUTE_READONLY) ? L'r' : L'-';			// Сверяем атрибуты
	attr += (wfd.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE)  ? L'a' : L'-';
	attr += (wfd.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)   ? L'h' : L'-';
	attr += (wfd.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM)   ? L's' : L'-';

}

CString WFDTranslator::getNameWithType() {
	return name + L"." + type;
}
CString WFDTranslator::getFullName() {
	return FullName;
}
CString WFDTranslator::getName() {
	return name;
}
CString WFDTranslator::getType() {
	return type;
}
CString WFDTranslator::getSize() {
	return size;
}
CString WFDTranslator::getDate() {
	return date;
}
CString WFDTranslator::getAttr() {
	return attr;
}
