//
// Classes.h
// The main header of HArcLib.
//
// $Id: Classes.h,v 1.7 2005/10/30 04:53:31 halpussy Exp $
//
// HArcLib
// -HAL's Lab Archiver Library-
// Copyright (C) HAL "Minthe" Pussywillow
// Under zlib/libpng license.
//

#ifndef HARCLIB_CLASSES_H
#define HARCLIB_CLASSES_H

#include "../HALsdk/HALsdkCp.h"
#include "Main.h"

LPSTR LoadLangString(const HMODULE hDll, const WORD wLangID, const WORD wStringID, LPSTR &lpszBuf);

typedef struct {
	INT64		llFileSize;				// ファイルサイズ
	INT64		llCompressedSize;		// 圧縮イメージサイズ
	INT64		llWriteSize;			// 書き込んだサイズ
	DWORD		dwAttributes;			// 属性
	DWORD		dwCRC;					// チェックサム
	UINT		uOSType;				// OSの種類
	WORD		wRatio;					// 圧縮率
	FILETIME	ftCreateTime;			// 作成時間
	FILETIME	ftAccessTime;			// 参照時間
	FILETIME	ftWriteTime;			// 更新時間
	char		szMode[32];				// 格納方法
	char		szSourceFileName[FNAME_MAX32+1];// 入力ファイル名
	char		szDestFileName[FNAME_MAX32+1];	// 出力ファイル名
} EXTRACTINFO, *LPEXTRACTINFO;

typedef struct {
	UINT		uCommand;					// 命令
	INT64		llOriginalSize;				// ファイルサイズ
	INT64		llCompressedSize;			// 圧縮イメージサイズ
	DWORD		dwAttributes;				// 属性
	DWORD		dwCRC;						// チェックサム
	UINT		uOSType;					// OSの種類
	WORD		wRatio;						// 圧縮率
	FILETIME	ftCreateTime;				// 作成時間
	FILETIME	ftAccessTime;				// 参照時間
	FILETIME	ftWriteTime;				// 更新時間
	char		szFileName[FNAME_MAX32+1];	// 入力ファイル名
	char		szAddFileName[FNAME_MAX32+1];	// 格納ファイル名
} MEMBERSINFO, *LPMEMBERSINFO;

typedef struct {
	HMODULE	hDll;
	UINT	uMsg;
	WORD	wLangID;
	int		iPriority;
	WORD	wInterval;
	BOOL	bBG;
	BOOL	bCur;
} THREADINFO, *LPTHREADINFO;

typedef struct {
	int  iType; //コールバックの形式
	HWND hWnd;  //ウィンドウ
	LPARCHIVERPROC ArcProc; //コールバック関数(iTypeが1のときは0初期化によりNULL)
} OWNERWINDOWINFO, *LPOWNERWINDOWINFO; //内部利用専用

typedef struct {
	int  iType; //コールバックの形式
	WND_ENUMMEMBPROC MembersProc; //コールバック関数(iTypeが1のときは0初期化によりNULL)
} MEMBERSPROCINFO, *LPMEMBERSPROCINFO; //内部利用専用

//Prototype
class COpenArchive;
class CCommand;
class CMain;

class baseCOpenArchive
{
public:
	baseCOpenArchive(const CMain *main, const HWND hWnd, LPCSTR lpszArc, const DWORD dwMode, LPCSTR lpszOpt=NULL);
	virtual ~baseCOpenArchive();

	//Sub Routine
	bool  IsFilter(LPSTR szFileName);
	virtual bool  SetArchiveInfo();
	virtual bool  Open();
	virtual void  SetAttribute(const DWORD dwAttribute, LPSTR szAttribute);
	virtual void  AddTotal();
	virtual void  ClearTotal();
	virtual int   FindBegin();
	virtual int   FindGet();

