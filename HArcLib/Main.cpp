//
// Main.cpp
// APIの実装
//
// $Id: Main.cpp,v 1.3 2005/10/30 04:19:46 halpussy Exp $
//

#include "Classes.h"

CMain *mCMain; //唯一の外部オブジェクト

BOOL APIENTRY DllMain(HMODULE hDll, DWORD dwReason, LPVOID lpRes){
	switch(dwReason){
		case DLL_PROCESS_ATTACH:{
			mCMain = new CMain(hDll);
			if(!mCMain)
				return FALSE;
			if(!mCMain->Initialize()){
				delete mCMain;
				return FALSE;
			}
		}break;
		case DLL_PROCESS_DETACH:{
			mCMain->Destroy();
			delete mCMain;
		}break;
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		default:
		break;
	}
	return TRUE;
}

//LHA.dll Compatible
int WINAPI API_PREFIX(const HWND _hwnd, LPCSTR _szCmdLine, LPSTR _szOutput, const DWORD _dwSize){
	return mCMain->Command(_hwnd, _szCmdLine, _szOutput, _dwSize);
}

void CALLBACK API_NAME(Run)(const HWND _hwnd, const HINSTANCE _hInst, LPCSTR _lpszCmdLine, const int _nCmdShow){
	API_PREFIX(NULL,_lpszCmdLine,NULL,0);
}

void CALLBACK API_NAME(CommandLine)(const HWND _hwnd, const HINSTANCE _hInst, LPCSTR _lpszCmdLine, const int _nCmdShow){
	API_PREFIX(NULL,_lpszCmdLine,NULL,0);
}

WORD WINAPI API_NAME(GetVersion)(){
	return mCMain->GetDllVersion();
}

WORD WINAPI API_NAME(GetSubVersion)(){
	return mCMain->GetDllSubVersion();
}

WORD WINAPI API_NAME(GetBuild)(){
	return BUILD;
}

BOOL WINAPI API_NAME(GetRunning)(){
	return mCMain->GetRunning();
}

BOOL WINAPI API_NAME(GetBackGroundMode)(){
	return mCMain->GetBackGroundMode();
}

BOOL WINAPI API_NAME(SetBackGroundMode)(const BOOL _BackGroundMode){
	return mCMain->SetBackGroundMode(_BackGroundMode);
}

BOOL WINAPI API_NAME(GetCursorMode)(){
	return mCMain->GetCursorMode();
}

BOOL WINAPI API_NAME(SetCursorMode)(const BOOL _CursorMode){
	return mCMain->SetCursorMode(_CursorMode);
}

WORD WINAPI API_NAME(GetCursorInterval)(){
	return mCMain->GetCursorInterval();
}

BOOL WINAPI API_NAME(SetCursorInterval)(const WORD _Interval){
	return mCMain->SetCursorInterval(_Interval);
}

BOOL WINAPI API_NAME(CheckArchive)(LPCSTR _szFileName, const int _iMode){
	return mCMain->CheckArchive(_szFileName,_iMode);
}

int WINAPI API_NAME(GetFileCount)(LPCSTR _szArcFile){
	return mCMain->GetFileCount(_szArcFile);
}

BOOL WINAPI API_NAME(ConfigDialog)(const HWND _hwnd, LPSTR _lpszComBuffer, const int _iMode){
	return mCMain->ConfigDialog(_hwnd, _lpszComBuffer, _iMode);
}

BOOL WINAPI API_NAME(QueryFunctionList)(const int _iFunction){
	return mCMain->QueryFunctionList(_iFunction);
}

//OpenArchive
HARC WINAPI API_NAME(OpenArchive)(const HWND _hwnd, LPCSTR _szFileName, const DWORD _dwMode){
	return mCMain->OpenArchive(_hwnd,_szFileName,_dwMode);
}

HARC WINAPI API_NAME(OpenArchive2)(const HWND _hwnd, LPCSTR _szFileName, const DWORD _dwMode, LPCSTR _szOption){
	return mCMain->OpenArchive(_hwnd,_szFileName,_dwMode,_szOption);
}

int WINAPI API_NAME(CloseArchive)(HARC _harc){
	return mCMain->CloseArchive(_harc);
}

int WINAPI API_NAME(FindFirst)(HARC _harc, LPCSTR _szWildName, INDIVIDUALINFO *_lpSubInfo){
	COpenArchive *pArc = mCMain->FindObject(_harc);
	return pArc ? pArc->FindFirst(_szWildName, _lpSubInfo) : ERROR_HARC_ISNOT_OPENED;
}

int WINAPI API_NAME(FindNext)(HARC _harc, INDIVIDUALINFO *_lpSubInfo){
	COpenArchive *pArc = mCMain->FindObject(_harc);
	return pArc ? pArc->FindNext(_lpSubInfo) : ERROR_HARC_ISNOT_OPENED;
}

