
#include <fbxsdk.h>

#include "PrintMaterial.h"

string PrintMaterial(FbxGeometry* pGeometry)
{
	int lMaterialCount = 0;
	FbxNode* lNode = NULL;

	string pString;

	if (pGeometry) {
		lNode = pGeometry->GetNode();
		if (lNode)
			lMaterialCount = lNode->GetMaterialCount();
	}

	if (lMaterialCount > 0)
	{
		FbxPropertyT<FbxDouble3> lKFbxDouble3;
		FbxPropertyT<FbxDouble> lKFbxDouble1;
		FbxColor theColor;

		for (int lCount = 0; lCount < lMaterialCount; lCount++)
		{
			pString += PrintInt("        Material ", lCount);

			FbxSurfaceMaterial *lMaterial = lNode->GetMaterial(lCount);

			pString += PrintString("            Name: \"", (char *)lMaterial->GetName(), "\"");

			//Get the implementation to see if it's a hardware shader.
			const FbxImplementation* lImplementation = GetImplementation(lMaterial, FBXSDK_IMPLEMENTATION_HLSL);
			FbxString lImplemenationType = "HLSL";
			if (!lImplementation)
			{
				lImplementation = GetImplementation(lMaterial, FBXSDK_IMPLEMENTATION_CGFX);
				lImplemenationType = "CGFX";
			}
			if (lImplementation)
			{
				//Now we have a hardware shader, let's read it
				FBXSDK_printf("            Hardware Shader Type: %s\n", lImplemenationType.Buffer());
				const FbxBindingTable* lRootTable = lImplementation->GetRootTable();
				FbxString lFileName = lRootTable->DescAbsoluteURL.Get();
				FbxString lTechniqueName = lRootTable->DescTAG.Get();


				const FbxBindingTable* lTable = lImplementation->GetRootTable();
				size_t lEntryNum = lTable->GetEntryCount();

				for (int i = 0; i < (int)lEntryNum; ++i)
				{
					const FbxBindingTableEntry& lEntry = lTable->GetEntry(i);
					const char* lEntrySrcType = lEntry.GetEntryType(true);
					FbxProperty lFbxProp;


					FbxString lTest = lEntry.GetSource();
					FBXSDK_printf("            Entry: %s\n", lTest.Buffer());


					if (strcmp(FbxPropertyEntryView::sEntryType, lEntrySrcType) == 0)
					{
						lFbxProp = lMaterial->FindPropertyHierarchical(lEntry.GetSource());
						if (!lFbxProp.IsValid())
						{
							lFbxProp = lMaterial->RootProperty.FindHierarchical(lEntry.GetSource());
						}


					}
					else if (strcmp(FbxConstantEntryView::sEntryType, lEntrySrcType) == 0)
					{
						lFbxProp = lImplementation->GetConstants().FindHierarchical(lEntry.GetSource());
					}
					if (lFbxProp.IsValid())
					{
						if (lFbxProp.GetSrcObjectCount<FbxTexture>() > 0)
						{
							//do what you want with the textures
							for (int j = 0; j < lFbxProp.GetSrcObjectCount<FbxFileTexture>(); ++j)
							{
								FbxFileTexture *lTex = lFbxProp.GetSrcObject<FbxFileTexture>(j);
								FBXSDK_printf("           File Texture: %s\n", lTex->GetFileName());
							}
							for (int j = 0; j < lFbxProp.GetSrcObjectCount<FbxLayeredTexture>(); ++j)
							{
								FbxLayeredTexture *lTex = lFbxProp.GetSrcObject<FbxLayeredTexture>(j);
								FBXSDK_printf("        Layered Texture: %s\n", lTex->GetName());
							}
							for (int j = 0; j < lFbxProp.GetSrcObjectCount<FbxProceduralTexture>(); ++j)
							{
								FbxProceduralTexture *lTex = lFbxProp.GetSrcObject<FbxProceduralTexture>(j);
								FBXSDK_printf("     Procedural Texture: %s\n", lTex->GetName());
							}
						}
						else
						{
							FbxDataType lFbxType = lFbxProp.GetPropertyDataType();
							FbxString blah = lFbxType.GetName();
							if (FbxBoolDT == lFbxType)
							{
								pString += PrintBool("                Bool: ", lFbxProp.Get<FbxBool>());
							}
							else if (FbxIntDT == lFbxType || FbxEnumDT == lFbxType)
							{
								pString += PrintInt("                Int: ", lFbxProp.Get<FbxInt>());
							}
							else if (FbxFloatDT == lFbxType)
							{
								pString += PrintDouble("                Float: ", lFbxProp.Get<FbxFloat>());

							}
							else if (FbxDoubleDT == lFbxType)
							{
								pString += PrintDouble("                Double: ", lFbxProp.Get<FbxDouble>());
							}
							else if (FbxStringDT == lFbxType
								|| FbxUrlDT == lFbxType
								|| FbxXRefUrlDT == lFbxType)
							{
								pString += PrintString("                String: ", lFbxProp.Get<FbxString>().Buffer());
							}
							else if (FbxDouble2DT == lFbxType)
							{
								FbxDouble2 lDouble2 = lFbxProp.Get<FbxDouble2>();
								FbxVector2 lVect;
								lVect[0] = lDouble2[0];
								lVect[1] = lDouble2[1];

								pString += Print2DVector("                2D vector: ", lVect);
							}
							else if (FbxDouble3DT == lFbxType || FbxColor3DT == lFbxType)
							{
								FbxDouble3 lDouble3 = lFbxProp.Get<FbxDouble3>();


								FbxVector4 lVect;
								lVect[0] = lDouble3[0];
								lVect[1] = lDouble3[1];
								lVect[2] = lDouble3[2];
								pString += Print3DVector("                3D vector: ", lVect);
							}

							else if (FbxDouble4DT == lFbxType || FbxColor4DT == lFbxType)
							{
								FbxDouble4 lDouble4 = lFbxProp.Get<FbxDouble4>();
								FbxVector4 lVect;
								lVect[0] = lDouble4[0];
								lVect[1] = lDouble4[1];
								lVect[2] = lDouble4[2];
								lVect[3] = lDouble4[3];
								pString += Print4DVector("                4D vector: ", lVect);
							}
							else if (FbxDouble4x4DT == lFbxType)
							{
								FbxDouble4x4 lDouble44 = lFbxProp.Get<FbxDouble4x4>();
								for (int j = 0; j < 4; ++j)
								{

									FbxVector4 lVect;
									lVect[0] = lDouble44[j][0];
									lVect[1] = lDouble44[j][1];
									lVect[2] = lDouble44[j][2];
									lVect[3] = lDouble44[j][3];
									pString += Print4DVector("                4x4D vector: ", lVect);
								}

							}
						}

					}
				}
			}
			else if (lMaterial->GetClassId().Is(FbxSurfacePhong::ClassId))
			{
				// We found a Phong material.  Display its properties.

				// Display the Ambient Color
				lKFbxDouble3 = ((FbxSurfacePhong *)lMaterial)->Ambient;
				theColor.Set(lKFbxDouble3.Get()[0], lKFbxDouble3.Get()[1], lKFbxDouble3.Get()[2]);
				pString += PrintColor("            Ambient: ", theColor);

				// Display the Diffuse Color
				lKFbxDouble3 = ((FbxSurfacePhong *)lMaterial)->Diffuse;
				theColor.Set(lKFbxDouble3.Get()[0], lKFbxDouble3.Get()[1], lKFbxDouble3.Get()[2]);
				pString += PrintColor("            Diffuse: ", theColor);

				// Display the Specular Color (unique to Phong materials)
				lKFbxDouble3 = ((FbxSurfacePhong *)lMaterial)->Specular;
				theColor.Set(lKFbxDouble3.Get()[0], lKFbxDouble3.Get()[1], lKFbxDouble3.Get()[2]);
				pString += PrintColor("            Specular: ", theColor);

				// Display the Emissive Color
				lKFbxDouble3 = ((FbxSurfacePhong *)lMaterial)->Emissive;
				theColor.Set(lKFbxDouble3.Get()[0], lKFbxDouble3.Get()[1], lKFbxDouble3.Get()[2]);
				pString += PrintColor("            Emissive: ", theColor);

				//Opacity is Transparency factor now
				lKFbxDouble1 = ((FbxSurfacePhong *)lMaterial)->TransparencyFactor;
				pString += PrintDouble("            Opacity: ", 1.0 - lKFbxDouble1.Get());

				// Display the Shininess
				lKFbxDouble1 = ((FbxSurfacePhong *)lMaterial)->Shininess;
				pString += PrintDouble("            Shininess: ", lKFbxDouble1.Get());

				// Display the Reflectivity
				lKFbxDouble1 = ((FbxSurfacePhong *)lMaterial)->ReflectionFactor;
				pString += PrintDouble("            Reflectivity: ", lKFbxDouble1.Get());
			}
			else if (lMaterial->GetClassId().Is(FbxSurfaceLambert::ClassId))
			{
				// We found a Lambert material. Display its properties.
				// Display the Ambient Color
				lKFbxDouble3 = ((FbxSurfaceLambert *)lMaterial)->Ambient;
				theColor.Set(lKFbxDouble3.Get()[0], lKFbxDouble3.Get()[1], lKFbxDouble3.Get()[2]);
				pString += PrintColor("            Ambient: ", theColor);

				// Display the Diffuse Color
				lKFbxDouble3 = ((FbxSurfaceLambert *)lMaterial)->Diffuse;
				theColor.Set(lKFbxDouble3.Get()[0], lKFbxDouble3.Get()[1], lKFbxDouble3.Get()[2]);
				pString += PrintColor("            Diffuse: ", theColor);

				// Display the Emissive
				lKFbxDouble3 = ((FbxSurfaceLambert *)lMaterial)->Emissive;
				theColor.Set(lKFbxDouble3.Get()[0], lKFbxDouble3.Get()[1], lKFbxDouble3.Get()[2]);
				pString += PrintColor("            Emissive: ", theColor);

				// Display the Opacity
				lKFbxDouble1 = ((FbxSurfaceLambert *)lMaterial)->TransparencyFactor;
				pString += PrintDouble("            Opacity: ", 1.0 - lKFbxDouble1.Get());
			}
			else
			pString += PrintString("Unknown type of Material");

			FbxPropertyT<FbxString> lString;
			lString = lMaterial->ShadingModel;
			pString += PrintString("            Shading Model: ", lString.Get().Buffer());
			pString += PrintString("");
		}
	}
	return pString;
}

