//
// Classes.h
// DLL�̃N���X�̐錾
//
// $Id: Classes.h,v 1.2 2005/09/18 05:46:11 halpussy Exp $
//

#ifndef CLASSES_H
#define CLASSES_H

#include "HArcLib/Classes.h"
#include "HALsdk/HALsdkCp.h"

EXTERNC int WINAPI UnPmaGetMemo(HARC _harc,LPSTR _lpBuffer,const int _nSize);

#include <commctrl.h>
//#include <stdio.h>
#include "UnPma/CPmaStream.h"
#include "resource.h"

class COpenArchive : public baseCOpenArchive
{
public:
	COpenArchive(const CMain *main, const HWND hWnd, LPCSTR lpszArc, const DWORD dwMode, LPCSTR lpszOpt=NULL);
	virtual ~COpenArchive();

	//SetAttribute�̓I�[�o�[���C�h����K�v�͂���܂���B
	//GetArcRatio/GetRatio�́A���k���̃t�H�[�}�b�g��ς������ꍇ�̂݃I�[�o�[���C�h���Ă��������B

	//Sub Routine
	bool  Open();
	void  SetAttribute(const DWORD dwAttribute, LPSTR szAttribute);
	int   FindBegin();
	int   FindGet();

	//Main Routine
	//WORD  GetArcRatio();
	//WORD  GetRatio();

	PmaStream UnPma; // for UnPmaGetMemo()

protected:

protected:
	//�ϐ�

	//��`�ςݕϐ�
	//mhWnd(HWND) �e�E�B���h�E
	//mszArchive(char [FNAME_MAX32+1]) ���ɖ�
	//mdwMode(DWORD) �I�[�v�����[�h
	//mszOption(LPSTR) �I�v�V����
	//mszWildName(LPSTR) ���C���h�J�[�h
	//mllArcReadSize(INT64) �ǂݍ��񂾃T�C�Y
	//muOSType(UINT) ���ɂ�OS�^�C�v
	//miSFX(int) SFX�`��
	//mSubInfoEx(INDIVIDUALINFOEX�\����)
};

class CCommand : public baseCCommand
{
public:
	CCommand(const CMain *main, const HWND hWnd, LPCSTR lpszCmd, LPSTR lpszOut, const DWORD dwSize,
		const OWNERWINDOWINFO Owner, const MEMBERSPROCINFO Members, const THREADINFO ThreadInfo);

	//Command����̌Ăяo���͂��Ȃ炸ArcProc/MembProc�ł��肢���܂��B
	//ArchiverProc/MembersProc���Ăяo���ƁA
	//SetOwnerWindow/SetEnumMembersProc�n�����������삵�Ȃ��Ȃ�܂��B

	//�t�ɁA�����ł̂ݏ������������ꍇ�ɂ�ArchiverProc/MembersProc���Ăяo���̂��ǂ��ł��傤�B

	//Sub Routine
	bool ArchiverProc(); //�����R�[���o�b�N
	bool MembersProc();  //�����R�[���o�b�N

	BOOL ConfirmProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	BOOL ProgressProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	//Main Routine
	int Command();

public:
	WORD wConfirm;
	bool showdlg;

	//��`�ςݕϐ�
	//cmd(CmdLine�N���X) ��͍ς݃R�}���h���C��
	//bf(BufWriter�N���X) �������ݗp�o�b�t�@
	//mExInfo(EXTRACTINFO�\����)
	//mMemberInfo(MEMBERSINFO�\����)
	//mnState(UINT) ArcProc�Ŏg�p
	//mwLangID(LANGID) ���ꃊ�\�[�X�擾�Ɏg�p

protected:
	//�ϐ�
	HWND hProgress;

	//��`�ςݕϐ�
	//mhWnd(HWND) �e�E�B���h�E
	//mThreadInfo(THREADINFO)
};

class CMain : public baseCMain
{
public:
	CMain(const HMODULE hDll);

	//Initialize/Destroy/GetFileCount�̓I�[�o�[���C�h����K�v�͂���܂���B
	//GetFileCount�́A���ɂȂ̂�OpenArchive�ł��Ȃ��\��������
	//(�w�b�_���Í�������Ă���Ȃ�)�ꍇ�ɃI�[�o�[���C�h���Ă��������B

	//Sub Routine
	//bool Initialize();
	//virtual void Destroy();
	void SetMain(){mCMain=this;} //�������Ȃ�����

	//Main Routine
	BOOL CheckArchive(LPCSTR lpszArc,const int iMode);
	//int  GetFileCount(LPCSTR lpszArc);
	BOOL ConfigDialog(const HWND hWnd, LPSTR lpszBuf, const int iMode);
	BOOL QueryFunctionList(const int iFunction);

protected:
	//�ϐ�
};

#endif