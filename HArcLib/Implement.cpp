//
// Implement.cpp
// 基本クラスの実装
//
// $Id: Implement.cpp,v 1.8 2005/10/30 04:53:31 halpussy Exp $
//

#include "Classes.h"
#include <process.h>

//Implementation of baseCMain
baseCMain::baseCMain(const HMODULE hDll) :
	mbRunning(FALSE), mwVer(0), mwSubVer(0), mszBuffer(NULL)
{	
	NullMemory(&mOwner, sizeof(OWNERWINDOWINFO));
	NullMemory(&mMembers, sizeof(MEMBERSPROCINFO));
	NullMemory(&mThreadInfo, sizeof(THREADINFO));

	mThreadInfo.hDll      = hDll;
	mThreadInfo.uMsg      = RegisterWindowMessage(WM_ARCEXTRACT);
	mThreadInfo.wLangID   = GetUserDefaultLangID();
	mThreadInfo.iPriority = 0;
	mThreadInfo.wInterval = 80;
	mThreadInfo.bBG       = FALSE;
	mThreadInfo.bCur      = TRUE;

	HRSRC Res = ::FindResource(hDll,MAKEINTRESOURCE(VS_VERSION_INFO),RT_VERSION);
	if(Res){
		HGLOBAL hRes = ::LoadResource(hDll,Res);
		if(hRes){
			VS_FIXEDFILEINFO *Verinfo = (VS_FIXEDFILEINFO*)((char*)::LockResource(hRes)+40);
			if(Verinfo){
				mwVer = (WORD)((Verinfo->dwFileVersionMS >> 14) * 25 + (Verinfo->dwFileVersionMS & 0xffff));
				mwSubVer = (WORD)((Verinfo->dwFileVersionLS >> 14) * 25 + (Verinfo->dwFileVersionLS & 0xffff));
			}
		}
	}
}

baseCMain::~baseCMain(){
	FreeAndNil(mszBuffer);
}

bool baseCMain::Initialize(){
	return true;
}

void baseCMain::Destroy(){}

/*inline*/ COpenArchive* baseCMain::FindObject(HARC hArc){
	return mhArc.Find((COpenArchive*)hArc);
}

int baseCMain::Command(const HWND hWnd, LPCSTR lpszCmd, LPSTR lpszBuf, const DWORD dwSize){
	int Ret;
	if(mbRunning) return ERROR_ALREADY_RUNNING;
	mbRunning = RUNMODE;

	{
		CCommand *pCommand = new CCommand(mCMain, hWnd, lpszCmd, lpszBuf, dwSize, mOwner, mMembers, mThreadInfo);
		pCommand->BeginThread();
		Ret = pCommand->GetError();
		delete pCommand;
	}

	mbRunning = FALSE;
	return Ret;
}

WORD baseCMain::GetDllVersion(){
	return mwVer;
}

WORD baseCMain::GetDllSubVersion(){
	return mwSubVer;
}

WORD baseCMain::GetCursorInterval(){
	return mThreadInfo.wInterval;
}

BOOL baseCMain::SetCursorInterval(const WORD wInterval){
	mThreadInfo.wInterval=wInterval;
	return TRUE;
}

BOOL baseCMain::GetBackGroundMode(){
	return mThreadInfo.bBG;
}

BOOL baseCMain::SetBackGroundMode(const BOOL bBG){
	mThreadInfo.bBG=bBG;
	return TRUE;
}

BOOL baseCMain::GetCursorMode(){
	return mThreadInfo.bCur;
}

BOOL baseCMain::SetCursorMode(const BOOL bCur){
	mThreadInfo.bCur=bCur;
	return TRUE;
}

BOOL baseCMain::GetRunning(){
	return mbRunning;
}

BOOL baseCMain::CheckArchive(LPCSTR lpszArc,const int iMode){
	return FALSE;
}

int baseCMain::GetFileCount(LPCSTR lpszArc){
	COpenArchive *pArc = OpenArchive(NULL, lpszArc, 0);
	int i=1;
	if(!pArc)
		return -1;
	if(pArc->FindFirst("*",NULL)){
		CloseArchive(pArc);
		return 0;
	}
	while(!pArc->FindNext(NULL))
		i++;
	CloseArchive(pArc);
	return i;
}

BOOL baseCMain::ConfigDialog(const HWND hWnd, LPSTR lpszBuf, const int iMode){
	return FALSE;
}

BOOL baseCMain::QueryFunctionList(const int iFunction){
	return FALSE;
}

