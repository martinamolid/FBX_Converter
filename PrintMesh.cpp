#include "PrintMesh.h"

#include "Filenames.h"

#include "PrintMaterial.h"
#include "PrintTexture.h"

#include <iostream>
#include <fstream>
using namespace std;

//#define MAT_HEADER_LENGTH 200

//void BinaryConvert(string name);

//string PrintControlsPoints(FbxMesh* pMesh);
string PrintPolygons(FbxMesh* pMesh);
//string PrintMaterialMapping(FbxMesh* pMesh);
//void PrintTextureMapping(FbxMesh* pMesh);
//void PrintTextureNames(FbxProperty &pProperty, FbxString& pConnectionString);
//void PrintMaterialConnections(FbxMesh* pMesh);
//void PrintMaterialTextureConnections(FbxSurfaceMaterial* pMaterial, char * header, int pMatId, int l);

struct Vertex {
	float pos[3];
	float uv[2];
	float norm[3];
	float tan[3];
	float binorm[3];
};

string PrintMesh(FbxNode* pNode) {
	FbxMesh* pMesh = (FbxMesh*)pNode->GetNodeAttribute();
	string pString;

	//string name = (char *)pNode->GetName();
	//pString += "Mesh: " + name + "\n";
	///pString += PrintControlsPoints(pMesh);
	pString += PrintPolygons(pMesh);
	pString += "\n";
	///pString += PrintMaterialMapping(pMesh);
	pString += PrintMaterial(pMesh);
	pString += "\n";
	pString += PrintTexture(pMesh);
	pString += "\n";

	return pString;
}

//void BinaryConvert(string name) {
//
//	ifstream binIn(name, ifstream::binary);
//	int nrOfVertices = 0;
//	binIn.read((char*)&nrOfVertices, sizeof(int));
//	Vertex* vtxBuff = new Vertex[nrOfVertices];
//	binIn.read((char*)vtxBuff, sizeof(Vertex)*nrOfVertices);
//	binIn.close();
//
//	ofstream binOut("xBinOut.txt", ofstream::out);
//	binOut << nrOfVertices << endl;
//	for (int i = 0; i < nrOfVertices; i++) {
//		binOut << vtxBuff[i].x << " " << vtxBuff[i].y << " " << vtxBuff[i].z << endl;
//		binOut << vtxBuff[i].u << " " << vtxBuff[i].v << endl;
//		binOut << vtxBuff[i].nx << " " << vtxBuff[i].ny << " " << vtxBuff[i].nz << endl;
//	}
//	binOut.close();
//
//	delete[] vtxBuff;
//}

//string PrintControlsPoints(FbxMesh* pMesh)
//{
//	string pString;
//	int i, lControlPointsCount = pMesh->GetControlPointsCount();
//	FbxVector4* lControlPoints = pMesh->GetControlPoints();
//
//	ofstream binFile ("xBin.bin", ofstream::binary);
//
//	struct vtxCoord {
//		float x, y, z;
//	};
//
//	int size = 0;
//
//	vtxCoord *farr = nullptr;
//
//	pString += "        Control Points Count: " + to_string(lControlPointsCount) + "\n";
//	// DEFINE SIZE HERE BASED ON CONTROL POINTS
//	size = sizeof(vtxCoord) * lControlPointsCount;
//	farr = new vtxCoord[size];
//
//	binFile.write((char*)&lControlPointsCount, sizeof(int));
//
//	for (i = 0; i < lControlPointsCount; i++)
//	{
//		//pString += "        Control Point " + to_string(i);
//		pString += Print3DVector("            Coordinates: ", lControlPoints[i]);
//		//pString += Print3DVector("", lControlPoints[i]);
//
//		farr[i].x = lControlPoints[i][0];
//		farr[i].y = lControlPoints[i][1];
//		farr[i].z = lControlPoints[i][2];
//
//		for (int j = 0; j < pMesh->GetElementNormalCount(); j++)
//		{
//			FbxGeometryElementNormal* leNormals = pMesh->GetElementNormal(j);
//			if (leNormals->GetMappingMode() == FbxGeometryElement::eByControlPoint)
//			{
//				char header[100];
//				FBXSDK_sprintf(header, 100, "            Normal Vector: ");
//				if (leNormals->GetReferenceMode() == FbxGeometryElement::eDirect)
//					pString += Print3DVector(header, leNormals->GetDirectArray().GetAt(i));
//			}
//		}
//	}
//
//	binFile.write((char*) farr, size);
//	//binFile.write((char*)&f, sizeof(float));
//	binFile.close();
//
//	//int newSize = 0;
//	//
//	//ifstream binIn("xBin.bin", ifstream::binary);
//	////binIn.seekg(0, binIn.end);
//	////newSize = binIn.tellg();
//	////binIn.seekg(0);
//	//int nrOfCPs = 0;
//	//binIn.read((char*)&nrOfCPs, sizeof(int));
//	//vtxCoord* fbuff = new vtxCoord[nrOfCPs];
//	//binIn.read((char*)fbuff, sizeof(vtxCoord)*nrOfCPs);
//	//binIn.close();
//
//	//ofstream binOut("xBinOut.txt");
//	//binOut << nrOfCPs << endl;
//	//for (int i = 0; i < nrOfCPs; i++) {
//	//	binOut << fbuff[i].x << " " << fbuff[i].y << " " << fbuff[i].z << endl;
//	//}
//	//binOut.close();
//
//	////BinaryConvert("xBin.bin");
//
//	//delete[] fbuff;
//	delete[] farr;
//
//	return pString;
//}

