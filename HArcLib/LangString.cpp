//
// LangString.cpp
// ストリングテーブル読み込み
//
// $Id: LangString.cpp,v 1.2 2005/10/30 04:19:46 halpussy Exp $
//

#include "Classes.h"

LPSTR LoadLangString(const HMODULE hDll, const WORD wLangID, const WORD wStringID, LPSTR &lpszBuf){
	//レジストリが存在せず、かつモジュールのストリングテーブルが壊れている可能性があるので
	//NULLのチェックを行うべきかもしれません…

	//各クラスは、そのクラス自身のLoadLangStringを使用するべきです。

	FreeAndNil(lpszBuf);

	{//まずはレジストリを検索
		char	szStringID[5];
		char	szRegID[260];
		DWORD	dwType=REG_SZ;
		HKEY	hKey=NULL;
		ULONG	nSize;

		//文字列に変換
		sprintf(szStringID, "%u", wStringID);
		sprintf(szRegID, "%s\\%04X", STRING_REGKEY, wLangID);

		//レジストリキー取得(1)
		if(!RegOpenKeyEx(HKEY_CURRENT_USER,szRegID,0,KEY_READ,&hKey)){ //無いとは思いますが書き込みできない環境を考慮
			//値を取得
			RegQueryValueEx(hKey, szStringID, NULL, &dwType, NULL, &nSize);
			if( !(lpszBuf = (char*)malloc(nSize+1)) ){
				RegCloseKey(hKey);
				return NULL;
			}
			RegQueryValueEx(hKey, szStringID, NULL, &dwType, (LPBYTE)lpszBuf, &nSize);
			RegCloseKey(hKey);
			return lpszBuf;
		}

		//レジストリキー取得(2)
		if(!RegOpenKeyEx(HKEY_LOCAL_MACHINE,szRegID,0,KEY_READ,&hKey)){ //無いとは思いますが書き込みできない環境を考慮
			//値を取得
			RegQueryValueEx(hKey, szStringID, NULL, &dwType, NULL, &nSize);
			if( !(lpszBuf = (char*)malloc(nSize+1)) ){
				RegCloseKey(hKey);
				return NULL;
			}
			RegQueryValueEx(hKey, szStringID, NULL, &dwType, (LPBYTE)lpszBuf, &nSize);
			RegCloseKey(hKey);
			return lpszBuf;
		}
	}

	{//ついでモジュール内を検索
	 //Thx. HyperBeat (まともに使えるようにしていただきました)
		int i;
		WORD len;
		HRSRC hRsrc;
		HGLOBAL hRes;
		LPWSTR szwRes;

		if( !(hRsrc = FindResourceEx(hDll,RT_STRING,MAKEINTRESOURCE((wStringID>>4)+1),wLangID))
			||!(hRes = LoadResource(hDll,hRsrc))
			||!(szwRes = (LPWSTR)LockResource(hRes))
		){
			if( !(hRsrc = FindResourceEx(hDll,RT_STRING,MAKEINTRESOURCE((wStringID>>4)+1),0x0409))
				||!(hRes = LoadResource(hDll,hRsrc))
				||!(szwRes = (LPWSTR)LockResource(hRes))
			){
				ErrorMsg(); //自己診断
				return NULL;
			}
		}

		for (i = 0,len = 0;i < (wStringID&15)+1;i++){
			szwRes += len;
			len = (WORD)(*szwRes & 0xFFFF);
			szwRes++;
		}

		lpszBuf = (LPSTR)malloc(len*2+1);
		NullMemory(lpszBuf,len*2+1);
		//*(szwRes+len) = L'\0'; //LockResourceで取得した文字列には穴を開けてはならない。
		WideCharToMultiByte(CP_ACP,0,szwRes,len,lpszBuf,len*2,NULL,NULL); //len文字のみ変換する。
		return lpszBuf;
	}

	return NULL;
}