LPSTR baseCMain::LoadLangString(const WORD wStringID, LPSTR &lpszBuf){
	return ::LoadLangString(mThreadInfo.hDll, mThreadInfo.wLangID, wStringID, lpszBuf);
}

COpenArchive* baseCMain::OpenArchive(const HWND hWnd, LPCSTR lpszArc, const DWORD dwMode, LPCSTR lpszOpt){
	COpenArchive *pArc = new COpenArchive(mCMain, hWnd,lpszArc,dwMode,lpszOpt);
	if(pArc->SetArchiveInfo()&&pArc->Open()){
		mhArc.Add(pArc);
		return pArc;
	}
	delete pArc;
	return NULL;
}

int baseCMain::CloseArchive(HARC hArc){
	COpenArchive *pArc = FindObject(hArc);
	if(!pArc) return ERROR_HARC_ISNOT_OPENED;
	mhArc.Remove(pArc);
	return 0;
}

BOOL baseCMain::SetOwnerWindow(const HWND hWnd){
	if(mbRunning||mOwner.iType||!hWnd) return FALSE;
	mOwner.iType = 1;
	mOwner.hWnd = hWnd;
	return TRUE;
}

BOOL baseCMain::ClearOwnerWindow(){
	if(mbRunning||mOwner.iType!=1) return FALSE;
	NullMemory(&mOwner, sizeof(OWNERWINDOWINFO));
	return TRUE;
}

BOOL baseCMain::SetOwnerWindowEx(const HWND hWnd, LPARCHIVERPROC lpArcProc){
	if(mbRunning||mOwner.iType||(!hWnd&&!lpArcProc)) return FALSE;
	mOwner.iType = 2;
	mOwner.hWnd = hWnd;
	mOwner.ArcProc = lpArcProc;
	return TRUE;
}

BOOL baseCMain::KillOwnerWindowEx(const HWND hWnd){
	if(mbRunning||mOwner.iType!=2) return FALSE;
	if(hWnd)
		if(mOwner.hWnd!=hWnd) return FALSE;
	NullMemory(&mOwner, sizeof(OWNERWINDOWINFO));
	return TRUE;
}

BOOL baseCMain::SetOwnerWindowEx64(const HWND hWnd, LPARCHIVERPROC lpArcProc, const DWORD dwStructSize){
	if(mbRunning||mOwner.iType||(!hWnd&&!lpArcProc)) return FALSE;
	switch(dwStructSize){
		case sizeof(EXTRACTINGINFO):		mOwner.iType=3; break;
		case sizeof(EXTRACTINGINFOEX):		mOwner.iType=4; break;
		case sizeof(EXTRACTINGINFOEX32):	mOwner.iType=5; break;
		case sizeof(EXTRACTINGINFOEX64):	mOwner.iType=6; break;
		default: return FALSE;
	}
	mOwner.hWnd = hWnd;
	mOwner.ArcProc = lpArcProc;
	return TRUE;
}

BOOL baseCMain::KillOwnerWindowEx64(const HWND hWnd){
	if(mbRunning||(mOwner.iType!=3&&mOwner.iType!=4&&mOwner.iType!=5&&mOwner.iType!=6)) return FALSE;
	if(hWnd)
		if(mOwner.hWnd!=hWnd) return FALSE;
	NullMemory(&mOwner, sizeof(OWNERWINDOWINFO));
	return TRUE;
}

BOOL baseCMain::SetEnumMembersProc(WND_ENUMMEMBPROC lpEnumProc){
	if(mbRunning||mMembers.iType||!lpEnumProc) return FALSE;
	mMembers.iType = 1;
	mMembers.MembersProc = lpEnumProc;
	return TRUE;
}

BOOL baseCMain::ClearEnumMembersProc(){
	if(mbRunning||mMembers.iType!=1) return FALSE;
	NullMemory(&mMembers, sizeof(MEMBERSPROCINFO));
	return TRUE;
}

BOOL baseCMain::SetEnumMembersProc64(WND_ENUMMEMBPROC lpEnumProc, const DWORD dwStructSize){
	if(mbRunning||mMembers.iType||!lpEnumProc) return FALSE;
	switch(dwStructSize){
		case sizeof(ENUM_MEMBER_INFO):		mMembers.iType=2; break;
		case sizeof(ENUM_MEMBER_INFO64):	mMembers.iType=3; break;
		default: return FALSE;
	}
	mMembers.MembersProc = lpEnumProc;
	return TRUE;
}

BOOL baseCMain::ClearEnumMembersProc64(){
	if(mbRunning||(mMembers.iType!=2&&mMembers.iType!=3)) return FALSE;
	NullMemory(&mMembers, sizeof(MEMBERSPROCINFO));
	return TRUE;
}

