#pragma once

#include "pch.h"

// WIN32_FIND_DATA - структура ОС Windows для описания файлов
// WFDFile - хранит необходимые поля данной структуре в строковом формате (для удобства записи)
struct WFDFile {

	CString fullName;
	CString name;
	CString type;
	CString size;
	CString date;
	CString attr;


	WFDFile(CString f = L"", CString n = L"", CString t = L"", CString s = L"", CString d = L"", CString a = L"") :
		fullName(f),
		name(n),
		type(t),
		size(s),
		date(d),
		attr(a)
	{ }

	CString getPath() 
	{
		CString temp = name;
		if (type != L"0") temp += L"." + type;
		int number = fullName.Find(temp);
		return fullName.Left(number);
	}
	
	CString GetOneString() {
		return fullName + "&" + name + "&" + type + "&" +
			size + "&" + date + "&" + attr + "\0";
	}

	void GetFromWString(CString str)
	{
		int number = str.Find(L"&");
		fullName = str.Left(number);
		str = str.Right(number);

		number = str.Find(L"&");
		name = str.Left(number);
		str = str.Right(number);

		number = str.Find(L"&");
		type = str.Left(number);
		str = str.Right(number);

		number = str.Find(L"&");
		size = str.Left(number);
		str = str.Right(number);

		number = str.Find(L"&");
		date = str.Left(number);
		str = str.Right(number);

		number = str.Find(L"&");
		attr = str.Left(number);
		str = str.Right(number);
	}

};