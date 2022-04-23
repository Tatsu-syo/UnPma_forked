//
// TempFile.h
// 一時ファイル
//
// $Id: TempFile.cpp,v 1.1 2005/09/18 05:49:28 halpussy Exp $
//

#include "../HALsdk.h"
#include "TempFile.h"

TempFile::TempFile() :
	mhFile(INVALID_HANDLE_VALUE), mbOpened(false)
{
	NullMemory(mszTempFile, MAX_PATH+1);
}

UINT TempFile::Create(LPCSTR lpszPrefix, LPCSTR lpszWorkDir){
	UINT uRet;

	if(!lpszPrefix||!lpszWorkDir)
		return 0;

	uRet = GetTempFileName(lpszWorkDir, lpszPrefix, 0, mszTempFile);
	if(!uRet) return 0;

	mhFile = CreateFile2(mszTempFile, GENERIC_READ|GENERIC_WRITE, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_TEMPORARY);
	if(mhFile==INVALID_HANDLE_VALUE){
		DeleteFile(mszTempFile);
		return 0;
	}

	mbOpened=true;
	return uRet;
}

bool TempFile::Done(LPCSTR lpszCopyTo){
	//書き戻したい場合はlpszCopyToをファイル名を含めて指定する
	bool bRet=true;

	if(mbOpened){
		CloseHandle(mhFile);
		if(lpszCopyTo){
			MakeDirectory(lpszCopyTo);
			if(!CopyFile(mszTempFile, lpszCopyTo, FALSE)){
				//ErrorMsg();
				bRet=false;
			}
		}

		DeleteFile(mszTempFile);
	}
	mbOpened=false;

	return bRet;
}