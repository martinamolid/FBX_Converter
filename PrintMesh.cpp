#include "PrintMesh.h"

#include <iostream>
#include <fstream>
using namespace std;

//#define MAT_HEADER_LENGTH 200

string PrintControlsPoints(FbxMesh* pMesh);
string PrintPolygons(FbxMesh* pMesh);
//void PrintMaterialMapping(FbxMesh* pMesh);
//void PrintTextureMapping(FbxMesh* pMesh);
//void PrintTextureNames(FbxProperty &pProperty, FbxString& pConnectionString);
//void PrintMaterialConnections(FbxMesh* pMesh);
//void PrintMaterialTextureConnections(FbxSurfaceMaterial* pMaterial, char * header, int pMatId, int l);

string PrintMesh(FbxNode* pNode) {
	FbxMesh* pMesh = (FbxMesh*)pNode->GetNodeAttribute();
	string pString;

	string name = (char *)pNode->GetName();
	pString += "Mesh: " + name + "\n";
	//pString += PrintControlsPoints(pMesh);
	pString += PrintPolygons(pMesh);
	pString += "\n";

	return pString;
}

string PrintControlsPoints(FbxMesh* pMesh)
{
	string pString;
	int i, lControlPointsCount = pMesh->GetControlPointsCount();
	FbxVector4* lControlPoints = pMesh->GetControlPoints();

	//file << "    Control Points" << endl;

	pString += "        Control Points Count: " + to_string(lControlPointsCount) + "\n";

	for (i = 0; i < lControlPointsCount; i++)
	{
		//stringstream sin;
		//pString += "        Control Point " + to_string(i);
		pString += Print3DVector("            Coordinates: ", lControlPoints[i]);

		for (int j = 0; j < pMesh->GetElementNormalCount(); j++)
		{
			FbxGeometryElementNormal* leNormals = pMesh->GetElementNormal(j);
			if (leNormals->GetMappingMode() == FbxGeometryElement::eByControlPoint)
			{
				char header[100];
				FBXSDK_sprintf(header, 100, "            Normal Vector: ");
				if (leNormals->GetReferenceMode() == FbxGeometryElement::eDirect)
					// Is this ever printed anyway???
				pString += Print3DVector(header, leNormals->GetDirectArray().GetAt(i));
			}
		}
	}

	return pString;
}

