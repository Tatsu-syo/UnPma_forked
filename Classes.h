//
// Classes.h
// DLLのクラスの宣言
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

	//SetAttributeはオーバーライドする必要はありません。
	//GetArcRatio/GetRatioは、圧縮率のフォーマットを変えたい場合のみオーバーライドしてください。

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
	//変数

	//定義済み変数
	//mhWnd(HWND) 親ウィンドウ
	//mszArchive(char [FNAME_MAX32+1]) 書庫名
	//mdwMode(DWORD) オープンモード
	//mszOption(LPSTR) オプション
	//mszWildName(LPSTR) ワイルドカード
	//mllArcReadSize(INT64) 読み込んだサイズ
	//muOSType(UINT) 書庫のOSタイプ
	//miSFX(int) SFX形式
	//mSubInfoEx(INDIVIDUALINFOEX構造体)
};

class CCommand : public baseCCommand
{
public:
	CCommand(const CMain *main, const HWND hWnd, LPCSTR lpszCmd, LPSTR lpszOut, const DWORD dwSize,
		const OWNERWINDOWINFO Owner, const MEMBERSPROCINFO Members, const THREADINFO ThreadInfo);

	//Commandからの呼び出しはかならずArcProc/MembProcでお願いします。
	//ArchiverProc/MembersProcを呼び出すと、
	//SetOwnerWindow/SetEnumMembersProc系が正しく動作しなくなります。

	//逆に、内部でのみ処理をしたい場合にはArchiverProc/MembersProcを呼び出すのが良いでしょう。

	//Sub Routine
	bool ArchiverProc(); //内蔵コールバック
	bool MembersProc();  //内蔵コールバック

	BOOL ConfirmProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	BOOL ProgressProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	//Main Routine
	int Command();

public:
	WORD wConfirm;
	bool showdlg;

	//定義済み変数
	//cmd(CmdLineクラス) 解析済みコマンドライン
	//bf(BufWriterクラス) 書き込み用バッファ
	//mExInfo(EXTRACTINFO構造体)
	//mMemberInfo(MEMBERSINFO構造体)
	//mnState(UINT) ArcProcで使用
	//mwLangID(LANGID) 言語リソース取得に使用

protected:
	//変数
	HWND hProgress;

	//定義済み変数
	//mhWnd(HWND) 親ウィンドウ
	//mThreadInfo(THREADINFO)
};

class CMain : public baseCMain
{
public:
	CMain(const HMODULE hDll);

	//Initialize/Destroy/GetFileCountはオーバーライドする必要はありません。
	//GetFileCountは、書庫なのにOpenArchiveできない可能性がある
	//(ヘッダが暗号化されているなど)場合にオーバーライドしてください。

	//Sub Routine
	//bool Initialize();
	//virtual void Destroy();
	void SetMain(){mCMain=this;} //消去しないこと

	//Main Routine
	BOOL CheckArchive(LPCSTR lpszArc,const int iMode);
	//int  GetFileCount(LPCSTR lpszArc);
	BOOL ConfigDialog(const HWND hWnd, LPSTR lpszBuf, const int iMode);
	BOOL QueryFunctionList(const int iFunction);

protected:
	//変数
};

#endif