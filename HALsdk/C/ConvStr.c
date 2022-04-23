//
// ConvStr.c
// ������ϊ�
//
// $Id: ConvStr.c,v 1.1 2005/09/18 05:49:06 halpussy Exp $
//

#include "../HALsdk.h"

LPWSTR GetUnicodeString(LPCSTR lpszAnsi){
	//�ϊ���̕�����͕K��Free���邱�ƁB
	LPWSTR lpszwUnicode;
	int i;

	if(!lpszAnsi) return NULL;
	i = MultiByteToWideChar(CP_ACP,0,lpszAnsi,-1,NULL,0);
	lpszwUnicode = (LPWSTR)malloc(i);
	MultiByteToWideChar(CP_ACP,0,lpszAnsi,-1,lpszwUnicode,i);

	return lpszwUnicode;
}

LPSTR GetAnsiString(LPCWSTR lpszwUnicode){
	//�ϊ���̕�����͕K��Free���邱�ƁB
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
	//lpszwPath�ɁAWin32API��Unicode�łň����钷���p�X�����܂��B
	//lpszwPath�� wcslen(lpszwUnicode)*2+10 �o�C�g�m�ۂ���Ă���K�v������܂��B
	if( !lpszwUnicode||!lpszwPath )
		return false;
	wcscpy(lpszwPath, L"\\\\?\\");
	wcscat(lpszwPath, lpszwUnicode);
	return true;
}