#include <fbxsdk.h>
#include "PrintNrOfMeshes.h"

#include "Filenames.h"

#include <fstream>
#include <iostream>
using namespace std;

//string PrintHierarchy(FbxNode* pNode, int pDepth);

//string PrintHierarchy(FbxScene* pScene)
//{
//	int i;
//	FbxNode* lRootNode = pScene->GetRootNode();
//	string pString;
//
//	ofstream binFile("xBinVtx.bin", ofstream::binary);
//
//	for (i = 0; i < lRootNode->GetChildCount(); i++)
//	{
//		pString += PrintHierarchy(lRootNode->GetChild(i), 0);
//	}
//	int temp = lRootNode->GetChildCount();
//	pString += to_string(temp);
//	binFile.write((char*)&temp, sizeof(int));
//	binFile.close();
//	return pString;
//}
//
//string PrintHierarchy(FbxNode* pNode, int pDepth)
//{
//	string lString;
//	int i;
//
//	for (i = 0; i < pDepth; i++)
//	{
//		lString += "     ";
//	}
//
//	lString += pNode->GetName();
//	lString += "\n";
//
//	//FBXSDK_printf(lString.Buffer());
//
//	for (i = 0; i < pNode->GetChildCount(); i++)
//	{
//		lString += PrintHierarchy(pNode->GetChild(i), pDepth + 1);
//		return lString;
//	}
//
//	return lString;
//}

string PrintNrOfMeshes(FbxScene* pScene)
{
	int i;
	FbxNode* lRootNode = pScene->GetRootNode();
	string pString;
	int nrOfMeshes = 0;

	ofstream binFile(BINARY_FILE, ofstream::binary);
	//int meshes = lRootNode->GetChildCount();
	for (i = 0; i < lRootNode->GetChildCount(); i++)
	{
		nrOfMeshes += PrintNrOfMeshes(lRootNode->GetChild(i), 0);
	}
	int temp = lRootNode->GetChildCount();
	pString += "Nr of Meshes: " +to_string(nrOfMeshes)+"\n";
	cout << "Nr of meshes: " << nrOfMeshes << endl;
	binFile.write((char*)&nrOfMeshes, sizeof(int));
	binFile.close();
	return pString;
}

int PrintNrOfMeshes(FbxNode* pNode, int pDepth)
{
	int nrOfMeshes = 0;;
	int i;

	nrOfMeshes++;

	for (i = 0; i < pNode->GetChildCount(); i++)
	{
		nrOfMeshes += PrintNrOfMeshes(pNode->GetChild(i), pDepth + 1);
		return nrOfMeshes;
	}

	return nrOfMeshes;
}
