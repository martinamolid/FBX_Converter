#include <fbxsdk.h>
#include "PrintHierarchy.h"

//string PrintHierarchy(FbxNode* pNode, int pDepth);

string PrintHierarchy(FbxScene* pScene)
{
	int i;
	FbxNode* lRootNode = pScene->GetRootNode();
	string pString;

	for (i = 0; i < lRootNode->GetChildCount(); i++)
	{
		pString += PrintHierarchy(lRootNode->GetChild(i), 0);
	}
	return pString;
}

string PrintHierarchy(FbxNode* pNode, int pDepth)
{
	string lString;
	int i;

	for (i = 0; i < pDepth; i++)
	{
		lString += "     ";
	}

	lString += pNode->GetName();
	lString += "\n";

	//FBXSDK_printf(lString.Buffer());

	for (i = 0; i < pNode->GetChildCount(); i++)
	{
		lString += PrintHierarchy(pNode->GetChild(i), pDepth + 1);
		return lString;
	}

	return lString;
}