void baseCMain::SetLangueSpecified(const LANGID wLanguage){
	if(GetRunning()) return;
	mThreadInfo.wLangID = wLanguage;
}

BOOL baseCMain::SetPriority(const int iPriority){
	if(GetRunning())
		return FALSE;
	mThreadInfo.iPriority = iPriority;
	return TRUE;
}

//Implementation of baseCOpenArchive
baseCOpenArchive::baseCOpenArchive(const CMain *main, const HWND hWnd, LPCSTR lpszArc, const DWORD dwMode, LPCSTR lpszOpt) :
	mszOption(NULL), mllArcOrgSize(0), mllArcCompSize(0), mllArcReadSize(0), miSFX(0)
{
	mhWnd   = hWnd;
	mCMain  = (CMain*)main;
	strcpy(mszArchive, lpszArc);
	mdwMode = dwMode;
	if(lpszOpt){
		mszOption = (char*)malloc(strlen(lpszOpt)+1);
		strcpy(mszOption, lpszOpt);
	}

	NullMemory(&mSubInfoEx, sizeof(INDIVIDUALINFOEX));
}

baseCOpenArchive::~baseCOpenArchive(){
	FreeAndNil(mszOption);
}

bool baseCOpenArchive::SetArchiveInfo(){
	WIN32_FIND_DATA Find;
	NullMemory(&Find, sizeof(WIN32_FIND_DATA));

	GetFileInfo(mszArchive, &Find);

	mftArcCreate   = Find.ftCreationTime;
	mftArcAccess   = Find.ftLastAccessTime;
	mftArcWrite    = Find.ftLastWriteTime;
	mllArcFileSize = MAKELONGLONG(Find.nFileSizeLow, Find.nFileSizeHigh);

	return true;
}

bool baseCOpenArchive::IsFilter(LPSTR szFileName){
	UINT u=0;
	for(;u<mcFilter.count();u++){
		if(IsMatchWildcard(mcFilter[u],szFileName))
			return true;
	}
	return false;
}

bool baseCOpenArchive::Open(){
	return true;
}

void baseCOpenArchive::SetAttribute(const DWORD dwAttribute, LPSTR szAttribute){
	if(!szAttribute)
		return;
	szAttribute[0] = dwAttribute&FA_DIREC ? 'D' :
						dwAttribute&FA_ARCH ? 'A' : '-';
	szAttribute[1] = dwAttribute&FA_SYSTEM ? 'S' : '-';
	szAttribute[2] = dwAttribute&FA_HIDDEN ? 'H' : '-';
	szAttribute[3] = dwAttribute&FA_RDONLY ? 'R' : 'W';
	szAttribute[4] = dwAttribute&FA_ENCRYPT ? 'G' : '-';
	szAttribute[5] = 0;
}

void baseCOpenArchive::AddTotal(){
	mllArcOrgSize  += mSubInfoEx.llOriginalSize;
	mllArcCompSize += mSubInfoEx.llCompressedSize;
}

void baseCOpenArchive::ClearTotal(){
	mllArcOrgSize = mllArcCompSize = 0;
}

int baseCOpenArchive::FindBegin(){
	return -1;
}

int baseCOpenArchive::FindGet(){
	return -1;
}

int baseCOpenArchive::FindFirstEx(LPCSTR lpszWild, LPINDIVIDUALINFOEX lpSubInfoEx){
	int Ret;
	if(!lpszWild) return 1;

	//Init
	mcFilter.Discard();
	if(mcFilter.parse(lpszWild, 0)){
		return ERROR_ENOUGH_MEMORY;
	}
	ClearTotal();

	//Begin
	if( (Ret = FindBegin()) ){
		mcFilter.Discard();
		return Ret;
	}
	AddTotal();
	if(lpSubInfoEx)
		memcpy(lpSubInfoEx, &mSubInfoEx, sizeof(INDIVIDUALINFOEX));
	return Ret;
}

int baseCOpenArchive::FindNextEx(LPINDIVIDUALINFOEX lpSubInfoEx){
	int Ret;
	if(!mcFilter.count()) return ERROR_NOT_SEARCH_MODE;

	//Get
	if( (Ret = FindGet()) ){
		mcFilter.Discard();
		return Ret;
	}
	AddTotal();
	if(lpSubInfoEx)
		memcpy(lpSubInfoEx, &mSubInfoEx, sizeof(INDIVIDUALINFOEX));
	return Ret;
}