int WINAPI API_NAME(FindFirstEx)(HARC _harc, LPCSTR _szWildName, INDIVIDUALINFOEX *_lpSubInfoEx){
	COpenArchive *pArc = mCMain->FindObject(_harc);
	return pArc ? pArc->FindFirstEx(_szWildName, _lpSubInfoEx) : ERROR_HARC_ISNOT_OPENED;
}

int WINAPI API_NAME(FindNextEx)(HARC _harc, INDIVIDUALINFOEX *_lpSubInfoEx){
	COpenArchive *pArc = mCMain->FindObject(_harc);
	return pArc ? pArc->FindNextEx(_lpSubInfoEx) : ERROR_HARC_ISNOT_OPENED;
}

int WINAPI API_NAME(GetArcFileName)(HARC _harc, LPSTR _lpBuffer, const int _nSize){
	COpenArchive *pArc = mCMain->FindObject(_harc);
	return pArc ? pArc->GetArcFileName(_lpBuffer, _nSize) : ERROR_HARC_ISNOT_OPENED;
}

DWORD WINAPI API_NAME(GetArcFileSize)(HARC _harc){
	COpenArchive *pArc = mCMain->FindObject(_harc);
	return pArc ? pArc->GetArcFileSize() : -1;
}

DWORD WINAPI API_NAME(GetArcOriginalSize)(HARC _harc){
	COpenArchive *pArc = mCMain->FindObject(_harc);
	return pArc ? pArc->GetArcOriginalSize() : -1;
}

DWORD WINAPI API_NAME(GetArcCompressedSize)(HARC _harc){
	COpenArchive *pArc = mCMain->FindObject(_harc);
	return pArc ? pArc->GetArcCompressedSize() : -1;
}

DWORD WINAPI API_NAME(GetArcReadSize)(HARC _harc){
	COpenArchive *pArc = mCMain->FindObject(_harc);
	return pArc ? pArc->GetArcReadSize() : -1;
}

BOOL WINAPI API_NAME(GetArcFileSizeEx)(HARC _harc, INT64 *_lpllsize){
	COpenArchive *pArc = mCMain->FindObject(_harc);
	return pArc ? pArc->GetArcFileSizeEx(_lpllsize) : FALSE;
}

BOOL WINAPI API_NAME(GetArcOriginalSizeEx)(HARC _harc, INT64 *_lpllsize){
	COpenArchive *pArc = mCMain->FindObject(_harc);
	return pArc ? pArc->GetArcOriginalSizeEx(_lpllsize) : FALSE;
}

BOOL WINAPI API_NAME(GetArcCompressedSizeEx)(HARC _harc, INT64 *_lpllsize){
	COpenArchive *pArc = mCMain->FindObject(_harc);
	return pArc ? pArc->GetArcCompressedSizeEx(_lpllsize) : FALSE;
}

BOOL WINAPI API_NAME(GetArcReadSizeEx)(HARC _harc, INT64 *_lpllsize){
	COpenArchive *pArc = mCMain->FindObject(_harc);
	return pArc ? pArc->GetArcReadSizeEx(_lpllsize) : FALSE;
}

WORD WINAPI API_NAME(GetArcRatio)(HARC _harc){
	COpenArchive *pArc = mCMain->FindObject(_harc);
	return pArc ? pArc->GetArcRatio() : FALSE;
}

WORD WINAPI API_NAME(GetArcDate)(HARC _harc){
	COpenArchive *pArc = mCMain->FindObject(_harc);
	return pArc ? pArc->GetArcDate() : -1;
}

WORD WINAPI API_NAME(GetArcTime)(HARC _harc){
	COpenArchive *pArc = mCMain->FindObject(_harc);
	return pArc ? pArc->GetArcTime() : -1;
}

BOOL WINAPI API_NAME(GetArcWriteTimeEx)(HARC _harc, FILETIME *_lpftLastWriteTime){
	COpenArchive *pArc = mCMain->FindObject(_harc);
	return pArc ? pArc->GetArcWriteTimeEx(_lpftLastWriteTime) : FALSE;
}

BOOL WINAPI API_NAME(GetArcCreateTimeEx)(HARC _harc, FILETIME *_lpftCreationTime){
	COpenArchive *pArc = mCMain->FindObject(_harc);
	return pArc ? pArc->GetArcCreateTimeEx(_lpftCreationTime) : FALSE;
}

BOOL WINAPI API_NAME(GetArcAccessTimeEx)(HARC _harc, FILETIME *_lpftLastAccessTime){
	COpenArchive *pArc = mCMain->FindObject(_harc);
	return pArc ? pArc->GetArcAccessTimeEx(_lpftLastAccessTime) : FALSE;
}

