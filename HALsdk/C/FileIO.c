//
// FileIO.c
// File IO using HANDLE.
//
// $Id: FileIO.c,v 1.2 2005/10/10 14:18:28 halpussy Exp $
//

#include "../HALsdk.h"
#include <windows.h>

INT64 GetFileSizei64(HANDLE hFile){
	LARGE_INTEGER li;
	li.QuadPart = 0;
	if(hFile==INVALID_HANDLE_VALUE){
		return -1;
	}
	li.LowPart=GetFileSize(hFile, &li.HighPart);
	return li.QuadPart;
}

INT64 GetFilePointeri64(HANDLE hFile){
	LARGE_INTEGER li;
	li.QuadPart = 0;
	if(hFile==INVALID_HANDLE_VALUE){
		return -1;
	}
	li.LowPart=SetFilePointer(hFile, 0, &li.HighPart, FILE_CURRENT);
	return li.QuadPart;
}

bool SetFilePointeri64(HANDLE hFile, INT64 llPointer){
	LARGE_INTEGER liPointer;
	liPointer.QuadPart = llPointer;
	if(hFile==INVALID_HANDLE_VALUE){
		return false;
	}
	if(SetFilePointer(hFile, liPointer.LowPart, &liPointer.HighPart, FILE_BEGIN)==-1)
		if(GetLastError()!=NO_ERROR){
			//ErrorMsg();
			return false;
		}
	return true;
}