#pragma once

#include "pch.h"

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

	CString getPath() {
		CString temp = name;
		if (type != L"0") temp += L"." + type;
		int number = fullName.Find(temp);
		return fullName.Left(number);
	}
};