UINT WINAPI API_NAME(GetArcOSType)(HARC _harc){
	COpenArchive *pArc = mCMain->FindObject(_harc);
	return pArc ? pArc->GetArcOSType() : -1;
}

int WINAPI API_NAME(IsSFXFile)(HARC _harc){
	COpenArchive *pArc = mCMain->FindObject(_harc);
	return pArc ? pArc->IsSFXFile() : -1;
}

int WINAPI API_NAME(GetFileName)(HARC _harc, LPSTR _lpBuffer, const int _nSize){
	COpenArchive *pArc = mCMain->FindObject(_harc);
	return pArc ? pArc->GetFileName(_lpBuffer, _nSize) : -1;
}

int WINAPI API_NAME(GetMethod)(HARC _harc, LPSTR _lpBuffer, const int _nSize){
	COpenArchive *pArc = mCMain->FindObject(_harc);
	return pArc ? pArc->GetMethod(_lpBuffer, _nSize) : -1;
}

DWORD WINAPI API_NAME(GetOriginalSize)(HARC _harc){
	COpenArchive *pArc = mCMain->FindObject(_harc);
	return pArc ? pArc->GetOriginalSize() : -1;
}

DWORD WINAPI API_NAME(GetCompressedSize)(HARC _harc){
	COpenArchive *pArc = mCMain->FindObject(_harc);
	return pArc ? pArc->GetCompressedSize() : -1;
}

BOOL WINAPI API_NAME(GetOriginalSizeEx)(HARC _harc, INT64 *_lpllSize){
	COpenArchive *pArc = mCMain->FindObject(_harc);
	return pArc ? pArc->GetOriginalSizeEx(_lpllSize) : FALSE;
}

BOOL WINAPI API_NAME(GetCompressedSizeEx)(HARC _harc, INT64 *_lpllSize){
	COpenArchive *pArc = mCMain->FindObject(_harc);
	return pArc ? pArc->GetCompressedSizeEx(_lpllSize) : FALSE;
}

WORD WINAPI API_NAME(GetRatio)(HARC _harc){
	COpenArchive *pArc = mCMain->FindObject(_harc);
	return pArc ? pArc->GetRatio() : -1;
}

WORD WINAPI API_NAME(GetDate)(HARC _harc){
	COpenArchive *pArc = mCMain->FindObject(_harc);
	return pArc ? pArc->GetDate() : -1;
}

WORD WINAPI API_NAME(GetTime)(HARC _harc){
	COpenArchive *pArc = mCMain->FindObject(_harc);
	return pArc ? pArc->GetTime() : -1;
}

DWORD WINAPI API_NAME(GetWriteTime)(HARC _harc){
	COpenArchive *pArc = mCMain->FindObject(_harc);
	return pArc ? pArc->GetWriteTime() : -1;
}

DWORD WINAPI API_NAME(GetCreateTime)(HARC _harc){
	COpenArchive *pArc = mCMain->FindObject(_harc);
	return pArc ? pArc->GetCreateTime() : -1;
}

DWORD WINAPI API_NAME(GetAccessTime)(HARC _harc){
	COpenArchive *pArc = mCMain->FindObject(_harc);
	return pArc ? pArc->GetAccessTime() : -1;
}

BOOL WINAPI API_NAME(GetWriteTimeEx)(HARC _harc, FILETIME *_lpftLastWriteTime){
	COpenArchive *pArc = mCMain->FindObject(_harc);
	return pArc ? pArc->GetWriteTimeEx(_lpftLastWriteTime) : FALSE;
}

BOOL WINAPI API_NAME(GetCreateTimeEx)(HARC _harc, FILETIME *_lpftCreationTime){
	COpenArchive *pArc = mCMain->FindObject(_harc);
	return pArc ? pArc->GetCreateTimeEx(_lpftCreationTime) : FALSE;
}

BOOL WINAPI API_NAME(GetAccessTimeEx)(HARC _harc, FILETIME *_lpftLastAccessTime){
	COpenArchive *pArc = mCMain->FindObject(_harc);
	return pArc ? pArc->GetAccessTimeEx(_lpftLastAccessTime) : FALSE;
}

int WINAPI API_NAME(GetAttribute)(HARC _harc){
	COpenArchive *pArc = mCMain->FindObject(_harc);
	return pArc ? pArc->GetAttribute() : -1;
}

DWORD WINAPI API_NAME(GetCRC)(HARC _harc){
	COpenArchive *pArc = mCMain->FindObject(_harc);
	return pArc ? pArc->GetCRC() : -1;
}

UINT WINAPI API_NAME(GetOSType)(HARC _harc){
	COpenArchive *pArc = mCMain->FindObject(_harc);
	return pArc ? pArc->GetOSType() : -1;
}