int baseCOpenArchive::FindFirst(LPCSTR lpszWild, LPINDIVIDUALINFO lpSubInfo){
	int Ret;
	if(!lpszWild) return 1;

	//Init
	mcFilter.Discard();
	if(mcFilter.parse(lpszWild, 0)){
		return ERROR_ENOUGH_MEMORY;
	}
	ClearTotal();

	//Begin
	if( (Ret = FindBegin()) ){
		mcFilter.Discard();
		return Ret;
	}
	AddTotal();

	//Exchange
	if(lpSubInfo){
		NullMemory(lpSubInfo,sizeof(INDIVIDUALINFO));
		lpSubInfo->dwOriginalSize=(DWORD)mSubInfoEx.llOriginalSize;
		lpSubInfo->dwCompressedSize=(DWORD)mSubInfoEx.llCompressedSize;
		lpSubInfo->dwCRC=mSubInfoEx.dwCRC;
		lpSubInfo->uFlag=mSubInfoEx.uFlag;
		lpSubInfo->uOSType=mSubInfoEx.uOSType;
		lpSubInfo->wRatio=mSubInfoEx.wRatio;
		{
			WORD wDate, wTime;
			if(FileTimeToDosTime(&mSubInfoEx.ftWriteTime,&wDate,&wTime))
			lpSubInfo->wDate = wDate;
			lpSubInfo->wTime = wTime;
		}
		strncpy(lpSubInfo->szFileName, mSubInfoEx.szFileName, FNAME_MAX32);
		strncpy(lpSubInfo->szMode, mSubInfoEx.szMode, 7);
		SetAttribute(mSubInfoEx.dwAttributes, lpSubInfo->szAttribute);
	}
	return Ret;
}

int baseCOpenArchive::FindNext(LPINDIVIDUALINFO lpSubInfo){
	int Ret;
	if(!mcFilter.count()) return ERROR_NOT_SEARCH_MODE;

	//Get
	if( (Ret = FindGet()) ){
		mcFilter.Discard();
		return Ret;
	}
	AddTotal();

	//Exchange
	if(lpSubInfo){
		NullMemory(lpSubInfo,sizeof(INDIVIDUALINFO));
		lpSubInfo->dwOriginalSize=(DWORD)mSubInfoEx.llOriginalSize;
		lpSubInfo->dwCompressedSize=(DWORD)mSubInfoEx.llCompressedSize;
		lpSubInfo->dwCRC=mSubInfoEx.dwCRC;
		lpSubInfo->uFlag=mSubInfoEx.uFlag;
		lpSubInfo->uOSType=mSubInfoEx.uOSType;
		lpSubInfo->wRatio=mSubInfoEx.wRatio;
		{
			WORD wDate, wTime;
			if(FileTimeToDosTime(&mSubInfoEx.ftWriteTime,&wDate,&wTime))
			lpSubInfo->wDate = wDate;
			lpSubInfo->wTime = wTime;
		}
		strncpy(lpSubInfo->szFileName, mSubInfoEx.szFileName, FNAME_MAX32);
		strncpy(lpSubInfo->szMode, mSubInfoEx.szMode, 7);
		SetAttribute(mSubInfoEx.dwAttributes, lpSubInfo->szAttribute);
	}
	return Ret;
}

int baseCOpenArchive::Add(LPCSTR lpszFileName, DWORD dwMode){
	return -1;
}

int baseCOpenArchive::Extract(LPCSTR lpszFileName, LPCSTR lpszDirName, DWORD dwMode){
	char szCmd[2048];
	if(!(dwMode&EXTRACT_FOUND_FILE)&&!(dwMode&EXTRACT_NAMED_FILE))
		return ERROR_NOT_FILENAME;
	if((dwMode&EXTRACT_NAMED_FILE)&&!lpszFileName)
		return ERROR_NOT_FILENAME;
	sprintf(szCmd, EXTRACT_CMD, mszArchive,
			lpszDirName ? lpszDirName : "",
			dwMode&EXTRACT_FOUND_FILE ? mSubInfoEx.szFileName : lpszFileName);
	return mCMain->Command(mhWnd, szCmd, NULL, 0);
}

int baseCOpenArchive::Move(LPCSTR lpszFileName, DWORD dwMode){
	return -1;
}

int baseCOpenArchive::Delete(LPCSTR lpszFileName, DWORD dwMode){
	return -1;
}

int baseCOpenArchive::GetArcFileName(LPSTR lpszBuf, const int iSiz){
	int l = strlen(mszArchive);
	if(!lpszBuf)
		return -1;
	strncpy(lpszBuf, mszArchive, iSiz);
	return l<iSiz ? 0 : l+1;
}

