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
	INT64		llFileSize;				// �t�@�C���T�C�Y
	INT64		llCompressedSize;		// ���k�C���[�W�T�C�Y
	INT64		llWriteSize;			// �������񂾃T�C�Y
	DWORD		dwAttributes;			// ����
	DWORD		dwCRC;					// �`�F�b�N�T��
	UINT		uOSType;				// OS�̎��
	WORD		wRatio;					// ���k��
	FILETIME	ftCreateTime;			// �쐬����
	FILETIME	ftAccessTime;			// �Q�Ǝ���
	FILETIME	ftWriteTime;			// �X�V����
	char		szMode[32];				// �i�[���@
	char		szSourceFileName[FNAME_MAX32+1];// ���̓t�@�C����
	char		szDestFileName[FNAME_MAX32+1];	// �o�̓t�@�C����
} EXTRACTINFO, *LPEXTRACTINFO;

typedef struct {
	UINT		uCommand;					// ����
	INT64		llOriginalSize;				// �t�@�C���T�C�Y
	INT64		llCompressedSize;			// ���k�C���[�W�T�C�Y
	DWORD		dwAttributes;				// ����
	DWORD		dwCRC;						// �`�F�b�N�T��
	UINT		uOSType;					// OS�̎��
	WORD		wRatio;						// ���k��
	FILETIME	ftCreateTime;				// �쐬����
	FILETIME	ftAccessTime;				// �Q�Ǝ���
	FILETIME	ftWriteTime;				// �X�V����
	char		szFileName[FNAME_MAX32+1];	// ���̓t�@�C����
	char		szAddFileName[FNAME_MAX32+1];	// �i�[�t�@�C����
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
	int  iType; //�R�[���o�b�N�̌`��
	HWND hWnd;  //�E�B���h�E
	LPARCHIVERPROC ArcProc; //�R�[���o�b�N�֐�(iType��1�̂Ƃ���0�������ɂ��NULL)
} OWNERWINDOWINFO, *LPOWNERWINDOWINFO; //�������p��p

typedef struct {
	int  iType; //�R�[���o�b�N�̌`��
	WND_ENUMMEMBPROC MembersProc; //�R�[���o�b�N�֐�(iType��1�̂Ƃ���0�������ɂ��NULL)
} MEMBERSPROCINFO, *LPMEMBERSPROCINFO; //�������p��p

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
	int  GetWarning(); //���������p
	int  GetError();   //���������p
	bool ArcProc();    //���������p
	bool MembProc();   //���������p
	LPSTR LoadLangString(const WORD wStringID, LPSTR &lpszBuf);

	virtual bool ArchiverProc(); //�����R�[���o�b�N
	virtual bool MembersProc();  //�����R�[���o�b�N

	//Main Routine
	virtual int Command();

public:
	//�����̕ϐ���CCommand�������o�Ɏ��N���X����Ă΂��\��������
	CmdLine				cmd;
	BufWriter			bf;
	CMain				*mCMain;

	HWND				mhWnd;
	EXTRACTINFO			mExInfo;
	MEMBERSINFO			mMemberInfo;
	THREADINFO			mThreadInfo;
	UINT				mnState;
	bool				mbChangeCursor; //���ꂪTRUE�̎��̂݁A�J�[�\����ύX����B

	char				szCurDir[MAX_PATH];
	LPCSTR				mlpszCmd; //�R�}���h�𓊂���K�v������ꍇ�̂ݎg�p���邱��

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
	WORD GetDllVersion(); //GetVersion�͎g���Ȃ��B
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

	CMain					*mCMain; //�������p��p
};

#include "../Classes.h"

#endif