int WINAPI API_NAME(Add)(HARC _harc, LPCSTR _szFileName, DWORD _dwMode){
	COpenArchive *pArc = mCMain->FindObject(_harc);
	return pArc ? pArc->Add(_szFileName, _dwMode) : ERROR_HARC_ISNOT_OPENED;
}

int WINAPI API_NAME(Extract)(HARC _harc, LPCSTR _szFileName, LPCSTR _szDirName, DWORD _dwMode){
	COpenArchive *pArc = mCMain->FindObject(_harc);
	return pArc ? pArc->Extract(_szFileName, _szDirName, _dwMode) : ERROR_HARC_ISNOT_OPENED;
}

int WINAPI API_NAME(Move)(HARC _harc, LPCSTR _szFileName, DWORD _dwMode){
	COpenArchive *pArc = mCMain->FindObject(_harc);
	return pArc ? pArc->Move(_szFileName, _dwMode) : ERROR_HARC_ISNOT_OPENED;
}

int WINAPI API_NAME(Delete)(HARC _harc, LPCSTR _szFileName, DWORD _dwMode){
	COpenArchive *pArc = mCMain->FindObject(_harc);
	return pArc ? pArc->Delete(_szFileName, _dwMode) : ERROR_HARC_ISNOT_OPENED;
}

int WINAPI API_NAME(ExtractMem)(const HWND _hwndParent, LPCSTR _szCmdLine, LPBYTE _lpBuffer, const DWORD _dwSize, /*time_t*/DWORD *_lpTime, LPWORD _lpwAttr, LPDWORD _lpdwWriteSize){
	return -1;
}

int WINAPI API_NAME(CompressMem)(const HWND _hwndParent, LPCSTR _szCmdLine, const LPBYTE _lpBuffer, const DWORD _dwSize, const /*time_t*/DWORD *_lpTime, const LPWORD _lpwAttr, LPDWORD _lpdwWriteSize){
	return -1;
}

//SetOwnerWindow
BOOL WINAPI API_NAME(SetOwnerWindow)(const HWND _hwnd){
	return mCMain->SetOwnerWindow(_hwnd);
}

BOOL WINAPI API_NAME(ClearOwnerWindow)(){
	return mCMain->ClearOwnerWindow();
}

BOOL WINAPI API_NAME(SetOwnerWindowEx)(const HWND _hwnd, LPARCHIVERPROC _lpArcProc){
	return mCMain->SetOwnerWindowEx(_hwnd, _lpArcProc);
}

BOOL WINAPI API_NAME(KillOwnerWindowEx)(const HWND _hwnd){
	return mCMain->KillOwnerWindowEx(_hwnd);
}

BOOL WINAPI API_NAME(SetOwnerWindowEx64)(const HWND _hwnd, LPARCHIVERPROC _lpArcProc, DWORD _dwStructSize){
	return mCMain->SetOwnerWindowEx64(_hwnd, _lpArcProc, _dwStructSize);
}

BOOL WINAPI API_NAME(KillOwnerWindowEx64)(const HWND _hwnd){
	return mCMain->KillOwnerWindowEx64(_hwnd);
}

//SetEnumMembersProc
BOOL WINAPI API_NAME(SetEnumMembersProc)(WND_ENUMMEMBPROC _lpEnumProc){
	return mCMain->SetEnumMembersProc(_lpEnumProc);
}

BOOL WINAPI API_NAME(ClearEnumMembersProc)(){
	return mCMain->ClearEnumMembersProc();
}

BOOL WINAPI API_NAME(SetEnumMembersProc64)(WND_ENUMMEMBPROC _lpEnumProc, DWORD _dwStructSize){
	return mCMain->SetEnumMembersProc64(_lpEnumProc, _dwStructSize);
}

BOOL WINAPI API_NAME(ClearEnumMembersProc64)(){
	return mCMain->ClearEnumMembersProc64();
}

//Others
void WINAPI SetLangueSpecified(LANGID _wLanguage){
	mCMain->SetLangueSpecified(_wLanguage);
}
void WINAPI SetLangueJapanese(){
	SetLangueSpecified(MAKELANGID(LANG_JAPANESE, SUBLANG_DEFAULT));
}
void WINAPI SetLangueEnglish(){
	SetLangueSpecified(MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
}
void WINAPI API_NAME(SetLangueSpecified)(LANGID _wLanguage){
	SetLangueSpecified(_wLanguage);
}

void WINAPI API_NAME(SetLangueJapanese)(){
	SetLangueJapanese();
}
void WINAPI API_NAME(SetLangueEnglish)(){
	SetLangueEnglish();
}

BOOL WINAPI API_NAME(SetPriority)(int _nPriority){
	return mCMain->SetPriority(_nPriority);
}