DWORD baseCOpenArchive::GetArcFileSize(){
	return (DWORD)mllArcFileSize;
}

DWORD baseCOpenArchive::GetArcOriginalSize(){
	return (DWORD)mllArcOrgSize;
}

DWORD baseCOpenArchive::GetArcCompressedSize(){
	return (DWORD)mllArcCompSize;
}

BOOL baseCOpenArchive::GetArcFileSizeEx(INT64 *lpllsize){
	if(!lpllsize)
		return FALSE;
	*lpllsize = mllArcFileSize;
	return TRUE;
}

BOOL baseCOpenArchive::GetArcOriginalSizeEx(INT64 *lpllsize){
	if(!lpllsize)
		return FALSE;
	*lpllsize = mllArcOrgSize;
	return TRUE;
}

BOOL baseCOpenArchive::GetArcCompressedSizeEx(INT64 *lpllsize){
	if(!lpllsize)
		return FALSE;
	*lpllsize = mllArcCompSize;
	return TRUE;
}

DWORD baseCOpenArchive::GetArcReadSize(){
	return (DWORD)mllArcReadSize;
}

BOOL baseCOpenArchive::GetArcReadSizeEx(INT64 *lpllsize){
	if(!lpllsize)
		return FALSE;
	*lpllsize = mllArcReadSize;
	return TRUE;
}

WORD baseCOpenArchive::GetArcRatio(){
	if(!mllArcOrgSize) return 0;
	return (WORD)( (UINT64)(mllArcCompSize*1000/mllArcOrgSize) );
}

WORD baseCOpenArchive::GetArcDate(){
	WORD wDate(-1);
	FileTimeToDosTime(&mftArcWrite, &wDate, NULL);
	return wDate;
}

WORD baseCOpenArchive::GetArcTime(){
	WORD wTime(-1);
	FileTimeToDosTime(&mftArcWrite, NULL, &wTime);
	return wTime;
}

BOOL baseCOpenArchive::GetArcWriteTimeEx(FILETIME *lpfttime){
	if(!lpfttime)
		return FALSE;
	*lpfttime = mftArcWrite;
	return TRUE;
}

BOOL baseCOpenArchive::GetArcCreateTimeEx(FILETIME *lpfttime){
	if(!lpfttime)
		return FALSE;
	*lpfttime = mftArcCreate;
	return TRUE;
}

BOOL baseCOpenArchive::GetArcAccessTimeEx(FILETIME *lpfttime){
	if(!lpfttime)
		return FALSE;
	*lpfttime = mftArcAccess;
	return TRUE;
}

UINT baseCOpenArchive::GetArcOSType(){
	return muOSType;
}

int baseCOpenArchive::IsSFXFile(){
	return miSFX;
}

int baseCOpenArchive::GetFileName(LPSTR lpszBuf, const int iSiz){
	int l = strlen(mSubInfoEx.szFileName);
	if(!lpszBuf)
		return -1;
	strncpy(lpszBuf, mSubInfoEx.szFileName, iSiz);
	return l<iSiz ? 0 : l+1;
}

int baseCOpenArchive::GetMethod(LPSTR lpszBuf, const int iSiz){
	int l = strlen(mSubInfoEx.szMode);
	if(!lpszBuf)
		return -1;
	strncpy(lpszBuf, mSubInfoEx.szMode, iSiz);
	return l<iSiz ? 0 : l+1;
}

DWORD baseCOpenArchive::GetOriginalSize(){
	return (DWORD)mSubInfoEx.llOriginalSize;
}

DWORD baseCOpenArchive::GetCompressedSize(){
	return (DWORD)mSubInfoEx.llCompressedSize;
}

BOOL baseCOpenArchive::GetOriginalSizeEx(INT64 *lpllsize){
	if(!lpllsize)
		return FALSE;
	*lpllsize = mSubInfoEx.llOriginalSize;
	return TRUE;
}

BOOL baseCOpenArchive::GetCompressedSizeEx(INT64 *lpllsize){
	if(!lpllsize)
		return FALSE;
	*lpllsize = mSubInfoEx.llCompressedSize;
	return TRUE;
}

WORD baseCOpenArchive::GetRatio(){
	return mSubInfoEx.wRatio;
}

WORD baseCOpenArchive::GetDate(){
	WORD wDate(-1);
	FileTimeToDosTime(&mSubInfoEx.ftWriteTime, &wDate, NULL);
	return wDate;
}

WORD baseCOpenArchive::GetTime(){
	WORD wTime(-1);
	FileTimeToDosTime(&mSubInfoEx.ftWriteTime, NULL, &wTime);
	return wTime;
}

