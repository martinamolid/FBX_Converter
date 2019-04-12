#include "PrintInfo.h"


string Print2DVector(const char* pHeader, FbxVector2 pValue)
{
	string lString;
	FbxString lFloatValue1 = (float)pValue[0];
	FbxString lFloatValue2 = (float)pValue[1];

	lFloatValue1 = pValue[0] <= -HUGE_VAL ? "-INFINITY" : lFloatValue1.Buffer();
	lFloatValue1 = pValue[0] >= HUGE_VAL ? "INFINITY" : lFloatValue1.Buffer();
	lFloatValue2 = pValue[1] <= -HUGE_VAL ? "-INFINITY" : lFloatValue2.Buffer();
	lFloatValue2 = pValue[1] >= HUGE_VAL ? "INFINITY" : lFloatValue2.Buffer();

	lString = pHeader;
	lString += lFloatValue1;
	lString += " ";
	lString += lFloatValue2;
	lString += "\n";
	return lString;
}

string Print3DVector(const char* pHeader, FbxVector4 pValue)
{
	string lString;
	FbxString lFloatValue1 = (float)pValue[0];
	FbxString lFloatValue2 = (float)pValue[1];
	FbxString lFloatValue3 = (float)pValue[2];

	lFloatValue1 = pValue[0] <= -HUGE_VAL ? "-INFINITY" : lFloatValue1.Buffer();
	lFloatValue1 = pValue[0] >= HUGE_VAL ? "INFINITY" : lFloatValue1.Buffer();
	lFloatValue2 = pValue[1] <= -HUGE_VAL ? "-INFINITY" : lFloatValue2.Buffer();
	lFloatValue2 = pValue[1] >= HUGE_VAL ? "INFINITY" : lFloatValue2.Buffer();
	lFloatValue3 = pValue[2] <= -HUGE_VAL ? "-INFINITY" : lFloatValue3.Buffer();
	lFloatValue3 = pValue[2] >= HUGE_VAL ? "INFINITY" : lFloatValue3.Buffer();

	lString = pHeader;
	lString += lFloatValue1;
	lString += " ";
	lString += lFloatValue2;
	lString += " ";
	lString += lFloatValue3;
	lString += "\n";
	return lString;
}

string Print4DVector(const char* pHeader, FbxVector4 pValue)
{
	string lString;
	FbxString lFloatValue1 = (float)pValue[0];
	FbxString lFloatValue2 = (float)pValue[1];
	FbxString lFloatValue3 = (float)pValue[2];
	FbxString lFloatValue4 = (float)pValue[3];

	lFloatValue1 = pValue[0] <= -HUGE_VAL ? "-INFINITY" : lFloatValue1.Buffer();
	lFloatValue1 = pValue[0] >= HUGE_VAL ? "INFINITY" : lFloatValue1.Buffer();
	lFloatValue2 = pValue[1] <= -HUGE_VAL ? "-INFINITY" : lFloatValue2.Buffer();
	lFloatValue2 = pValue[1] >= HUGE_VAL ? "INFINITY" : lFloatValue2.Buffer();
	lFloatValue3 = pValue[2] <= -HUGE_VAL ? "-INFINITY" : lFloatValue3.Buffer();
	lFloatValue3 = pValue[2] >= HUGE_VAL ? "INFINITY" : lFloatValue3.Buffer();
	lFloatValue4 = pValue[3] <= -HUGE_VAL ? "-INFINITY" : lFloatValue4.Buffer();
	lFloatValue4 = pValue[3] >= HUGE_VAL ? "INFINITY" : lFloatValue4.Buffer();

	lString = pHeader;
	lString += lFloatValue1;
	lString += ", ";
	lString += lFloatValue2;
	lString += ", ";
	lString += lFloatValue3;
	lString += ", ";
	lString += lFloatValue4;
	lString += "\n";
	return lString;
}

string PrintColor(const char* pHeader, FbxColor pValue)
{
	string lString;

	lString = pHeader;
	lString += (float)pValue.mRed;

	lString += " (red), ";
	lString += (float)pValue.mGreen;

	lString += " (green), ";
	lString += (float)pValue.mBlue;

	lString += " (blue)";
	lString += "\n";
	return lString;
}

string PrintColor(const char* pHeader, FbxPropertyT<FbxDouble3> pValue)
{
	// THIS SHOULD PROBABLY BE REMOVED, IT SEEMS USELESS
	string lString;

	lString = pHeader;
	//lString += (float) pValue.mRed;
	//lString += (double)pValue.GetArrayItem(0);
	lString += " (red), ";
	//lString += (float) pValue.mGreen;
	//lString += (double)pValue.GetArrayItem(1);
	lString += " (green), ";
	//lString += (float) pValue.mBlue;
	//lString += (double)pValue.GetArrayItem(2);
	lString += " (blue)";
	lString += "\n";
	return lString;
}
