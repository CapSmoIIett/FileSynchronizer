#pragma once

#define EQUAL 0
#define NOTEQUAL 1
#define LEFTtoRIGHT 2
#define RIGHTtoLEFT 3

#define CONTENT		1
#define TYPE		2
#define SIZE		4
#define DATE		8
#define ATTR		16
#define CONTENT		32

// Структура представляющая результат сравнение двух файлов
struct ComparisonResult
{
	WFDFile FirstFile;
	WFDFile SecondFile;
	int ratio;
	int difference; 

	ComparisonResult(WFDFile f, WFDFile s, int r , int dif = 0) :
		FirstFile(f),
		SecondFile(s),
		ratio(r),
		difference(dif)
	{

	}
	
	bool isDifContent()
	{
		return (difference & CONTENT) == CONTENT ;
	}
	bool isDifType()
	{
		return (difference & TYPE) == TYPE;
	}
	bool isDifSize()
	{
		return (difference & SIZE) == SIZE;
	}
	bool isDifDate()
	{
		return (difference & DATE) == DATE;
	}
	bool isDifAttr()
	{
		return (difference & ATTR) == ATTR;
	}
};