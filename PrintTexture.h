#pragma once

#include "PrintInfo.h"

string FindAndDisplayTextureInfoByProperty(FbxProperty pProperty, bool& pDisplayHeader, int pMaterialIndex);
string PrintTexture(FbxGeometry* pGeometry);
string PrintTextureInfo(FbxTexture* pTexture, int pBlendMode);