DWORD baseCOpenArchive::GetWriteTime(){
	return FileTimeToUTC(mSubInfoEx.ftWriteTime);
}

DWORD baseCOpenArchive::GetAccessTime(){
	return FileTimeToUTC(mSubInfoEx.ftAccessTime);
}

DWORD baseCOpenArchive::GetCreateTime(){
	return FileTimeToUTC(mSubInfoEx.ftCreateTime);
}

BOOL baseCOpenArchive::GetWriteTimeEx(FILETIME *lpfttime){
	if(!lpfttime)
		return FALSE;
	*lpfttime = mSubInfoEx.ftWriteTime;
	return TRUE;
}

BOOL baseCOpenArchive::GetCreateTimeEx(FILETIME *lpfttime){
	if(!lpfttime)
		return FALSE;
	*lpfttime = mSubInfoEx.ftCreateTime;
	return TRUE;
}

BOOL baseCOpenArchive::GetAccessTimeEx(FILETIME *lpfttime){
	if(!lpfttime)
		return FALSE;
	*lpfttime = mSubInfoEx.ftAccessTime;
	return TRUE;
}

DWORD baseCOpenArchive::GetCRC(){
	return mSubInfoEx.uOSType;
}

int baseCOpenArchive::GetAttribute(){
	return (int)mSubInfoEx.dwAttributes;
}

UINT baseCOpenArchive::GetOSType(){
	return mSubInfoEx.uOSType;
}

//Implementation of baseCCommand
baseCCommand::baseCCommand(const CMain *main, const HWND hWnd, LPCSTR lpszCmd, LPSTR lpszOut, const DWORD dwSize,
		const OWNERWINDOWINFO Owner, const MEMBERSPROCINFO Members, const THREADINFO ThreadInfo) :
	bf(lpszOut, dwSize), miWarning(0), miError(0), mnState(0),
	mszBuffer(NULL), mbChangeCursor(true), mlpszCmd(lpszCmd)
{
	mhWnd       = hWnd;
	mCMain      = (CMain*)main;
	mOwner      = Owner;
	mMembers    = Members;
	mThreadInfo = ThreadInfo;

	NullMemory(&mExInfo, sizeof(EXTRACTINFO));
	NullMemory(&mMemberInfo, sizeof(MEMBERSINFO));

	if(cmd.parse(lpszCmd,1)){
		miError = ERROR_ENOUGH_MEMORY;
		return;
	}
}

baseCCommand::~baseCCommand(){
	FreeAndNil(mszBuffer);
}

void baseCCommand::BeginThread(){
	HANDLE hThread;
	UINT uThreadID = 1;
	if(miError) return;

	hThread = (HANDLE)_beginthreadex(
		NULL, 			// default security attributes
		0, 				// use default stack size 
		CommandHost, 	// thread function
		this, 			// argument to thread function
		0, 				// use default creation flags
		&uThreadID); 	// returns the thread identifier

	SetThreadPriority(hThread, mThreadInfo.iPriority);
	WaitForSingleObject(hThread, INFINITE);
	CloseHandle(hThread);
}

UINT CALLBACK baseCCommand::CommandHost(void *p){
	CCommand *pCommand = (CCommand*)p;
	::GetCurrentDirectory(MAX_PATH,pCommand->szCurDir);
	pCommand->SetError(pCommand->Command());
	if(!pCommand->GetError())pCommand->SetError(pCommand->GetWarning());
	::SetCurrentDirectory(pCommand->szCurDir);
	return 0;
}

void baseCCommand::SetWarning(int iWarning){
	miWarning = iWarning;
}

void baseCCommand::SetError(int iErr){
	miError = iErr;
}

int baseCCommand::GetWarning(){
	return miWarning;
}

int baseCCommand::GetError(){
	return miError;
}

