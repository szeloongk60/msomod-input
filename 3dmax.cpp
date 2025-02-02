#include "stdafx.h"
#include "iputy.h"

static MyImporterClassDesc MyImporterDesc;
ClassDesc2* GetMyImporterDesc() { return &MyImporterDesc; }

__declspec( dllexport ) const TCHAR *
LibDescription() { return "Import mso new format model   -make by laoda"; }

__declspec( dllexport ) int
LibNumberClasses() { return 1; }

__declspec( dllexport ) ClassDesc *
LibClassDesc(int i) {
	switch(i) {
		case 0: return new MyImporterClassDesc(); break;
		default: return 0; break;
		}

	}

// Return version so can detect obsolete DLLs
__declspec( dllexport ) ULONG 
LibVersion() { return VERSION_3DSMAX; }

// Let the plug-in register itself for deferred loading
__declspec( dllexport ) ULONG CanAutoDefer()
{
	return 1;
}
int MyImporterClassDesc::IsPublic() { 
    return TRUE; 
}

void* MyImporterClassDesc::Create(BOOL loading) { 
    return new MyImporter(); 
}

const TCHAR* MyImporterClassDesc::ClassName() { 
    return "MyImporter"; 
}

SClass_ID MyImporterClassDesc::SuperClassID() { 
    return SCENE_IMPORT_CLASS_ID; 
}

Class_ID MyImporterClassDesc::ClassID() { 
    return IMPORTERPLUGIN_CLASS_ID; 
}

const TCHAR* MyImporterClassDesc::Category() { 
    return ""; 
}

const TCHAR* MyImporterClassDesc::InternalName() { 
    return "MyImporter"; 
}



// MyImporter Implementation
MyImporter::MyImporter() {
    ip = NULL;
}

MyImporter::~MyImporter() {
}

int MyImporter::ExtCount() {
    return 1;
}

const TCHAR* MyImporter::Ext(int n) {
    return "mod";
}

const TCHAR* MyImporter::LongDesc() {
    return "My 3ds Max Scene Importer";
}

const TCHAR* MyImporter::ShortDesc() {
    return "MX Model Import";
}

const TCHAR* MyImporter::AuthorName() {
    return "Your Name";
}

const TCHAR* MyImporter::CopyrightMessage() {
    return "Copyright 2025";
}

const TCHAR* MyImporter::OtherMessage1() {
    return "";
}

const TCHAR* MyImporter::OtherMessage2() {
    return "";
}
HINSTANCE MyImporterClassDesc::HInstance() { 
    return hInstance; 
}
unsigned int MyImporter::Version() {
    return 100;
}

void MyImporter::ShowAbout(HWND hWnd) {
    MessageBox(hWnd, "My 3ds Max Importer", "About", MB_OK);
}