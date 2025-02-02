#ifndef MY_IMPORTER_H
#define MY_IMPORTER_H

#include "max.h"
#include "istdplug.h"
#include "iparamb2.h"
#include "iparamm2.h"
#include "typedef.h"

extern ClassDesc2* GetMyImporterDesc();

#define IMPORTERPLUGIN_CLASS_ID Class_ID(0x12345678, 0x87654322)

class MyImporter : public SceneImport {
private:
    Interface* ip;
    
public:
    MyImporter();
    ~MyImporter();
    
    // Required methods
    int ExtCount();
    const TCHAR* Ext(int n);
    const TCHAR* LongDesc();
    const TCHAR* ShortDesc();
    const TCHAR* AuthorName();
    const TCHAR* CopyrightMessage();
    const TCHAR* OtherMessage1();
    const TCHAR* OtherMessage2();
    unsigned int Version();
    void ShowAbout(HWND hWnd);
	FILE_SCENE_HEADER MyImporter::LoadHeader(FILE* file);
	void LoadMat(FILE* file,DWORD num,Interface* i);
	void CBaseObject(FILE* file);
	void DrawBox(BOUNDING_BOX bbox,ImpInterface* i, Interface* gi);
	//void printText(const char *text);

    // Import method
    int DoImport(const TCHAR* name, ImpInterface* ii, Interface* i, BOOL suppressPrompts=FALSE);
	void LoadPhy(FILE* fp);
    
private:
    INode* CreateMeshFromData(FILE* file,Interface* gi);
	INode* CreateMeshFromData2(FILE* file,Interface* gi);
};

class MyImporterClassDesc : public ClassDesc2 {
public:
    int IsPublic();
    void* Create(BOOL loading = FALSE);
    const TCHAR* ClassName();
    SClass_ID SuperClassID();
    Class_ID ClassID();
    const TCHAR* Category();
    const TCHAR* InternalName();
	HINSTANCE HInstance();
	
	
};

#endif // MY_IMPORTER_H