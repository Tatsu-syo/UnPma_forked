//
// Main.h
// Main header file.
//
// $Id: Main.h,v 1.4 2005/10/30 04:19:46 halpussy Exp $
//

#ifndef HARCLIB_MAIN_H
#define HARCLIB_MAIN_H

#include "CommArc.h"

//対応アプリはこの行をConfig.hの#define API_PREFIX/API_NAMEの行と
//置換し、DLL固有値を定義しなおしヘッダファイルとしてください。
#include "../Config.h"

EXTERNC_BEGIN

WORD  WINAPI API_NAME(GetVersion)();
BOOL  WINAPI API_NAME(GetRunning)();
BOOL  WINAPI API_NAME(GetBackGroundMode)();
BOOL  WINAPI API_NAME(SetBackGroundMode)(const BOOL _BackGroundMode);
BOOL  WINAPI API_NAME(GetCursorMode)();
BOOL  WINAPI API_NAME(SetCursorMode)(const BOOL _CursorMode);
WORD  WINAPI API_NAME(GetCursorInterval)();
BOOL  WINAPI API_NAME(SetCursorInterval)(const WORD _Interval);
int   WINAPI API_PREFIX(const HWND _hwnd, LPCSTR _szCmdLine, LPSTR _szOutput, const DWORD _dwSize);

BOOL  WINAPI API_NAME(CheckArchive)(LPCSTR _szFileName, const int _iMode);
int   WINAPI API_NAME(GetFileCount)(LPCSTR _szArcFile);
BOOL  WINAPI API_NAME(ConfigDialog)(const HWND _hwnd, LPSTR _lpszComBuffer, const int _iMode);
BOOL  WINAPI API_NAME(QueryFunctionList)(const int _iFunction);

HARC  WINAPI API_NAME(OpenArchive)(const HWND _hwnd, LPCSTR _szFileName, const DWORD _dwMode);
HARC  WINAPI API_NAME(OpenArchive2)(const HWND _hwnd, LPCSTR _szFileName, const DWORD _dwMode, LPCSTR _szOption);
int   WINAPI API_NAME(CloseArchive)(HARC _harc);
int   WINAPI API_NAME(FindFirst)(HARC _harc, LPCSTR _szWildName, LPINDIVIDUALINFO _lpSubInfo);
int   WINAPI API_NAME(FindNext)(HARC _harc, LPINDIVIDUALINFO _lpSubInfo);
int   WINAPI API_NAME(GetArcFileName)(HARC _harc, LPSTR _lpBuffer, const int _nSize);
DWORD WINAPI API_NAME(GetArcFileSize)(HARC _harc);
DWORD WINAPI API_NAME(GetArcOriginalSize)(HARC _harc);
DWORD WINAPI API_NAME(GetArcCompressedSize)(HARC _harc);
BOOL  WINAPI API_NAME(GetArcFileSizeEx)(HARC _harc, INT64 *_lpllSize);
BOOL  WINAPI API_NAME(GetArcOriginalSizeEx)(HARC _harc, INT64 *_lpllSize);
BOOL  WINAPI API_NAME(GetArcCompressedSizeEx)(HARC _harc, INT64 *_lpllSize);
DWORD WINAPI API_NAME(GetArcReadSize)(HARC _harc);
BOOL  WINAPI API_NAME(GetArcReadSizeEx)(HARC _harc, INT64 *_lpllSize);
WORD  WINAPI API_NAME(GetArcRatio)(HARC _harc);
WORD  WINAPI API_NAME(GetArcDate)(HARC _harc);
WORD  WINAPI API_NAME(GetArcTime)(HARC _harc);
BOOL  WINAPI API_NAME(GetArcWriteTimeEx)(HARC _harc, FILETIME *_lpftLastWriteTime);
BOOL  WINAPI API_NAME(GetArcCreateTimeEx)(HARC _harc, FILETIME *_lpftCreationTime);
BOOL  WINAPI API_NAME(GetArcAccessTimeEx)(HARC _harc, FILETIME *_lpftLastAccessTime);
UINT  WINAPI API_NAME(GetArcOSType)(HARC _harc);
int   WINAPI API_NAME(IsSFXFile)(HARC _harc);
int   WINAPI API_NAME(GetFileName)(HARC _harc, LPSTR _lpBuffer, const int _nSize);
int   WINAPI API_NAME(GetMethod)(HARC _harc, LPSTR _lpBuffer, const int _nSize);
DWORD WINAPI API_NAME(GetOriginalSize)(HARC _harc);
DWORD WINAPI API_NAME(GetCompressedSize)(HARC _harc);
BOOL  WINAPI API_NAME(GetOriginalSizeEx)(HARC _harc, INT64 *_lpllSize);
BOOL  WINAPI API_NAME(GetCompressedSizeEx)(HARC _harc, INT64 *_lpllSize);
WORD  WINAPI API_NAME(GetRatio)(HARC _harc);
WORD  WINAPI API_NAME(GetDate)(HARC _harc);
WORD  WINAPI API_NAME(GetTime)(HARC _harc);
DWORD WINAPI API_NAME(GetWriteTime)(HARC _harc);
DWORD WINAPI API_NAME(GetAccessTime)(HARC _harc);
DWORD WINAPI API_NAME(GetCreateTime)(HARC _harc);
BOOL  WINAPI API_NAME(GetWriteTimeEx)(HARC _harc, FILETIME *_lpftLastWriteTime);
BOOL  WINAPI API_NAME(GetCreateTimeEx)(HARC _harc, FILETIME *_lpftCreationTime);
BOOL  WINAPI API_NAME(GetAccessTimeEx)(HARC _harc, FILETIME *_lpftLastAccessTime);
DWORD WINAPI API_NAME(GetCRC)(HARC _harc);
int   WINAPI API_NAME(GetAttribute)(HARC _harc);
UINT  WINAPI API_NAME(GetOSType)(HARC _harc);