bool baseCCommand::ArcProc(){
	DoEvents();
	if(mThreadInfo.bBG)            Sleep(100);
	if(mThreadInfo.iPriority==-1)  Sleep(20);
	if(mThreadInfo.iPriority==-2)  Sleep(50);
	if(mThreadInfo.iPriority==-15) Sleep(100);

	switch(mOwner.iType){
		case 1: case 3:{
			BOOL ret;
			EXTRACTINGINFO egi;
			NullMemory(&egi, sizeof(EXTRACTINGINFO));
			egi.dwFileSize = (DWORD)mExInfo.llFileSize;
			egi.dwWriteSize = (DWORD)mExInfo.llWriteSize;
			strcpy(egi.szSourceFileName, mExInfo.szSourceFileName);
			strcpy(egi.szDestFileName, mExInfo.szDestFileName);
			ret = !mOwner.ArcProc ?
					SendMessage(mOwner.hWnd, mThreadInfo.uMsg, mnState, (LPARAM)&egi) :
					mOwner.ArcProc(mOwner.hWnd, mThreadInfo.uMsg, mnState, &egi);
			returnbool(ret);
		}
		case 2: case 4:{
			BOOL ret;
			EXTRACTINGINFOEX egiex;
			NullMemory(&egiex, sizeof(EXTRACTINGINFOEX));
			egiex.exinfo.dwFileSize = (DWORD)mExInfo.llFileSize;
			egiex.exinfo.dwWriteSize = (DWORD)mExInfo.llWriteSize;
			lstrcpy(egiex.exinfo.szSourceFileName, mExInfo.szSourceFileName);
			lstrcpy(egiex.exinfo.szDestFileName, mExInfo.szDestFileName);
			egiex.dwCompressedSize = (DWORD)mExInfo.llCompressedSize;
			egiex.dwCRC = mExInfo.dwCRC;
			egiex.wRatio = mExInfo.wRatio;
			FileTimeToDosTime(&mExInfo.ftWriteTime, &egiex.wDate, &egiex.wTime);
			strncpy(egiex.szMode, mExInfo.szMode, 8);
			ret = !mOwner.ArcProc ?
					SendMessage(mOwner.hWnd, mThreadInfo.uMsg, mnState, (LPARAM)&egiex) :
					mOwner.ArcProc(mOwner.hWnd, mThreadInfo.uMsg, mnState, &egiex);
			returnbool(ret);
		}
		case 5:{
			BOOL ret;
			EXTRACTINGINFOEX32 egiex32;
			NullMemory(&egiex32, sizeof(EXTRACTINGINFOEX32));
			egiex32.dwStructSize = sizeof(EXTRACTINGINFOEX32);
			egiex32.exinfo.dwFileSize = (DWORD)mExInfo.llFileSize;
			egiex32.exinfo.dwWriteSize = (DWORD)mExInfo.llWriteSize;
			strcpy(egiex32.exinfo.szSourceFileName, mExInfo.szSourceFileName);
			strcpy(egiex32.exinfo.szDestFileName, mExInfo.szDestFileName);
			egiex32.dwFileSize = (DWORD)mExInfo.llFileSize;
			egiex32.dwCompressedSize = (DWORD)mExInfo.llCompressedSize;
			egiex32.dwWriteSize = (DWORD)mExInfo.llWriteSize;
			egiex32.dwAttributes = (DWORD)mExInfo.dwAttributes;
			egiex32.dwCRC = mExInfo.dwCRC;
			egiex32.wRatio = mExInfo.wRatio;
			egiex32.ftCreateTime = mExInfo.ftCreateTime;
			egiex32.ftAccessTime = mExInfo.ftAccessTime;
			egiex32.ftWriteTime = mExInfo.ftWriteTime;
			strcpy(egiex32.szSourceFileName, mExInfo.szSourceFileName);
			strcpy(egiex32.szDestFileName, mExInfo.szDestFileName);
			ret = !mOwner.ArcProc ?
					SendMessage(mOwner.hWnd, mThreadInfo.uMsg, mnState, (LPARAM)&egiex32) :
					mOwner.ArcProc(mOwner.hWnd, mThreadInfo.uMsg, mnState, &egiex32);
			returnbool(ret);
		}
		case 6:{
			BOOL ret;
			EXTRACTINGINFOEX64 egiex64;
			NullMemory(&egiex64, sizeof(EXTRACTINGINFOEX64));
			egiex64.dwStructSize = sizeof(EXTRACTINGINFOEX64);
			egiex64.exinfo.dwFileSize = (DWORD)mExInfo.llFileSize;
			egiex64.exinfo.dwWriteSize = (DWORD)mExInfo.llWriteSize;
			lstrcpy(egiex64.exinfo.szSourceFileName, mExInfo.szSourceFileName);
			lstrcpy(egiex64.exinfo.szDestFileName, mExInfo.szDestFileName);
			egiex64.llFileSize = mExInfo.llFileSize;
			egiex64.llCompressedSize = mExInfo.llCompressedSize;
			egiex64.llWriteSize = mExInfo.llWriteSize;
			egiex64.dwAttributes = mExInfo.dwAttributes;
			egiex64.dwCRC = mExInfo.dwCRC;
			egiex64.wRatio = mExInfo.wRatio;
			egiex64.ftCreateTime = mExInfo.ftCreateTime;
			egiex64.ftAccessTime = mExInfo.ftAccessTime;
			egiex64.ftWriteTime = mExInfo.ftWriteTime;
			strcpy(egiex64.szSourceFileName, mExInfo.szSourceFileName);
			strcpy(egiex64.szDestFileName, mExInfo.szDestFileName);
			ret = !mOwner.ArcProc ?
					SendMessage(mOwner.hWnd, mThreadInfo.uMsg, mnState, (LPARAM)&egiex64) :
					mOwner.ArcProc(mOwner.hWnd, mThreadInfo.uMsg, mnState, &egiex64);
			returnbool(ret);
		}
		case 0:{
			return ArchiverProc();
		}
		default:{
			return false; //これが実行されるはずはない。
		}
	}
}