	//Main Routine
	int   FindFirst(LPCSTR lpszWildName, LPINDIVIDUALINFO lpSubInfo);
	int   FindNext(LPINDIVIDUALINFO lpSubInfo);
	int   FindFirstEx(LPCSTR lpszWildName, LPINDIVIDUALINFOEX lpSubInfoEx);
	int   FindNextEx(LPINDIVIDUALINFOEX lpSubInfoEx);

	virtual int   Add(LPCSTR lpszFileName, DWORD dwMode);
	virtual int   Extract(LPCSTR lpszFileName, LPCSTR lpszDirName, DWORD dwMode);
	virtual int   Move(LPCSTR lpszFileName, DWORD dwMode);
	virtual int   Delete(LPCSTR lpszFileName, DWORD dwMode);

	int   GetArcFileName(LPSTR lpszBuf, const int iSiz);
	DWORD GetArcFileSize();
	DWORD GetArcOriginalSize();
	DWORD GetArcCompressedSize();
	BOOL  GetArcFileSizeEx(INT64 *lpllsize);
	BOOL  GetArcOriginalSizeEx(INT64 *lpllsize);
	BOOL  GetArcCompressedSizeEx(INT64 *lpllsize);
	DWORD GetArcReadSize();
	BOOL  GetArcReadSizeEx(INT64 *lpllsize);
	WORD  GetArcRatio();
	WORD  GetArcDate();
	WORD  GetArcTime();
	BOOL  GetArcWriteTimeEx(FILETIME *lpfttime);
	BOOL  GetArcCreateTimeEx(FILETIME *lpfttime);
	BOOL  GetArcAccessTimeEx(FILETIME *lpfttime);
	UINT  GetArcOSType();
	int   IsSFXFile();

	int   GetFileName(LPSTR lpszBuf, const int iSiz);
	int   GetMethod(LPSTR lpszBuf, const int iSiz);
	DWORD GetOriginalSize();
	DWORD GetCompressedSize();
	BOOL  GetOriginalSizeEx(INT64 *lpllsize);
	BOOL  GetCompressedSizeEx(INT64 *lpllsize);
	WORD  GetRatio();
	WORD  GetDate();
	WORD  GetTime();
	DWORD GetWriteTime();
	DWORD GetAccessTime();
	DWORD GetCreateTime();
	BOOL  GetWriteTimeEx(FILETIME *lpfttime);
	BOOL  GetCreateTimeEx(FILETIME *lpfttime);
	BOOL  GetAccessTimeEx(FILETIME *lpfttime);
	DWORD GetCRC();
	int   GetAttribute();
	UINT  GetOSType();

protected:
	HWND     mhWnd;
	char     mszArchive[FNAME_MAX32+1];
	DWORD    mdwMode;
	LPSTR    mszOption;

	INT64    mllArcFileSize;
	INT64    mllArcOrgSize;
	INT64    mllArcCompSize;
	INT64    mllArcReadSize;
	FILETIME mftArcWrite;
	FILETIME mftArcCreate;
	FILETIME mftArcAccess;
	UINT     muOSType;
	int      miSFX;

	INDIVIDUALINFOEX mSubInfoEx;
	CmdLine  mcFilter;
	CMain    *mCMain;
};

class baseCCommand
{
public:
	baseCCommand(const CMain *main, const HWND hWnd, LPCSTR lpszCmd, LPSTR lpszOut, const DWORD dwSize,
		const OWNERWINDOWINFO Owner, const MEMBERSPROCINFO Members, const THREADINFO ThreadInfo);
	virtual ~baseCCommand();

	//Sub Routine
	void BeginThread();
	static  UINT CALLBACK CommandHost(void *p);
	void SetWarning(int iWarning);
	void SetError(int iError);
	int  GetWarning(); //内部処理用
	int  GetError();   //内部処理用
	bool ArcProc();    //内部処理用
	bool MembProc();   //内部処理用
	LPSTR LoadLangString(const WORD wStringID, LPSTR &lpszBuf);

