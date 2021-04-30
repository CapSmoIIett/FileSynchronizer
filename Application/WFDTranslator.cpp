#include "pch.h"
#include "WFDTranslator.h"


WFDTranslator::WFDTranslator(WIN32_FIND_DATA wfd)
{
	name = CString(wfd.cFileName);

	int n = name.Find(L'.');

	if (n == -1) {				// Если не нашли тип
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

	size.Format(L"%d", ((long)wfd.nFileSizeHigh * (MAXDWORD + 1)) + wfd.nFileSizeLow);

	attr += (wfd.dwFileAttributes & FILE_ATTRIBUTE_READONLY) ? L'r' : L'-';			// Сверяем атрибуты
	attr += (wfd.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE)  ? L'a' : L'-';
	attr += (wfd.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)   ? L'h' : L'-';
	attr += (wfd.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM)   ? L's' : L'-';

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