bool baseCCommand::MembProc(){
	bool ret = false;

	DoEvents();
	mbChangeCursor=false; //カーソル変更不可
	switch(mMembers.iType){
		case 1:{
			ENUM_MEMBER_INFO emi;
			NullMemory(&emi,sizeof(ENUM_MEMBER_INFO));
			emi.dwStructSize = sizeof(ENUM_MEMBER_INFO);
			emi.uCommand = mMemberInfo.uCommand;
			emi.dwOriginalSize = (DWORD)mMemberInfo.llOriginalSize;
			emi.dwCompressedSize = (DWORD)mMemberInfo.llCompressedSize;
			emi.dwAttributes = mMemberInfo.dwAttributes;
			emi.dwCRC = mMemberInfo.dwCRC;
			emi.uOSType = mMemberInfo.uOSType;
			emi.wRatio = mMemberInfo.wRatio;
			emi.ftCreateTime = mMemberInfo.ftCreateTime;
			emi.ftAccessTime = mMemberInfo.ftAccessTime;
			emi.ftWriteTime = mMemberInfo.ftWriteTime;
			lstrcpy(emi.szFileName, mMemberInfo.szFileName);
			lstrcpy(emi.szAddFileName, mMemberInfo.szAddFileName);
			if(mMembers.MembersProc(&emi))
				ret = true;
			mMemberInfo.ftCreateTime = emi.ftCreateTime;
			mMemberInfo.ftAccessTime = emi.ftAccessTime;
			mMemberInfo.ftWriteTime = emi.ftWriteTime;
			strcpy(mMemberInfo.szFileName, emi.szFileName);
			strcpy(mMemberInfo.szAddFileName, emi.szAddFileName);
		}break;
		case 2: case 3:{
			ENUM_MEMBER_INFO64 emi64;
			NullMemory(&emi64,sizeof(ENUM_MEMBER_INFO64));
			emi64.dwStructSize = sizeof(ENUM_MEMBER_INFO64);
			emi64.uCommand = mMemberInfo.uCommand;
			emi64.llOriginalSize = mMemberInfo.llOriginalSize;
			emi64.llCompressedSize = mMemberInfo.llCompressedSize;
			emi64.dwAttributes = mMemberInfo.dwAttributes;
			emi64.dwCRC = mMemberInfo.dwCRC;
			emi64.uOSType = mMemberInfo.uOSType;
			emi64.wRatio = mMemberInfo.wRatio;
			emi64.ftCreateTime = mMemberInfo.ftCreateTime;
			emi64.ftAccessTime = mMemberInfo.ftAccessTime;
			emi64.ftWriteTime = mMemberInfo.ftWriteTime;
			strcpy(emi64.szFileName, mMemberInfo.szFileName);
			strcpy(emi64.szAddFileName, mMemberInfo.szAddFileName);
			if(mMembers.MembersProc(&emi64))
				ret = true;
			mMemberInfo.ftCreateTime = emi64.ftCreateTime;
			mMemberInfo.ftAccessTime = emi64.ftAccessTime;
			mMemberInfo.ftWriteTime = emi64.ftWriteTime;
			strcpy(mMemberInfo.szFileName, emi64.szFileName);
			strcpy(mMemberInfo.szAddFileName, emi64.szAddFileName);
		}break;
		case 0:{
			ret = MembersProc();
		}break;
		default:{
			ret = false; //これが実行されるはずはない。
		}break;
	}

	mbChangeCursor=true; //カーソル変更許可
	return ret;
}

bool baseCCommand::ArchiverProc(){
	return true;
}

bool baseCCommand::MembersProc(){
	return true;
}

int baseCCommand::Command(){
	return -1;
}

LPSTR baseCCommand::LoadLangString(const WORD wStringID, LPSTR &lpszBuf){
	return ::LoadLangString(mThreadInfo.hDll, mThreadInfo.wLangID, wStringID, lpszBuf);
}