	virtual bool ArchiverProc(); //内蔵コールバック
	virtual bool MembersProc();  //内蔵コールバック

	//Main Routine
	virtual int Command();

public:
	//これらの変数はCCommandをメンバに持つクラスから呼ばれる可能性がある
	CmdLine				cmd;
	BufWriter			bf;
	CMain				*mCMain;

	HWND				mhWnd;
	EXTRACTINFO			mExInfo;
	MEMBERSINFO			mMemberInfo;
	THREADINFO			mThreadInfo;
	UINT				mnState;
	bool				mbChangeCursor; //これがTRUEの時のみ、カーソルを変更する。

	char				szCurDir[MAX_PATH];
	LPCSTR				mlpszCmd; //コマンドを投げる必要がある場合のみ使用すること

protected:
	OWNERWINDOWINFO		mOwner;
	MEMBERSPROCINFO		mMembers;

	LPSTR				mszBuffer;

	int					miWarning;
	int					miError;
};

class baseCMain
{
public:
	baseCMain(const HMODULE hDll);
	virtual ~baseCMain();

	//Sub Routine
	virtual bool Initialize();
	virtual void Destroy();
	COpenArchive* FindObject(const HARC hArc);
	LPSTR LoadLangString(const WORD wStringID, LPSTR &lpszBuf);

	//Main Routine
	int  Command(const HWND hWnd, LPCSTR lpszCmd, LPSTR lpszBuf, const DWORD dwSize);
	WORD GetDllVersion(); //GetVersionは使えない。
	WORD GetDllSubVersion();
	WORD GetCursorInterval();
	BOOL SetCursorInterval(const WORD wInterval);
	BOOL GetBackGroundMode();
	BOOL SetBackGroundMode(const BOOL bBG);
	BOOL GetCursorMode();
	BOOL SetCursorMode(const BOOL bCur);
	BOOL GetRunning();
	virtual BOOL CheckArchive(LPCSTR lpszArc,const int iMode);
	virtual int  GetFileCount(LPCSTR lpszArc);
	virtual BOOL ConfigDialog(const HWND hWnd, LPSTR lpszBuf, const int iMode);
	virtual BOOL QueryFunctionList(const int iFunction);

	COpenArchive* OpenArchive(const HWND hWnd, LPCSTR lpszArc, const DWORD dwMode, LPCSTR lpszOpt=NULL);
	int  CloseArchive(HARC hArc);

	BOOL SetOwnerWindow(const HWND hWnd);
	BOOL ClearOwnerWindow();
	BOOL SetOwnerWindowEx(const HWND hWnd, LPARCHIVERPROC lpArcProc);
	BOOL KillOwnerWindowEx(const HWND hWnd);
	BOOL SetOwnerWindowEx64(const HWND hWnd, LPARCHIVERPROC lpArcProc, const DWORD dwStructSize);
	BOOL KillOwnerWindowEx64(const HWND hWnd);

	BOOL SetEnumMembersProc(WND_ENUMMEMBPROC lpEnumProc);
	BOOL ClearEnumMembersProc();
	BOOL SetEnumMembersProc64(WND_ENUMMEMBPROC lpEnumProc, const DWORD dwStructSize);
	BOOL ClearEnumMembersProc64();

	void SetLangueSpecified(const LANGID wLanguage);
	BOOL SetPriority(const int iPriority);

protected:
	Pointers<COpenArchive*>	mhArc;
	BOOL					mbRunning;

	WORD					mwVer;
	WORD					mwSubVer;

	int						miPriority;

	OWNERWINDOWINFO			mOwner;
	MEMBERSPROCINFO			mMembers;
	THREADINFO				mThreadInfo;

	LPSTR					mszBuffer;

	//OwnerWindows pWin;
	//MembersCallbacks pMembers;

	CMain					*mCMain; //内部利用専用
};

#include "../Classes.h"

#endif