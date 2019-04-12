#pragma once

#include <fbxsdk.h>
//TESTING MM
#include <string>
using namespace std;

//void DisplayMetaDataConnections(FbxObject* pNode);
//void DisplayString(const char* pHeader, const char* pValue = "", const char* pSuffix = "");
//void DisplayBool(const char* pHeader, bool pValue, const char* pSuffix = "");
//void DisplayInt(const char* pHeader, int pValue, const char* pSuffix = "");
//void DisplayDouble(const char* pHeader, double pValue, const char* pSuffix = "");
string Print2DVector(const char* pHeader, FbxVector2 pValue);
string Print3DVector(const char* pHeader, FbxVector4 pValue);
string Print4DVector(const char* pHeader, FbxVector4 pValue);
string PrintColor(const char* pHeader, FbxColor pValue);