string PrintPolygons(FbxMesh* pMesh)
{
	string pString;

	int i, j, lPolygonCount = pMesh->GetPolygonCount();
	FbxVector4* lControlPoints = pMesh->GetControlPoints();
	char header[100];

	//FbxMesh* parent = pMesh->GetDstObject();
	//MM This entire parent name part is a train wreck, needs more testing with a FBX with actual hierarchies
	/*if (parent != NULL) {
		string parentName = (char*)pMesh->GetDstObject()->GetName();
		if (parentName != "") {
			pString += "Parent Name: " + parentName + "\n";
		}
	}*/

	ofstream binVtxFile(BINARY_FILE, ofstream::binary | ofstream::app);

	int nrOfMat = pMesh->GetElementMaterialCount();
	//cout << "Material Count in PrintPolygons: " << nrOfMat << endl;
	pString += "mat count " + to_string(nrOfMat) + "\n";
	binVtxFile.write((char*)&nrOfMat, sizeof(int));

	int vtxCount = pMesh->GetPolygonVertexCount();
	pString += "vtx count " + to_string(vtxCount) + "\n";
	binVtxFile.write((char*)&vtxCount, sizeof(int));

	Vertex *vertices = new Vertex[vtxCount];

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
				vertices[vertexId].pos[0] = lControlPoints[lControlPointIndex][0];
				vertices[vertexId].pos[1] = lControlPoints[lControlPointIndex][1];
				vertices[vertexId].pos[2] = lControlPoints[lControlPointIndex][2];

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
						vertices[vertexId].uv[0] = leUV->GetDirectArray().GetAt(lControlPointIndex)[0];
						vertices[vertexId].uv[1] = leUV->GetDirectArray().GetAt(lControlPointIndex)[1];
						break;
					case FbxGeometryElement::eIndexToDirect:
					{
						int id = leUV->GetIndexArray().GetAt(lControlPointIndex);
						pString += Print2DVector(header, leUV->GetDirectArray().GetAt(id));
						vertices[vertexId].uv[0] = leUV->GetDirectArray().GetAt(id)[0];
						vertices[vertexId].uv[1] = leUV->GetDirectArray().GetAt(id)[1];
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
						vertices[vertexId].uv[0] = leUV->GetDirectArray().GetAt(lTextureUVIndex)[0];
						vertices[vertexId].uv[1] = leUV->GetDirectArray().GetAt(lTextureUVIndex)[1];
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
						vertices[vertexId].norm[0] = leNormal->GetDirectArray().GetAt(vertexId)[0];
						vertices[vertexId].norm[1] = leNormal->GetDirectArray().GetAt(vertexId)[1];
						vertices[vertexId].norm[2] = leNormal->GetDirectArray().GetAt(vertexId)[2];
						break;
					case FbxGeometryElement::eIndexToDirect:
					{
						int id = leNormal->GetIndexArray().GetAt(vertexId);
						pString += Print3DVector(header, leNormal->GetDirectArray().GetAt(id));
						vertices[vertexId].norm[0] = leNormal->GetDirectArray().GetAt(id)[0];
						vertices[vertexId].norm[1] = leNormal->GetDirectArray().GetAt(id)[1];
						vertices[vertexId].norm[2] = leNormal->GetDirectArray().GetAt(id)[2];
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
				FBXSDK_sprintf(header, 100, "vtan ");

				if (leTangent->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
				{
					switch (leTangent->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
						pString += Print3DVector(header, leTangent->GetDirectArray().GetAt(vertexId));
						vertices[vertexId].tan[0] = leTangent->GetDirectArray().GetAt(vertexId)[0];
						vertices[vertexId].tan[1] = leTangent->GetDirectArray().GetAt(vertexId)[1];
						vertices[vertexId].tan[2] = leTangent->GetDirectArray().GetAt(vertexId)[2];
						break;
					case FbxGeometryElement::eIndexToDirect:
					{
						int id = leTangent->GetIndexArray().GetAt(vertexId);
						pString += Print3DVector(header, leTangent->GetDirectArray().GetAt(id));
						vertices[vertexId].tan[0] = leTangent->GetDirectArray().GetAt(id)[0];
						vertices[vertexId].tan[1] = leTangent->GetDirectArray().GetAt(id)[1];
						vertices[vertexId].tan[2] = leTangent->GetDirectArray().GetAt(id)[2];
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

				FBXSDK_sprintf(header, 100, "vbin ");
				if (leBinormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
				{
					switch (leBinormal->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
						pString += Print3DVector(header, leBinormal->GetDirectArray().GetAt(vertexId));
						vertices[vertexId].binorm[0] = leBinormal->GetDirectArray().GetAt(vertexId)[0];
						vertices[vertexId].binorm[1] = leBinormal->GetDirectArray().GetAt(vertexId)[1];
						vertices[vertexId].binorm[2] = leBinormal->GetDirectArray().GetAt(vertexId)[2];
						break;
					case FbxGeometryElement::eIndexToDirect:
					{
						int id = leBinormal->GetIndexArray().GetAt(vertexId);
						pString += Print3DVector(header, leBinormal->GetDirectArray().GetAt(id));
						vertices[vertexId].binorm[0] = leBinormal->GetDirectArray().GetAt(id)[0];
						vertices[vertexId].binorm[1] = leBinormal->GetDirectArray().GetAt(id)[1];
						vertices[vertexId].binorm[2] = leBinormal->GetDirectArray().GetAt(id)[2];
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

	binVtxFile.write((char*)vertices, sizeof(Vertex)*vtxCount);
	delete[] vertices;
	///BinaryConvert("xBinVtx.bin");
	binVtxFile.close();
	return pString;
}

//string PrintMaterialMapping(FbxMesh* pMesh)
//{
//	const char* lMappingTypes[] = { "None", "By Control Point", "By Polygon Vertex", "By Polygon", "By Edge", "All Same" };
//	const char* lReferenceMode[] = { "Direct", "Index", "Index to Direct" };
//
//	string pString; 
//
//	int lMtrlCount = 0;
//	FbxNode* lNode = NULL;
//	if (pMesh) {
//		lNode = pMesh->GetNode();
//		if (lNode)
//			lMtrlCount = lNode->GetMaterialCount();
//	}
//
//	for (int l = 0; l < pMesh->GetElementMaterialCount(); l++)
//	{
//		FbxGeometryElementMaterial* leMat = pMesh->GetElementMaterial(l);
//		if (leMat)
//		{
//			char header[100];
//			FBXSDK_sprintf(header, 100, "    Material Element %d: ", l);
//			pString += PrintString(header);
//			//pString += header;
//
//
//			pString += PrintString("           Mapping: ", lMappingTypes[leMat->GetMappingMode()]);
//			pString += PrintString("           ReferenceMode: ", lReferenceMode[leMat->GetReferenceMode()]);
//
//			int lMaterialCount = 0;
//			//FbxString lString;
//
//			if (leMat->GetReferenceMode() == FbxGeometryElement::eDirect ||
//				leMat->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
//			{
//				lMaterialCount = lMtrlCount;
//			}
//
//			if (leMat->GetReferenceMode() == FbxGeometryElement::eIndex ||
//				leMat->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
//			{
//				int i;
//
//				pString += "           Indices: ";
//
//				int lIndexArrayCount = leMat->GetIndexArray().GetCount();
//				for (i = 0; i < lIndexArrayCount; i++)
//				{
//					pString += to_string(leMat->GetIndexArray().GetAt(i));
//
//					if (i < lIndexArrayCount - 1)
//					{
//						pString += ", ";
//					}
//				}
//
//				pString += "\n";
//
//				//FBXSDK_printf(lString);
//			}
//		}
//	}
//
//	//DisplayString("");
//	return pString;
//}


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