//
// Classes.h
// ìùçáà»äOÇÃAPIÇÃêÈåæ
//
// $Id: Protocol.h,v 1.1 2005/06/02 15:32:26 halpussy Exp $
//

#ifndef ProtocolH
#define ProtocolH

#include "../Classes.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct{
	DWORD 	dwOriginalSize;
	DWORD 	dwCompressedSize;
	DWORD	dwCRC;
	UINT	uFlag;
	UINT	uOSType;
	WORD	wRatio;
	WORD	wDate;
	WORD 	wTime;
	char	szFileName[FNAME_MAX32 + 1];
	char	dummy1[3];
	char	szAttribute[8];
	char	szMode[8];
} INDIVIDUALINFOFAR;

/* Meltice eXtention Dll (MXD) APIs */
int  WINAPI MxdGetVersion();
int  WINAPI MxdGetProtocolVersion();
bool WINAPI MxdConfigName(char *buf, const int mode, const int lang);
bool WINAPI MxdConfigDialog(const HWND wh, LPSTR, const int mode);
bool WINAPI MxdCheckArchive(const char *file_name);
bool WINAPI MxdUnpackArchive(const char *file_name, const char *output_dir);
bool WINAPI MxdPackArchive(const char *filenames, const char *base_dir);

/* Deaces PluginS (DPS) APIs */
void DPS_QuestionName(char *buf);
int  DPS_QuestionVersion();
int  DPS_QuestionRMenuName(char *buf);
int  DPS_QuestionMakeName(char *buf);
int  DPS_QuestionMakeType(char *buf);
int  DPS_CallByStartUp(HWND hWnd);
int  DPS_CallByRMenu(HWND hWnd,char *Path,const char *FileList);
int  DPS_CallByPluginMenu(HWND hWnd,char open,const char *FileName);
int  DPS_CallByCheckFile(HWND hWnd,const char *FileName,INDIVIDUALINFOFAR *FileInfo);
int  DPS_CallByExpandFile(HWND hWnd,const char *FileName,const char *path,const char *FileList);
int  DPS_CallByMakeFile(HWND hWnd,const char *FileName,const char *path,const char *FileList);

#ifdef __cplusplus
}
#endif

#endif