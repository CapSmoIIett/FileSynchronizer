#pragma once

#include "pch.h"

struct WFDFile {
	CString fullName;
	CString name;
	CString type;
	CString size;
	CString date;
	CString attr;

	WFDFile() :
		fullName(L""),
		name(L""),
		type(L""),
		size(L""),
		date(L""),
		attr(L"")
	{ }
	WFDFile(CString f, CString n, CString t, CString s, CString d, CString a) :
		fullName(f),
		name(n),
		type(t),
		size(s),
		date(d),
		attr(a)
	{ }
};