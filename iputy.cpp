// iputy.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "iputy.h"
#include "stdmat.h"
#include "decomp.h"
#include "typedef.h"
#include <stdio.h>
#include "resource.h"
#include <windows.h>
#include <maxapi.h>
#include <notify.h>
#include <iostream> 

HINSTANCE hInstance = NULL;
HWND hWnd ;
HWND hDlg;
INT_PTR CALLBACK DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        case WM_INITDIALOG:
            return TRUE;
        case WM_COMMAND:
            if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) {
                EndDialog(hDlg, LOWORD(wParam));
                return TRUE;
            }
            break;
    }
    return FALSE;
}

int MyImporter::DoImport(const TCHAR* name, ImpInterface* ii, Interface* i, BOOL suppressPrompts) {
    ip = i;
   AllocConsole();
    freopen("CONOUT$", "w", stdout);
//std::cout <<"gg" << std::endl;
	FILE_SCENE_HEADER header;
	FILE_MATERIAL_HEADER mateH;
    hWnd = GetCOREInterface()->GetMAXHWnd();
    FILE* file = fopen(name, "rb");
    if (!file) {
        MessageBox(NULL, "Cannot open file", "Error", MB_OK);
        return FALSE;
    }
    //INode* node;

    header=LoadHeader(file);
	LoadMat(file,header.dwMaterialNum,i);
	DWORD typee;
	DWORD sizee;
	COLLISION_MESH_OBJECT_DESC_SAVELOAD	m_colMeshDesc;
	for(DWORD i2=1;i2<=header.dwObjectNum;i2++){
		fread(&typee, sizeof(DWORD), 1, file);
		printf("type: %#lx\n", typee); 
		fread(&sizee, sizeof(DWORD), 1, file);
		printf("sizee: %#lx\n", sizee); 
		if(typee == OBJECT_TYPE_BONE){
			CBaseObject(file);
			fread(&m_colMeshDesc,sizeof(m_colMeshDesc),1,file);
			 
			DrawBox(m_colMeshDesc.boundingBox,ii,i);

		}else if(typee == OBJECT_TYPE_MESH){
			long noww=ftell(file)+sizee;
			//printf("mesh %x \n",ftell(file));
			CBaseObject(file);
			CreateMeshFromData2(file,i);
			fseek(file,noww,SEEK_SET);

			
		}

	}
    fclose(file);
    return (1);
}
FILE_SCENE_HEADER MyImporter::LoadHeader(FILE* file)
{
	
FILE_SCENE_HEADER header;
	//inputFile.read(reinterpret_cast<char*>(&header), sizeof(header));
	fread(&header, sizeof(header), 1, file);
	  AllocConsole(); 
	 printf("��Ʒ����:%d\n",header.dwObjectNum);
	 printf("ģ������:%d\n",header.dwMaxMeshObjectNum);
	 printf("----END Load Header-----\n");
return header;
	 
}


