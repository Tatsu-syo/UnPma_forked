//PmaSfx.exe
//PMA SelF eXtractor

#include "../HALsdk/HALsdk.h"
#include "../UnPma/CPmaStream.h"
#include <stdio.h>
#include <windows.h>
#include <shlobj.h>
#include "resource.h"

#define SFXName "PMA Self Extract Archiver"

//#pragma comment(linker, "/nowin98")
//#pragma comment(linker, "/fixed")

class PmaSfx
{
public:
	BOOL DlgProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
	BOOL ConfirmProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam);

	bool bExtract;
	bool bContinue;
	INT64 llSize;
	HWND hWindow;

private:
	bool DirectoryRefDialog(LPSTR Result);
	void DoExtract(HWND hWnd);

	LPSTR szName;
	BROWSEINFO BI;

	PmaStream UnPma;
};
