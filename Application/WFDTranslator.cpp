#include "pch.h"
#include "WFDTranslator.h"


WFDTranslator::WFDTranslator(WIN32_FIND_DATA wfd, CString path)
{
	name = CString(wfd.cFileName);

	FullName = path + L"\\" + name;

	size.Format(L"%d", ((long)wfd.nFileSizeHigh * ((long)MAXDWORD + 1)) + wfd.nFileSizeLow);

	int pointLocation = 0;
	if (size != L"0")
	{
		int temp = 0;
		do
		{
			pointLocation = temp;
			temp = name.Find(L'.', pointLocation + 1);
		} while (temp != -1);
	}


	if (pointLocation == 0) {				// Если не нашли тип
		type = " ";
	}
	else {			
		type = name.Right(name.GetLength() - pointLocation - 1);	// -1 убираем точку из типа
		name = name.Left(pointLocation);
	}

	//FileTimeToSystemTime(&wfd.ftLastAccessTime, &UTC);
	FileTimeToSystemTime(&wfd.ftLastWriteTime, &UTC);
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
