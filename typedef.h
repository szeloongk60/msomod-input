// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once
/*#include <windows.h>

#include <fstream>
#include <iostream>
#include <tchar.h>*/
#define TEXTURE_MAP_TYPE_NUM 10
#define MAX_NAME_LEN 128
// TODO: reference additional headers your program requires here

struct VECTOR3 {
    float x, y, z;
	 VECTOR3(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {}
};
 struct MATRIX4
{
	float	_11;
	float	_12;
	float	_13;
	float	_14;

	float	_21;
	float	_22;
	float	_23;
	float	_24;

	float	_31;
	float	_32;
	float	_33;
	float	_34;

	float	_41;
	float	_42;
	float	_43;
	float	_44;

};
struct NODE_TM
{
	float		fRotAng;
	float		fPosX,fPosY,fPosZ;
	float		fRotAxisX,fRotAxisY,fRotAxisZ;
	float		fScaleX,fScaleY,fScaleZ;
	float		fScaleAxisX,fScaleAxisY,fScaleAxisZ;
	float		fScaleAngle;
	MATRIX4		mat4;
	MATRIX4		mat4Inverse;
};
struct FILE_BASE_OBJECT_HEADER
{
	DWORD				dwIndex;
	NODE_TM				TM;
	DWORD				dwChildObjectNum;
	DWORD				dwParentObjectIndex;
	char				szObjName[MAX_NAME_LEN];	
	
};
struct TVERTEX
{
	float u;
	float v;
};
#pragma pack(push,4)
struct FILE_MESH_HEADER {
    DWORD dwMaxVertexNum;
    DWORD dwVertexNum;
    DWORD dwOriginalVertexNum;
    DWORD dwExtVertexNum;
    DWORD dwTexVertexNum;
    DWORD dwMtlIndex;
    DWORD dwFaceGroupNum;
    DWORD meshFlag;
    DWORD dwGridIndex;
    VECTOR3 v3Dir;
};
#pragma pack(pop)

struct FILE_FACE_GROUP_HEADER
{
	DWORD					dwMtlIndex;
	DWORD					dwIndex;
	DWORD					dwFacesNum;
	DWORD					dwMaxFacesNum;
	DWORD					dwVertexIndexNum;
	DWORD					dwLightUVNum1;
	DWORD					dwLightUVNum2;
};	
struct FACE
{
	WORD		a;
	WORD		b;
	WORD		c;
	WORD		mtlid;
	VECTOR3		v3Normal[3];
};
enum OBJECT_TYPE
{
	OBJECT_TYPE_UNKNOWN = 0xf000000,
	OBJECT_TYPE_LIGHT = 0xf1000000,
	OBJECT_TYPE_CAMERA = 0xf2000000,
	OBJECT_TYPE_CAMERA_TARGET = 0xf3000000,
	OBJECT_TYPE_MESH = 0xf4000000,
	OBJECT_TYPE_BONE = 0xf5000000,
	OBJECT_TYPE_ILLUSION_MESH = 0xf6000000,
	OBJECT_TYPE_COLLISION_MESH = 0xf7000000,
	OBJECT_TYPE_MATERIAL = 0x00f00000,
	OBJECT_TYPE_MOTION = 0x0000f000
};
struct FILE_SCENE_HEADER
{
	DWORD			dwVersion;
	DWORD			dwObjectNum;
	DWORD			dwMaterialNum;

	DWORD			dwMaxMeshObjectNum;
	DWORD			dwMaxLightObjectNum;
	DWORD			dwMaxCameraObjectNum;
	DWORD			dwMaxBoneObjectNum;
};

	struct FILE_MATERIAL_HEADER
{
	DWORD			dwtype;
	DWORD			dwno;
	DWORD			dwTextureNum;
	DWORD			dwDiffuse;
	DWORD			dwAmbient;
	DWORD			dwSpecular;
	float			fTransparency;
	float			fShine;
	float			fShineStrength;
	TCHAR			uszTexmapFileName[TEXTURE_MAP_TYPE_NUM][MAX_NAME_LEN];
	TCHAR			uszMtlName[MAX_NAME_LEN];
	DWORD			dwIndex;
	DWORD			dwFlag;
};
	struct BOUNDING_BOX
{
	VECTOR3			v3Oct[8];
};

struct BOUNDING_SPHERE
{
	VECTOR3			v3Point;
	float			fRs;
};

struct BOUNDING_CYLINDER
{
	VECTOR3			v3Point;
	float			fRs;
	float			fAy;
};

struct BOUNDING_CAPSULE
{
	VECTOR3			v3From;
	VECTOR3			v3To;
	float			fRadius;
};
struct COLLISION_MESH_OBJECT_DESC_SAVELOAD
{
	BOUNDING_BOX		boundingBox;
	BOUNDING_SPHERE		boundingSphere;
	BOUNDING_CYLINDER	boundingCylinder;
	DWORD				dwObjIndex;
	char				szObjName[MAX_NAME_LEN];
};
//bone
struct BONE_OLD
{
	void*			pBone;
	float			fWeight;
	VECTOR3			v3Offset;
	VECTOR3			v3NormalOffset;
};

struct BONE
{
	void*			pBone;
	float			fWeight;
	VECTOR3			v3Offset;
	VECTOR3			v3NormalOffset;
	VECTOR3			v3TangentOffset;
};




struct PHYSIQUE_VERTEX
{
	BYTE			bBonesNum;
	BONE*			pBoneList;
};
struct FILE_PHYSIQUE_HEADER
{
	DWORD				dwVertexNum;
	DWORD				dwTotalBonesNum;
	DWORD				dwAllocBonesNum;

};