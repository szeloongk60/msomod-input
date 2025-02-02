#include "stdafx.h"
#include "iputy.h"
#include "typedef.h"
#include <stdmat.h> 
#include "max.h"
void SetMaterialColors(StdMat* material, DWORD dwDiffuse, DWORD dwAmbient, DWORD dwSpecular)
{
    if (!material) return;

    // �� DWORD ת��Ϊ Color
    Color diffuseColor(GetRValue(dwDiffuse) / 255.0f,
                       GetGValue(dwDiffuse) / 255.0f,
                       GetBValue(dwDiffuse) / 255.0f);

    Color ambientColor(GetRValue(dwAmbient) / 255.0f,
                       GetGValue(dwAmbient) / 255.0f,
                       GetBValue(dwAmbient) / 255.0f);

    Color specularColor(GetRValue(dwSpecular) / 255.0f,
                        GetGValue(dwSpecular) / 255.0f,
                        GetBValue(dwSpecular) / 255.0f);

    // ���ò�������
    material->SetDiffuse(diffuseColor, GetCOREInterface()->GetTime());
    material->SetAmbient(ambientColor, GetCOREInterface()->GetTime());
    material->SetSpecular(specularColor, GetCOREInterface()->GetTime());
}

void MyImporter::LoadMat(FILE* file,DWORD num,Interface*i)
{
	FILE_MATERIAL_HEADER Hmate;

	//FILE_SCENE_HEADER header;
	//inputFile.read(reinterpret_cast<char*>(&header), sizeof(header));
	for(DWORD num1=1;num1<=num;num1++)
	{
		fread(&Hmate, sizeof(Hmate), 1, file);
		StdMat* stdMaterial = NewDefaultStdMat();
	
		if (stdMaterial)
		{
        // ���ò��ʵ���������ɫ
        SetMaterialColors(stdMaterial, Hmate.dwDiffuse, Hmate.dwAmbient,Hmate.dwSpecular);
        // ���ò�������
        stdMaterial->SetName(Hmate.uszMtlName);
		

        // ��������ӵ�������
        GetCOREInterface()->GetMaterialLibrary().Add(stdMaterial);
		}
	}
	 
}
Matrix3 BuildMatrixFromNodeTM(const NODE_TM& tmData) {
    Matrix3 result;
    result.IdentityMatrix();

    // ����λ�� (Position)
    result.SetTrans(Point3(tmData.fPosX, tmData.fPosZ, tmData.fPosY));

    // ������ת (Rotation)
    Quat rotationQuat;
    rotationQuat.Set(Point3(tmData.fRotAxisX, tmData.fRotAxisY, tmData.fRotAxisZ).Normalize(), tmData.fRotAng);
    Matrix3 rotMatrix;
    rotationQuat.MakeMatrix(rotMatrix);

    // �������� (Scale)
    Point3 scaleValues(tmData.fScaleX, tmData.fScaleY, tmData.fScaleZ);
    Matrix3 scaleMatrix;
    scaleMatrix.IdentityMatrix();
    scaleMatrix.SetRow(0, Point3(scaleValues.x, 0.0f, 0.0f));
    scaleMatrix.SetRow(1, Point3(0.0f, scaleValues.y, 0.0f));
    scaleMatrix.SetRow(2, Point3(0.0f, 0.0f, scaleValues.z));

    // �ϲ��任
    result = rotMatrix * scaleMatrix;
    result.SetTrans(Point3(tmData.fPosX, tmData.fPosZ, tmData.fPosY));

    return result;
}

void MyImporter::CBaseObject(FILE* file)
{
	 FILE_BASE_OBJECT_HEADER baseh;
	
	fread(&baseh, sizeof(baseh), 1, file);
	DWORD *subb=new DWORD[baseh.dwChildObjectNum];
	printf("name=%s \n",baseh.szObjName);
	if(baseh.dwChildObjectNum>0){
	fread(subb, sizeof(DWORD), baseh.dwChildObjectNum, file);
	}
	


}
//#include <objects.h>  // ������ BoxObject ����
//#include "genobj.h"
void MyImporter::DrawBox(BOUNDING_BOX bbox,ImpInterface* i, Interface* gi){

 
}
void MyImporter::LoadPhy(FILE* fp)
{
    // 1. ��ȡ�ļ�ͷ
    FILE_PHYSIQUE_HEADER header;
    fread(&header, sizeof(FILE_PHYSIQUE_HEADER), 1, fp);/////////////////////
    
    // 2. �����ڴ沢��ȡ��������
    DWORD m_dwVertexNum = header.dwVertexNum;
    //m_pPhyVertexList = new PHYSIQUE_VERTEX[m_dwVertexNum];
    for(DWORD i = 0; i < m_dwVertexNum; i++)
    {
        PHYSIQUE_VERTEX phyVer;
        fread(&phyVer, sizeof(PHYSIQUE_VERTEX), 1, fp);/////////////////
        //m_pPhyVertexList[i].bBonesNum = phyVer.bBonesNum;
        // �ݴ�ƫ������������Ҫת��Ϊʵ��ָ��
        //m_pPhyVertexList[i].pBoneList = phyVer.pBoneList;
    }
    
    // 3. ��ȡ��������
  //  m_dwTotalBonesNum = header.dwTotalBonesNum;
  //  m_pBoneList = new BONE[m_dwTotalBonesNum];
    for(DWORD i = 0; i < header.dwTotalBonesNum; i++)
    {
        BONE bone;
        fread(&bone, sizeof(BONE), 1, fp);
       /* m_pBoneList[i].fWeight = bone.fWeight;
        // ��Ҫͨ�������ҵ���Ӧ�Ĺ�������
        m_pBoneList[i].pBone = g_pScene->GetObjectByIndex((DWORD)bone.pBone);
        m_pBoneList[i].v3Offset = bone.v3Offset;
        m_pBoneList[i].v3TangentOffset = bone.v3TangentOffset;
        m_pBoneList[i].v3NormalOffset = bone.v3NormalOffset;*/
    }
    
    // 4. �޸�����Ĺ����б�ָ��
   /* for(DWORD i = 0; i < m_dwVertexNum; i++)
    {
        // ��֮ǰ�洢��ƫ����ת��Ϊʵ�ʵ�ָ��
        DWORD offset = (DWORD)m_pPhyVertexList[i].pBoneList;
        m_pPhyVertexList[i].pBoneList = m_pBoneList + offset;
    }*/
    
}