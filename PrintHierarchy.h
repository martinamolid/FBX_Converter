#pragma once

#include "DisplayCommon.h"

string PrintHierarchy(FbxScene* pScene);
string PrintHierarchy(FbxNode* pNode, int pDepth);