INode* MyImporter::CreateMeshFromData2(FILE* file, Interface* gi) {
    TriObject* triObject = CreateNewTriObject();
    if (!triObject) return NULL;
    
    Mesh& mesh = triObject->GetMesh();
    
    FILE_MESH_HEADER pHeader;
    fread(&pHeader, sizeof(pHeader), 1, file);
	printf("meshflag=\%#lx\n",pHeader.meshFlag);
    
    // �����ڴ�
    VECTOR3* vert = new VECTOR3[pHeader.dwVertexNum];
	 VECTOR3* vert2 = new VECTOR3[pHeader.dwVertexNum];
    TVERTEX* m_TexP = new TVERTEX[pHeader.dwTexVertexNum];
    DWORD* m_ExtP = new DWORD[pHeader.dwExtVertexNum];
    
    // ��ȡ��������
    if (pHeader.dwVertexNum) {
        fread(vert, sizeof(VECTOR3) * pHeader.dwVertexNum, 1, file);
        mesh.setNumVerts(pHeader.dwVertexNum);
        
        // ���ö���
        for (int i = 0; i < pHeader.dwVertexNum; i++) {
            Point3 pt(vert[i].x, vert[i].z, vert[i].y);  // ע��Y��Z�ύ��
            mesh.setVert(i, pt);
        }
    }
    
    // ��ȡUV����
    if (pHeader.dwTexVertexNum) {
        fread(m_TexP, sizeof(TVERTEX), pHeader.dwTexVertexNum, file);
        mesh.setNumTVerts(pHeader.dwTexVertexNum);
        
        // ����UV����
        for (int i = 0; i < pHeader.dwTexVertexNum; i++) {
            UVVert uv(m_TexP[i].u, m_TexP[i].v, 0.0f);
            mesh.setTVert(i, uv);
        }
        
        // �������������ӳ��
        mesh.setMapSupport(1);
    }
    
    // ��ȡ���ⶥ�����ݣ������Ҫ��
    if (pHeader.dwExtVertexNum) {
        fread(m_ExtP, sizeof(DWORD), pHeader.dwExtVertexNum, file);
    }
    
    // ����������
    int totalFaces = 0;
    FILE_FACE_GROUP_HEADER fhead;
    
    // �ȼ���������
    long currentPos = ftell(file);
    for (DWORD i = 0; i < pHeader.dwFaceGroupNum; i++) {
        fread(&fhead, sizeof(fhead), 1, file);
        totalFaces += fhead.dwFacesNum;

        fseek(file, sizeof(WORD) * fhead.dwFacesNum * 3, SEEK_CUR);
    }
    fseek(file, currentPos, SEEK_SET);
	//=========================================
    
    // ����������
    mesh.setNumFaces(totalFaces);
    
    // �����UV������TVFace
    if (pHeader.dwTexVertexNum) {
       mesh.setNumTVFaces(totalFaces);
    }
    
    // ��ȡÿ�����������
    int faceOffset = 0;
    for (DWORD g = 0; g < pHeader.dwFaceGroupNum; g++) {
		//FILE_FACE_GROUP_HEADER
        fread(&fhead, sizeof(fhead), 1, file);
       // totalFaces += fhead.dwFacesNum;

        WORD* indices = new WORD[fhead.dwFacesNum * 3];
		TVERTEX * m_ptLightUV0= new TVERTEX[fhead.dwLightUVNum1];
		if(fhead.dwFacesNum)
			fread(indices, sizeof(WORD), fhead.dwFacesNum * 3, file);
		if(fhead.dwLightUVNum1)
			fread(m_ptLightUV0,sizeof(TVERTEX),fhead.dwLightUVNum1,file);
        
        // ������Ͳ���ID
        for (DWORD f = 0; f < fhead.dwFacesNum; f++) {
            int faceIndex = faceOffset + f;
            
            // ������Ķ�������
            mesh.faces[faceIndex].setVerts(
                indices[f * 3 + 0],
                indices[f * 3 + 2],  // ע��˳�����
                indices[f * 3 + 1]
            );
            
            // ����ƽ����Ͳ���
            mesh.faces[faceIndex].setSmGroup(1);
            mesh.faces[faceIndex].setMatID((MtlID)g);
            
            // ���ñ߿ɼ���
            mesh.faces[faceIndex].setEdgeVisFlags(1, 1, 1);
            
            // �����UV������TVFace
            if (pHeader.dwTexVertexNum) {
                mesh.tvFace[faceIndex].setTVerts(
                    indices[f * 3 + 0],
                    indices[f * 3 + 2],
                    indices[f * 3 + 1]
                );
            }
        }
        
        delete[] indices;
        faceOffset += fhead.dwFacesNum;
    }
    
    // �����ڴ�
    delete[] vert;
    delete[] m_TexP;
    delete[] m_ExtP;
    
    // ��������
    mesh.InvalidateTopologyCache();
    mesh.InvalidateGeomCache();
    
    // �����ڵ�
    INode* node = gi->CreateObjectNode(triObject);
    if (!node) {
        triObject->DeleteThis();
        return NULL;
    }
    
    // ���ýڵ�����
    node->SetName(_T("Game Model"));
	LoadPhy(file);
	printf("load fine phy%x\n",ftell(file));
	printf("vertexnum%x\n",pHeader.dwMaxVertexNum);
    //if (pHeader.meshFlag!=0x00000030)
		//fread(vert2,sizeof(VECTOR3),pHeader.dwVertexNum,file);
	printf("load fine meshflag%x\n",ftell(file));
    return node;
}