int   WINAPI API_NAME(Add)(HARC _harc, LPCSTR _szFileName, DWORD _dwMode);
int   WINAPI API_NAME(Extract)(HARC _harc, LPCSTR _szFileName, LPCSTR _szDirName, DWORD _dwMode);
int   WINAPI API_NAME(Move)(HARC _harc, LPCSTR _szFileName, DWORD _dwMode);
int   WINAPI API_NAME(Delete)(HARC _harc, LPCSTR _szFileName, DWORD _dwMode);

WORD  WINAPI API_NAME(GetSubVersion)();
void  CALLBACK API_NAME(Run)(const HWND _hwnd, const HINSTANCE _hInst, LPCSTR _lpszCmdLine, const int _nCmdShow);
WORD  WINAPI API_NAME(GetBuild)();
void  CALLBACK API_NAME(CommandLine)(const HWND _hwnd, const HINSTANCE _hInst, LPCSTR _lpszCmdLine, const int _nCmdShow);

int   WINAPI API_NAME(ExtractMem)(const HWND _hwndParent, LPCSTR _szCmdLine, LPBYTE _lpBuffer, const DWORD _dwSize, _TIME_T *_lpTime, LPWORD _lpwAttr, LPDWORD _lpdwWriteSize);
int   WINAPI API_NAME(CompressMem)(const HWND _hwndParent, LPCSTR _szCmdLine, const LPBYTE _lpBuffer, const DWORD _dwSize, const _TIME_T *_lpTime, const LPWORD _lpwAttr, LPDWORD _lpdwWriteSize);

BOOL  WINAPI API_NAME(SetOwnerWindow)(const HWND _hwnd);
BOOL  WINAPI API_NAME(ClearOwnerWindow)();
BOOL  WINAPI API_NAME(SetOwnerWindowEx)(const HWND _hwnd, LPARCHIVERPROC _lpArcProc);
BOOL  WINAPI API_NAME(KillOwnerWindowEx)(const HWND _hwnd);
BOOL  WINAPI API_NAME(SetOwnerWindowEx64)(const HWND _hwnd, LPARCHIVERPROC _lpArcProc, DWORD _dwStructSize);
BOOL  WINAPI API_NAME(KillOwnerWindowEx64)(const HWND _hwnd);

BOOL  WINAPI API_NAME(SetEnumMembersProc)(WND_ENUMMEMBPROC _lpEnumProc);
BOOL  WINAPI API_NAME(ClearEnumMembersProc)();
BOOL  WINAPI API_NAME(SetEnumMembersProc64)(WND_ENUMMEMBPROC _lpEnumProc, DWORD _dwStructSize);
BOOL  WINAPI API_NAME(ClearEnumMembersProc64)();

void  WINAPI SetLangueSpecified(LANGID _wLanguage);
void  WINAPI SetLangueJapanese();
void  WINAPI SetLangueEnglish();
void  WINAPI API_NAME(SetLangueSpecified)(LANGID _wLanguage);
void  WINAPI API_NAME(SetLangueJapanese)();
void  WINAPI API_NAME(SetLangueEnglish)();
BOOL  WINAPI API_NAME(SetPriority)(int _nPriority);

//以下HArcLibオリジナルAPI
int   WINAPI API_NAME(FindFirstEx)(HARC _harc, LPCSTR _szWildName, LPINDIVIDUALINFOEX _lpSubInfoEx);
int   WINAPI API_NAME(FindNextEx)(HARC _harc, LPINDIVIDUALINFOEX _lpSubInfoEx);

//BOOL  WINAPI API_NAME(SetEnumMembersCallback)(const HWND _hwnd, WND_ENUMMEMBPROCEX _lpEnumProcEx);
//BOOL  WINAPI API_NAME(ClearEnumMembersCallback)(const HWND _hwnd);
//BOOL  WINAPI API_NAME(SetEnumMembersCallbackEx)(const HWND _hwnd, WND_ENUMMEMBPROCEX _lpEnumProcEx);
//BOOL  WINAPI API_NAME(ClearEnumMembersCallbackEx)(const HWND _hwnd);
//BOOL  WINAPI API_NAME(SetEnumMembersCallback64)(const HWND _hwnd, WND_ENUMMEMBPROCEX _lpEnumProcEx);
//BOOL  WINAPI API_NAME(ClearEnumMembersCallback64)(const HWND _hwnd);

EXTERNC_END

#endif