string PrintPolygons(FbxMesh* pMesh)
{
	string pString;

	int i, j, lPolygonCount = pMesh->GetPolygonCount();
	FbxVector4* lControlPoints = pMesh->GetControlPoints();
	char header[100];

	int polySize = 3; // MM: There's no way to get the poly size for every poly, as each and every has their own size. Assuming it is triangulated, we'll hardcode this for now

	// MM This entire parent name part is a train wreck, needs more testing with a FBX with actual hierarchies
	/*if (pMesh->GetDstObject() != NULL) {
		string parentName = (char*)pMesh->GetDstObject()->GetName();
		if (parentName != "") {
			pString += "Parent Name: " + parentName + "\n";
		}
	}*/


	pString += "Poly Count: " + to_string(lPolygonCount) + "\n";
	pString += "Vtx Count: " + to_string(lPolygonCount*polySize) + "\n";

	int vertexId = 0;
	for (i = 0; i < lPolygonCount; i++)
	{
		//pString += "        Polygon " + to_string(i) + "\n";
		int l;

		for (l = 0; l < pMesh->GetElementPolygonGroupCount(); l++)
		{
			FbxGeometryElementPolygonGroup* lePolgrp = pMesh->GetElementPolygonGroup(l);
			switch (lePolgrp->GetMappingMode())
			{
			case FbxGeometryElement::eByPolygon:
				if (lePolgrp->GetReferenceMode() == FbxGeometryElement::eIndex)
				{
					FBXSDK_sprintf(header, 100, "        Assigned to group: ");
					int polyGroupId = lePolgrp->GetIndexArray().GetAt(i);
					pString += header + to_string(polyGroupId) + "\n";
					break;
				}
			default:
				// any other mapping modes don't make sense
				pString += "        \"unsupported group assignment\"\n";
				break;
			}
		}

		int lPolygonSize = pMesh->GetPolygonSize(i);

		for (j = 0; j < lPolygonSize; j++)
		{
			int lControlPointIndex = pMesh->GetPolygonVertex(i, j);
			if (lControlPointIndex < 0)
			{
				//DisplayString("            Coordinates: Invalid index found!");
				continue;
			}
			else
			{
				pString += Print3DVector("v ", lControlPoints[lControlPointIndex]);
			}

			for (l = 0; l < pMesh->GetElementVertexColorCount(); l++)
			{
				FbxGeometryElementVertexColor* leVtxc = pMesh->GetElementVertexColor(l);
				FBXSDK_sprintf(header, 100, "            Color vertex: ");

				switch (leVtxc->GetMappingMode())
				{
				default:
					break;
				case FbxGeometryElement::eByControlPoint:
					switch (leVtxc->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
						pString += PrintColor(header, leVtxc->GetDirectArray().GetAt(lControlPointIndex));
						break;
					case FbxGeometryElement::eIndexToDirect:
					{
						int id = leVtxc->GetIndexArray().GetAt(lControlPointIndex);
						pString += PrintColor(header, leVtxc->GetDirectArray().GetAt(id));
					}
					break;
					default:
						break; // other reference modes not shown here!
					}
					break;

				case FbxGeometryElement::eByPolygonVertex:
				{
					switch (leVtxc->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
						pString += PrintColor(header, leVtxc->GetDirectArray().GetAt(vertexId));
						break;
					case FbxGeometryElement::eIndexToDirect:
					{
						int id = leVtxc->GetIndexArray().GetAt(vertexId);
						pString += PrintColor(header, leVtxc->GetDirectArray().GetAt(id));
					}
					break;
					default:
						break; // other reference modes not shown here!
					}
				}
				break;

				case FbxGeometryElement::eByPolygon: // doesn't make much sense for UVs
				case FbxGeometryElement::eAllSame:   // doesn't make much sense for UVs
				case FbxGeometryElement::eNone:       // doesn't make much sense for UVs
					break;
				}
			}
			for (l = 0; l < pMesh->GetElementUVCount(); ++l)
			{
				FbxGeometryElementUV* leUV = pMesh->GetElementUV(l);
				FBXSDK_sprintf(header, 100, "vt ");

				switch (leUV->GetMappingMode())
				{
				default:
					break;
				case FbxGeometryElement::eByControlPoint:
					switch (leUV->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
						pString += Print2DVector(header, leUV->GetDirectArray().GetAt(lControlPointIndex));
						break;
					case FbxGeometryElement::eIndexToDirect:
					{
						int id = leUV->GetIndexArray().GetAt(lControlPointIndex);
						pString += Print2DVector(header, leUV->GetDirectArray().GetAt(id));
					}
					break;
					default:
						break; // other reference modes not shown here!
					}
					break;

				case FbxGeometryElement::eByPolygonVertex:
				{
					int lTextureUVIndex = pMesh->GetTextureUVIndex(i, j);
					switch (leUV->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
					case FbxGeometryElement::eIndexToDirect:
					{
						pString += Print2DVector(header, leUV->GetDirectArray().GetAt(lTextureUVIndex));
					}
					break;
					default:
						break; // other reference modes not shown here!
					}
				}
				break;

				case FbxGeometryElement::eByPolygon: // doesn't make much sense for UVs
				case FbxGeometryElement::eAllSame:   // doesn't make much sense for UVs
				case FbxGeometryElement::eNone:       // doesn't make much sense for UVs
					break;
				}
			}
			for (l = 0; l < pMesh->GetElementNormalCount(); ++l)
			{
				FbxGeometryElementNormal* leNormal = pMesh->GetElementNormal(l);
				FBXSDK_sprintf(header, 100, "vn ");

				if (leNormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
				{
					switch (leNormal->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
						pString += Print3DVector(header, leNormal->GetDirectArray().GetAt(vertexId));
						break;
					case FbxGeometryElement::eIndexToDirect:
					{
						int id = leNormal->GetIndexArray().GetAt(vertexId);
						pString += Print3DVector(header, leNormal->GetDirectArray().GetAt(id));
					}
					break;
					default:
						break; // other reference modes not shown here!
					}
				}

			}
			for (l = 0; l < pMesh->GetElementTangentCount(); ++l)
			{
				FbxGeometryElementTangent* leTangent = pMesh->GetElementTangent(l);
				FBXSDK_sprintf(header, 100, "            Tangent: ");

				if (leTangent->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
				{
					switch (leTangent->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
						pString += Print3DVector(header, leTangent->GetDirectArray().GetAt(vertexId));
						break;
					case FbxGeometryElement::eIndexToDirect:
					{
						int id = leTangent->GetIndexArray().GetAt(vertexId);
						pString += Print3DVector(header, leTangent->GetDirectArray().GetAt(id));
					}
					break;
					default:
						break; // other reference modes not shown here!
					}
				}

			}
			for (l = 0; l < pMesh->GetElementBinormalCount(); ++l)
			{

				FbxGeometryElementBinormal* leBinormal = pMesh->GetElementBinormal(l);

				FBXSDK_sprintf(header, 100, "            Binormal: ");
				if (leBinormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
				{
					switch (leBinormal->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
						pString += Print3DVector(header, leBinormal->GetDirectArray().GetAt(vertexId));
						break;
					case FbxGeometryElement::eIndexToDirect:
					{
						int id = leBinormal->GetIndexArray().GetAt(vertexId);
						pString += Print3DVector(header, leBinormal->GetDirectArray().GetAt(id));
					}
					break;
					default:
						break; // other reference modes not shown here!
					}
				}
			}
			vertexId++;
		} // for polygonSize
	} // for polygonCount


	//DisplayString("");
	return pString;
}

//string PrintTextureNames(FbxProperty &pProperty, FbxString& pConnectionString)
//{
//	int lLayeredTextureCount = pProperty.GetSrcObjectCount<FbxLayeredTexture>();
//	if (lLayeredTextureCount > 0)
//	{
//		for (int j = 0; j < lLayeredTextureCount; ++j)
//		{
//			FbxLayeredTexture *lLayeredTexture = pProperty.GetSrcObject<FbxLayeredTexture>(j);
//			int lNbTextures = lLayeredTexture->GetSrcObjectCount<FbxTexture>();
//			pConnectionString += " Texture ";
//
//			for (int k = 0; k < lNbTextures; ++k)
//			{
//				//lConnectionString += k;
//				pConnectionString += "\"";
//				pConnectionString += (char*)lLayeredTexture->GetName();
//				pConnectionString += "\"";
//				pConnectionString += " ";
//			}
//			pConnectionString += "of ";
//			pConnectionString += pProperty.GetName();
//			pConnectionString += " on layer ";
//			pConnectionString += j;
//		}
//		pConnectionString += " |";
//	}
//	else
//	{
//		//no layered texture simply get on the property
//		int lNbTextures = pProperty.GetSrcObjectCount<FbxTexture>();
//
//		if (lNbTextures > 0)
//		{
//			pConnectionString += " Texture ";
//			pConnectionString += " ";
//
//			for (int j = 0; j < lNbTextures; ++j)
//			{
//				FbxTexture* lTexture = pProperty.GetSrcObject<FbxTexture>(j);
//				if (lTexture)
//				{
//					pConnectionString += "\"";
//					pConnectionString += (char*)lTexture->GetName();
//					pConnectionString += "\"";
//					pConnectionString += " ";
//				}
//			}
//			pConnectionString += "of ";
//			pConnectionString += pProperty.GetName();
//			pConnectionString += " |";
//		}
//	}
//}