INode* MyImporter::CreateMeshFromData(FILE* file,Interface* gi ) {
   TriObject* triObject = CreateNewTriObject();
    if (!triObject) return NULL;

    Mesh* mesh = &triObject->GetMesh();
	FILE_MESH_HEADER	pHeader ;
	fread(&pHeader, sizeof(pHeader), 1, file);
	printf("------------this is mesh data------------\n");
	printf("%d",pHeader.dwVertexNum);
	 VECTOR3* vert= new VECTOR3[pHeader.dwVertexNum];
	 TVERTEX* m_TexP=new TVERTEX[pHeader.dwTexVertexNum];
	DWORD* m_ExtP=new DWORD[pHeader.dwExtVertexNum];
	//----read---------------
	if (pHeader.dwVertexNum)
		fread(vert, sizeof(VECTOR3)*pHeader.dwVertexNum, 1, file);
	
	if (pHeader.dwTexVertexNum)
		fread(m_TexP,sizeof(TVERTEX),pHeader.dwTexVertexNum,file);

	if (pHeader.dwExtVertexNum)
		fread(m_ExtP,sizeof(DWORD),pHeader.dwExtVertexNum,file);

    // ��ȡ��������
  printf("vertex num= %d\n",pHeader.dwVertexNum);
    //fread(&numVerts, sizeof(int), 1, file);
    mesh->setNumVerts(pHeader.dwVertexNum);
    
    // ��ȡ��������
    for (int i = 0; i<pHeader.dwVertexNum; i++) {
	 mesh->setVert(i ,Point3(vert[i].x, vert[i].z, vert[i].y));
       printf("v %.6f %.6f %.6f\n",vert[i].x, vert[i].z, vert[i].y);
    }
	//-----------------------------------
	printf("now start face");
    FILE_FACE_GROUP_HEADER fhead;
	for (DWORD i=1 ;i<=pHeader.dwFaceGroupNum;i++){
    // ��ȡ������
		fread(&fhead, sizeof(fhead), 1, file);
		mesh->setNumFaces(fhead.dwFacesNum);
		printf("Facenum= %d\n",fhead.dwFacesNum);
    // ��ȡ������
		WORD* a = new WORD[fhead.dwFacesNum *3];
		fread(a, sizeof(WORD),fhead.dwFacesNum *3 , file);
		for (int i = 0; i < fhead.dwFacesNum; i++) {
			int v0, v1, v2;
			v0=a[i * 3 + 0] + 1;
			v1=a[i * 3 + 2] + 1;
			v2=a[i * 3 + 1] + 1;
			mesh->faces[i].setVerts(v0, v1, v2);
			mesh->faces[i].setEdgeVisFlags(1, 1, 1);
			 mesh->faces[i].setSmGroup(1);
			printf("f %d,%d,%d\n",v0,v1,v2);
		}
	}
	printf("run here");
    INode* node = gi->CreateObjectNode(triObject);
    if (!node) {
        triObject->DeleteThis(); // �ͷ� TriObject
    //    fclose(file);
        return FALSE;
    }

    // ���ýڵ�����
    node->SetName(_T("Imported Object"));

  printf("run here2");
    // ��ȡ��������
 /*   int numTVerts;
    fread(&numTVerts, sizeof(int), 1, file);
    
    if (numTVerts > 0) {
        mesh->setNumTVerts(numTVerts);
        
        for (int i = 0; i < numTVerts; i++) {
            UVVert tvert;
            fread(&tvert, sizeof(UVVert), 1, file);
            mesh->setTVert(i, tvert);
        }
        
        mesh->setNumTVFaces(numFaces);
        for (int i = 0; i < numFaces; i++) {
            mesh->tvFace[i].setTVerts(
                mesh->faces[i].v[0],
                mesh->faces[i].v[1],
                mesh->faces[i].v[2]
            );
        }
    }*/
    
    // ��������
  //  mesh->buildNormals();
  //  mesh->buildBoundingBox();
   // 
    // �����ڵ�
  //  INode* node = ip->CreateObjectNode(triObject);
    
    // ��������
  //  char nameBuffer[256];
  ///  strncpy(nameBuffer, name, 255);
   // nameBuffer[255] = '\0';
  //  node->SetName((TCHAR*)"tesgt1");
    
    // ���ò���

    return node;
}

// DLL ��ڵ�
BOOL WINAPI DllMain(HINSTANCE hinstDLL, ULONG fdwReason, LPVOID lpvReserved) {
    if (fdwReason == DLL_PROCESS_ATTACH) {
       hInstance = hinstDLL;
        DisableThreadLibraryCalls(hinstDLL);
    }
    return TRUE;
}