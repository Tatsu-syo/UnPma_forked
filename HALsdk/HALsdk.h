//
// HALsdk.h
// The Header of HALsdk.
//
// $Id: HALsdk.h,v 1.13 2005/10/27 14:52:45 halpussy Exp $
//
// HALsdk
// -HAL's Lab Software Development Kit-
// <Alternative:Ekaiyu the "Unifier">
// Copyright (C) HAL "Minthe" Pussywillow
// Under zlib/libpng license.
//
// Introduction:
// I have made many DLLs, and they use the same lower loutines.
// So I decided to put the loutines together.
// This is the integration.
//

#ifndef HALSDK_H
#define HALSDK_H

#include "Macro.h"

#include <wtypes.h>
#include <stdio.h>

#ifndef min
#define min(a,b) (((a)<(b)) ? (a) : (b))
#define max(a,b) (((a)>(b)) ? (a) : (b))
#endif

EXTERNC_BEGIN

//HALsdk.c
int MakeDirectory(LPCSTR lpszDir);
	// lpszDir: The directory name which will be made.
	// ret:     The result of mkdir().

int IsMatchWildcard(LPCSTR lpszWildcard, LPCSTR lpszString);
	// lpszWildcard: The wildcard string.
	// lpszString:   The string to compare.
	// ret:          NG:0 OK:!0

int IsMatchWildcard2(LPCSTR lpszWildcard, LPCSTR lpszString, const int iMode);

bool FileExists(LPCSTR lpszFile);
	// lpszFile: The filename to check existing.
	// ret:      Exist:TRUE Not Exist:FALSE

int SFXer(LPCSTR lpszSFX, LPCSTR lpszArc, LPCSTR lpszSFXArc);
	// The file <lpszSFXArc> will be overwriiten. So please be careful.
	// lpszSFX:    The SFX name. (Not Fullpath OK)
	// lpszArc:    The Archive name for the base of lpszSFXArc.
	// lpszSFXArc: The SFXed Archive name.
	// ret:        0:OK
	//             1:String not set.
	//             2:Not found SFX.
	//             3:Could not get handle.

int IsDir(LPCSTR lpszFile);
	// lpszFile: The filename for check whether it is directory.
	// ret:      Directory:0 File:1 Error:-1
	//           If lpszFile has '?' or '*', ret is always 1.

bool GetDirectory(LPSTR lpszDir, LPCSTR lpszSource, const int iGetMode);
	// lpszDir:    The directory part. (ret)
	// lpszSource: The filename for getting directory part.
	// iGetMode:   0:All of directory part.
	//             1:First directory part.
	// ret:        OK:TRUE
	//             String not set:FALSE

bool GetFileName(LPSTR lpszFile, LPCSTR lpszSource);
	// lpszFile:   The filename part. (ret)
	// lpszSource: The filename for getting directory part.
	// ret:        OK:TRUE
	//             String not set:FALSE

bool GetShorten(LPSTR ret, LPCSTR source, LPCSTR shortener);
	// Copy source to ret with the shorter length of source or shortener.

int strcmpu(LPCSTR lpszStr1, LPCSTR lpszStr2);
	// Compare lpszStr1 and lpszStr2, Upcasing them.
	// ret: OK:0 NG:1 Error:-1

//BOOL UnWildcard()
//The C version of UnWildcardS().
//Not implemented.

WORD LastByte(LPCSTR Str);
	// HALsdk Version of DBCSReadLastByte. (?)

int GetOSType();
	// Returns a integer which refer to one Windows.
	// Used in EnumAlternativeStreams().

void NullMemory(void* buf, DWORD n);
	// Fill buf with '\0' for n.

void ErrorMsg();

bool rewrite(FILE *fp);

bool IsExecutable(LPCSTR szFileName);

bool GetFileInfo(LPCSTR szFileName, WIN32_FIND_DATA *pFind);

bool GetCompletePath(LPCTSTR FileName, DWORD PathNameSize, LPTSTR PathName);

void ReverseBytes(void *pDest, void *pIn, DWORD dwLen);

//ConvStr.c
LPWSTR GetUnicodeString(LPCSTR lpszAnsi);

LPSTR GetAnsiString(LPCWSTR lpszwUnicode);

bool MakeUnicodePath(LPCWSTR lpszwUnicode, LPWSTR lpszwPath);

//DateUtil.c
unsigned long FileTimeToUTC(const FILETIME in);

FILETIME UTCToFileTime(const DWORD UTC);

bool FileTimeToDosTime(const FILETIME *in, WORD *Date, WORD *Time);

bool IsInterCalary(const int iYear);

unsigned long DosTimeToUTC(const WORD Date, const WORD Time);

FILETIME DosTimeToFileTime(const WORD Date, const WORD Time);

//DlgCtrl.c
void DoEvents();
	// Dispatch messages of windows.

int InfinityLoop();
	// Wait for end of application.

bool CenteringWindow(const HWND hParent, const HWND hWnd);
	// hParent: The parent window.
	// hWnd:    The window handle for centering.
	// ret:     OK:TRUE, Hwnd null:FALSE

bool ResetParent(const HWND hParent, const HWND hWnd);

bool SetFont(HWND hWnd, int iHeight, DWORD dwCharSet, LPCSTR lpszFontName);

bool NotifyIcon(HWND hWnd, DWORD dwMessage, UINT uID, UINT uFlags,
				UINT uCallbackMessage, HICON hIcon, LPCSTR szTip);

void SetIcon2(HWND hWnd, HICON hIconBig, HICON hIconSmall);

void SetIcon(HWND hWnd, HICON hIcon);

void SetClassCursorRecursive(HWND hWnd, HCURSOR hCursor);

void AddStatusText(HWND hCtrl, LPCSTR szText);

void BeginCursorSubClass(HWND hWnd);

void EndCursorSubClass(HWND hWnd);

void SetSubClassCursor(HWND hWnd, HCURSOR hCursor);

int SetWhiteBackGround(HDC hDc);

bool EnableVisualTexture(HWND hWnd);

//DrvCtrl.c
bool CheckDiskSize(const char *drive,ULONGLONG size);

bool CheckFileSystem(const char *drive,ULONGLONG size);

//PathCtrl.c
bool AbbreviationPath(LPCSTR szSrc, LPSTR szDest, const int iLength);

bool ConvertAbsPathToRelPath(LPCSTR src,LPSTR dest);

bool GetRelativeDir(LPCSTR src, LPCSTR base, LPSTR dest);

bool FixPath(LPCSTR src, LPSTR dest);

bool FixPathDirect(LPSTR path);

//Crc.c
bool InitCRC16(crc16table *crc16);

USHORT UpdateCRC16(crc16table crc16, BYTE* p, DWORD dw, WORD crc);

bool InitCRC32(crc32table *crc32);

UINT UpdateCRC32(crc32table crc32, BYTE* p, DWORD dw, UINT crc);

UINT DoneCRC32(UINT crc);

//FileIO.c
INT64 GetFileSizei64(HANDLE hFile);

INT64 GetFilePointeri64(HANDLE hFile);

bool SetFilePointeri64(HANDLE hFile, INT64 llPointer);

EXTERNC_END

#endif //HALsdk.h included.