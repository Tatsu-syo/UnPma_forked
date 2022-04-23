//
// ConvStr.c
// 文字列変換
//
// $Id: ConvStr.c,v 1.1 2005/09/18 05:49:06 halpussy Exp $
//

#include "../HALsdk.h"

LPWSTR GetUnicodeString(LPCSTR lpszAnsi){
	//変換後の文字列は必ずFreeすること。
	LPWSTR lpszwUnicode;
	int i;

	if(!lpszAnsi) return NULL;
	i = MultiByteToWideChar(CP_ACP,0,lpszAnsi,-1,NULL,0);
	lpszwUnicode = (LPWSTR)malloc(i);
	MultiByteToWideChar(CP_ACP,0,lpszAnsi,-1,lpszwUnicode,i);

	return lpszwUnicode;
}

LPSTR GetAnsiString(LPCWSTR lpszwUnicode){
	//変換後の文字列は必ずFreeすること。
	LPSTR lpszAnsi;
	int i;

	if(!lpszwUnicode)	return NULL;
	i = WideCharToMultiByte(CP_ACP,0,lpszwUnicode,-1,NULL,0,NULL,NULL);
	lpszAnsi = (LPSTR)malloc(i);
	if(!lpszAnsi)		return NULL;
	WideCharToMultiByte(CP_ACP,0,lpszwUnicode,-1,lpszAnsi,i,NULL,NULL);

	return lpszAnsi;
}

bool MakeUnicodePath(LPCWSTR lpszwUnicode, LPWSTR lpszwPath){
	//lpszwPathに、Win32APIのUnicode版で扱える長いパスを入れます。
	//lpszwPathは wcslen(lpszwUnicode)*2+10 バイト確保されている必要があります。
	if( !lpszwUnicode||!lpszwPath )
		return false;
	wcscpy(lpszwPath, L"\\\\?\\");
	wcscat(lpszwPath, lpszwUnicode);
	return true;
}