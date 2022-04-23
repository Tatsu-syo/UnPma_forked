//
// COpenArchive.cpp
// 書庫閲覧APIの実装
//
// $Id: COpenArchive.cpp,v 1.4 2005/09/18 05:46:11 halpussy Exp $
//

#include "Classes.h"

extern CMain *mCMain;

int WINAPI UnPmaGetMemo(HARC _harc,LPSTR _lpBuffer,const int _nSize){
	COpenArchive *pArc = mCMain->FindObject(_harc);
	return pArc ? pArc->UnPma.GetMemo(_lpBuffer, _nSize) : -1;
}

COpenArchive::COpenArchive(const CMain *main, const HWND hWnd, LPCSTR lpszArc, const DWORD dwMode, LPCSTR lpszOpt) :
	baseCOpenArchive(main, hWnd, lpszArc, dwMode, lpszOpt)
{}

COpenArchive::~COpenArchive(){
}

bool COpenArchive::Open(){
	//開く前の処理
	if(!UnPma.Open(mszArchive))
		return false;
	if(!UnPma.SeekTop())
		return false;
	muOSType = 15; //OSTYPE_CPM
	if(UnPma.GetFilePointer())
		miSFX=1;
	return true;
}

void COpenArchive::SetAttribute(const DWORD dwAttribute, LPSTR szAttribute){
	if(!szAttribute)
		return;
	szAttribute[0] = dwAttribute&FA_DIREC ? 'd' :
						dwAttribute&FA_ARCH ? 'a' : '-';
	szAttribute[1] = dwAttribute&FA_SYSTEM ? 's' : '-';
	szAttribute[2] = dwAttribute&FA_HIDDEN ? 'h' : '-';
	szAttribute[3] = dwAttribute&FA_RDONLY ? 'r' : 'w';
	szAttribute[4] = 0;
}

int COpenArchive::FindBegin(){
	UnPma.SeekTop(); //一回目のUnPmaFindFirstだと重複してしまうが。。。
	return FindGet();
}

int COpenArchive::FindGet(){
	DWORD dwErr=0;

	do{
		if(!UnPma.ReadHeader(&mSubInfoEx)){
			dwErr=UnPma.GetLastError();
			if(dwErr==0)
				return -1;
			return (int)dwErr;
		}
	}while(!IsFilter(mSubInfoEx.szFileName));

	FixPathDirect(mSubInfoEx.szFileName);
	mllArcReadSize = UnPma.GetFilePointer();
	return 0;
}

