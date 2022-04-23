//
// Dps.cpp
// Deaces Plugin System
//
// $Id: Dps.cpp,v 1.1 2005/06/02 15:32:26 halpussy Exp $
//

#include "Protocol.h"

//Plugin Name
void DPS_QuestionName(char *buf){
	lstrcpy(buf,"UnPma32*DPS");
}


// Version
int DPS_QuestionVersion(void){
	return UnPmaGetVersion();
}


//
int DPS_QuestionRMenuName(char *buf){
	return 0;
}


// Add to Compress Extentions
int DPS_QuestionMakeName(char *buf)
{
	//lstrcpy(buf,"PMA");
	return 0;
}

// Define .noa Extention
int DPS_QuestionMakeType(char *buf)
{
	//lstrcpy(buf,".pma");
	return 0;
}


// Define add to menu
int DPS_CallByStartUp(HWND hWnd)
{
	return 1;
}


//
int DPS_CallByRMenu(HWND hWnd,char *Path,const char *FileList)
{
	return 0;
}


// Setting
int DPS_CallByPluginMenu(HWND hWnd,char open,const char *FileName)
{
	if(UnPmaConfigDialog(hWnd,NULL,1))
		return 0;
	else
		return -1;
}


// List
int DPS_CallByCheckFile(HWND hWnd,const char *FileName,INDIVIDUALINFOFAR *FileInfo)
{
	HARC arc;
	INDIVIDUALINFO info;
	INDIVIDUALINFOFAR *c_info = FileInfo;
	int c = 0;

	if( !(arc = UnPmaOpenArchive(hWnd,FileName,0)) )
		return -1;
	if(UnPmaFindFirst(arc,"*",&info)){
		UnPmaCloseArchive(arc);
		return -1;
	}

	do{
		memcpy(&c_info[c],&info,sizeof(INDIVIDUALINFO));
		c++;
	}while(!UnPmaFindNext(arc,&info));

	UnPmaCloseArchive(arc);

	return c;
}


//Extract
int DPS_CallByExpandFile(HWND hWnd,const char *FileName,const char *path,const char *FileList)
{
	char cmd[2048];
	cmd[0] = '\0';
	lstrcpy(cmd,"x -- \"");
	lstrcat(cmd,FileName);
	lstrcat(cmd,"\" \"");
	lstrcat(cmd,path);
	lstrcat(cmd,"\" ");
	lstrcat(cmd, FileList);

	return UnPma(hWnd,cmd,NULL,0);
}


// Compress
int DPS_CallByMakeFile(HWND hWnd,const char *FileName,const char *path,const char *FileList){
	return 0;
}
