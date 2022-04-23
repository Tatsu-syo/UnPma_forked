//
// CMain.cpp
// LHA.dll互換/非互換APIの実装
//
// $Id: CMain.cpp,v 1.7 2005/09/19 09:51:48 halpussy Exp $
//

#include "Classes.h"

CMain::CMain(const HMODULE hDll) :
	baseCMain(hDll)
{
	SetMain(); //必ず実行してください。実行しないと誤作動の原因になります。
}

BOOL CMain::CheckArchive(LPCSTR lpszArc,const int iMode){
	HANDLE hArc;
	int    i,iCount=1;

	hArc=UnPmaOpenArchive(NULL, lpszArc, 0);
	if(!hArc)
		return FALSE;
	i=UnPmaFindFirst(hArc,"*",NULL);
	while(!i){
		i=UnPmaFindNext(hArc, NULL);
		iCount++;
		if(!iMode&&iCount>2)
			break; //BASICなら3メンバまでチェック
	}
	UnPmaCloseArchive(hArc);
	if(!i||i==-1)
		return TRUE;
	return FALSE;
}

BOOL CMain::ConfigDialog(const HWND hWnd, LPSTR lpszBuf, const int iMode){
	if( MessageBox(hWnd,
		"UnPma32.dll 0.02\n"
		"Build on 2005/11/07\n"
		"Copyright (C) HAL \"Minthe\" Pussywillow\n"
		"2005-2005\n"
		//"Under zlib/libpng license.\n\n"
		"http://f50.aaa.livedoor.jp/~minthe/pukiwiki/\n"
		"http://minthe.s57.xrea.com/",
			DLL_NAME,MB_OKCANCEL|MB_ICONINFORMATION) ==IDOK)
		return TRUE;
	else
		return FALSE;
}

BOOL CMain::QueryFunctionList(const int iFunction){
	switch(iFunction){
	case ISARC:
	case ISARC_GET_VERSION:
	case ISARC_GET_RUNNING:
	case ISARC_GET_CURSOR_INTERVAL:
	case ISARC_SET_CURSOR_INTERVAL:
	case ISARC_GET_BACK_GROUND_MODE:
	case ISARC_SET_BACK_GROUND_MODE:
	case ISARC_GET_CURSOR_MODE:
	case ISARC_SET_CURSOR_MODE:

	case ISARC_CHECK_ARCHIVE:
	case ISARC_GET_FILE_COUNT:
	case ISARC_QUERY_FUNCTION_LIST:
	case ISARC_CONFIG_DIALOG:

	case ISARC_OPEN_ARCHIVE:
	case ISARC_CLOSE_ARCHIVE:
	case ISARC_FIND_FIRST:
	case ISARC_FIND_NEXT:
	case ISARC_EXTRACT:
	//case ISARC_ADD:
	//case ISARC_MOVE:
	//case ISARC_DELETE:
	case ISARC_SETOWNERWINDOW:
	case ISARC_CLEAROWNERWINDOW:
	case ISARC_SETOWNERWINDOWEX:
	case ISARC_KILLOWNERWINDOWEX:

	case ISARC_GET_ARC_FILE_NAME:
	case ISARC_GET_ARC_FILE_SIZE:
	case ISARC_GET_ARC_DATE:
	case ISARC_GET_ARC_TIME:
	case ISARC_GET_ARC_IS_SFX_FILE:
	case ISARC_GET_ARC_ORIGINAL_SIZE:
	case ISARC_GET_ARC_COMPRESSED_SIZE:
	case ISARC_GET_ARC_RATIO:
	case ISARC_GET_ARC_WRITE_TIME_EX:
	case ISARC_GET_ARC_CREATE_TIME_EX:
	case ISARC_GET_ARC_ACCESS_TIME_EX:
	case ISARC_GET_FILE_NAME:
	case ISARC_GET_ORIGINAL_SIZE:
	case ISARC_GET_RATIO:
	case ISARC_GET_CRC:
	case ISARC_GET_ATTRIBUTE:
	case ISARC_GET_METHOD:
	case ISARC_GET_DATE:
	case ISARC_GET_TIME:
	case ISARC_GET_WRITE_TIME:
	case ISARC_GET_CREATE_TIME:
	case ISARC_GET_ACCESS_TIME:
	case ISARC_GET_WRITE_TIME_EX:
	case ISARC_GET_CREATE_TIME_EX:
	case ISARC_GET_ACCESS_TIME_EX:
	case ISARC_SET_ENUM_MEMBERS_PROC:
	case ISARC_CLEAR_ENUM_MEMBERS_PROC:
	case ISARC_GET_ARC_FILE_SIZE_EX:
	case ISARC_GET_ARC_ORIGINAL_SIZE_EX:
	case ISARC_GET_ARC_COMPRESSED_SIZE_EX:
	case ISARC_GET_ORIGINAL_SIZE_EX:
	case ISARC_GET_COMPRESSED_SIZE_EX:

	case ISARC_SETOWNERWINDOWEX64:
	case ISARC_KILLOWNERWINDOWEX64:
	case ISARC_SET_ENUM_MEMBERS_PROC64:
	case ISARC_CLEAR_ENUM_MEMBERS_PROC64:
	case ISARC_OPEN_ARCHIVE2:
	//case ISARC_GET_ARC_READ_SIZE:
	//case ISARC_GET_ARC_READ_SIZE_EX:

	//case SET_LANGUE_JAPANESE:
	//case SET_LANGUE_ENGLISH:
	//case SET_LANGUE_SPECIFIED:
	//case ISARC_SET_LANGUE_SPECIFIED:
	//case ISARC_SET_LANGUE_JAPANESE:
	//case ISARC_SET_LANGUE_ENGLISH:
	case ISARC_SET_PRIORITY:
		return TRUE;

	default:
		return FALSE;
	}
}
