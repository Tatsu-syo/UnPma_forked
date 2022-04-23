//
// MemmFile.cpp
// メモリーマップドファイル
//
// $Id: MemmFile.cpp,v 1.1 2005/09/18 05:49:28 halpussy Exp $
//

#include "MemmFile.h"

bool MemmapFile::Open( const char* fname ){
	Close();

	// ファイルを読みとり専用で開く
	hFile = ::CreateFile(fname,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL|FILE_FLAG_SEQUENTIAL_SCAN,NULL);
	if(hFile==INVALID_HANDLE_VALUE) return false;

	// サイズを取得
	dwSize = ::GetFileSize(hFile,NULL);

	if(!dwSize){
		// 0バイトのファイルはマッピング出来ないので適当に回避
		// (これにより、必ずbaseが有効になる)
		Ptr = &dwSize;
	}else{
		// マッピングオブジェクトを作る
		hMap = ::CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, 0, NULL);
		if(!hMap){
			::CloseHandle(hFile);
			hFile = INVALID_HANDLE_VALUE;
			return false;
		}
		// ビュー
		Ptr = ::MapViewOfFileEx(hMap, FILE_MAP_READ, 0, 0, 0 ,(VOID*)0x00000000);
		if(!Ptr){
			::CloseHandle(hMap);
			::CloseHandle(hFile);
			hFile = INVALID_HANDLE_VALUE;
			return false;
		}
	}
	return true;
}	

void MemmapFile::Close(){
	if(hFile!=INVALID_HANDLE_VALUE){
		// ヘンテコマッピングをしてなければここで解放
		if(Ptr!=&dwSize) ::UnmapViewOfFile(Ptr);
		if(hMap) ::CloseHandle(hMap);
		::CloseHandle(hFile);
		Init();
	}
}
