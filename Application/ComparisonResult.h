#pragma once

#define EQUAL 0
#define NOTEQUAL 1
#define LEFTtoRIGHT 2
#define RIGHTtoLEFT 3


struct ComparisonResult
{
	WFDFile FirstFile;
	WFDFile SecondFile;
	int ratio;
	ComparisonResult(WFDFile f, WFDFile s, int r) :
		FirstFile(f),
		SecondFile(s),
		ratio(r)
	{

	}
};