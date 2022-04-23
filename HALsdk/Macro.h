//
// Macro.h
// Macro of HALsdk.
//
// $Id: Macro.h,v 1.9 2005/10/30 04:17:50 halpussy Exp $
//

#ifndef HALSDK_MACRO_H
#define HALSDK_MACRO_H

#pragma warning(disable:4996) // for New C RunTime Library. I'll not use strcpy_s.

#if (defined __cplusplus) && (defined __BORLANDC__)
namespace std {
typedef unsigned long time_t;
#define _TIME_T_DEFINED
#define _TIME_T std::time_t
}
#else
typedef unsigned long time_t;
#define _TIME_T_DEFINED
#define _TIME_T time_t
#endif

//n1<=x<=n2
#define Between(n1, x, n2) (((n1)<=(x))&&((x)<=(n2)))

#define jms1(c) (Between(0x81,(BYTE)(c),0x9f)||Between(0xe0,(BYTE)(c),0xfc))
#define jms2(c) (((BYTE)(c)!=0x7F)&&Between(0x40,(BYTE)(c),0xFC))
#define jmsnext(p) (!(*(p))?p:(jms1(*(p))&&jms2(*(p+1)))?p+2:p+1)
#define Upcase(c) (Between('a',(BYTE)(c),'z')?c-' ':c)
#define Lowercase(c) (Between('A',(BYTE)(c),'Z')?c+' ':c)
#define isJMS(p,i) ((*(p)==0)?0:(jms1(*(p))&&jms2(*(p+1))&&(i==0||i==2))?1:(i==1)?2:0)
//*pは2byte文字の2byte目ではないことを想定しています。
#define jmscode(p) ((isJMS(p,0)==1)?(*(p)<<8)+*(p+1):*(p))

//Debug用。
#define MessageDlg(c0,c1) MessageBox(NULL,c0,c1,MB_OK)
//Debug終了次第すぐに削除すること。
#define MessageInt(c0,c1) {char IDebug[9];sprintf(IDebug,"%08X",c0);MessageBox(NULL,IDebug,c1,MB_OK);}
//生データ閲覧に。
#define WriteRawData(p, l) fwrite(p, l, 1, stdout)

//ptrを解放しNULLにする。
#define FreeAndNil(ptr) {free(ptr);ptr=NULL;}
#define DeleteAndNil(ptr) {delete ptr;ptr=NULL;}

//上級者向け(^^;;; っていうかReverseBytes/memcpyに送る以外に用途が思いつかない…
#define MakePointer(x, l) (((BYTE*)&(x))+(l))

//左右にnビットシフト。
//cbには整数型のビットサイズを入れること。(ex. int:32)
#define RotateLeft(x, n, cb) (((x) << (n)) | ((x) >> ((cb)-(n))))
#define RotateRight(x, n, cb) (((x) >> ((cb)-(n))) | ((x) << (n)))

#define MAKELONGLONG(lo, hi) ((INT64)( (DWORD)(lo) ) | ( (INT64)( (DWORD)(hi) )<<32 ))

//Open Files
#define CreateFile2(szFile, dwAccess, dwShare, dwDisposition, dwFlags) \
	CreateFile(szFile, dwAccess, dwShare, NULL, dwDisposition, dwFlags, NULL)

/* Currently Disabled.
#define FileOpen(szFile, dwAccess, dwShare, dwDisposition) \
	CreateFile2(szFile, dwAccess, dwShare, dwDisposition, FILE_ATTRIBUTE_NORMAL)

#define FileOpenRead(szFile) \
	CreateFile2(szFile, GENERIC_READ, FILE_SHARE_READ, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL)

#define FileOpenReadSequential(szFile) \
	CreateFile2(szFile, GENERIC_READ, FILE_SHARE_READ, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL|FILE_FLAG_SEQUENTIAL_SCAN)

#define FileOpenReadRandom(szFile) \
	CreateFile2(szFile, GENERIC_READ, FILE_SHARE_READ, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL|FILE_FLAG_RANDOM_ACCESS)

#define FileOpenWrite(szFile, dwDisposition) \
	CreateFile2(szFile, GENERIC_WRITE, FILE_SHARE_READ, dwDisposition, FILE_ATTRIBUTE_NORMAL)

#define FileOpenWriteSequential(szFile, dwDispositon) \
	CreateFile2(szFile, GENERIC_WRITE, FILE_SHARE_READ, dwDisposition, FILE_ATTRIBUTE_NORMAL|FILE_FLAG_SEQUENTIAL_SCAN)

#define FileOpenWriteRandom(szFile, dwDisposition) \
	CreateFile2(szFile, GENERIC_WRITE, FILE_SHARE_READ, dwDisposition, FILE_ATTRIBUTE_NORMAL|FILE_FLAG_RANDOM_ACCESS)
*/

//ソースの上のほうでCreateDialogProc(コールバック名, クラス名, メンバ関数名);と
//宣言すると、ウィンドウプロシージャをクラスのメンバにするための関数を作ってくれます。
//ただし、ダイアログは"～Param" APIで作成、lParamにクラスのポインタを指定し、
//メンバ関数側ではSetWindowLong(hWnd, GWL_USERDATA, ...)しないでください。
#define CreateDialogProc(func, type, procedure) \
	static BOOL CALLBACK func(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam){ \
		if(uMsg==WM_INITDIALOG){ \
			SetWindowLong(hWnd, GWL_USERDATA, lParam); \
		} \
		return ((type*)GetWindowLong(hWnd, GWL_USERDATA))->procedure(hWnd, uMsg, wParam, lParam); \
	}


//EnumFileList Flags
#define ENUM_NOTRECURSE			0
#define ENUM_RECURSEDIR			1
#define ENUM_RECURSEFILE		2
#define ENUM_SHOWDOSNAME		4 //未実装
#define ENUM_ADDDIRECTORY		8
#define ENUM_CONFIRMOVERWRITE	16 //未実装
#define ENUM_ADDEMPTYDIR		32
#define ENUM_ADDALTSTREAM		64

//IsMatchWildcard2 Flags
#define WILD_MODE_STRING		0
#define WILD_MODE_SAMEDIRECTORY	1
#define WILD_MODE_RECURSIVE		2
#define WILD_MODE_CUT_WILDCARD	4
#define WILD_MODE_CUT_STRING	8

//GetOSType Return
#define WIN31		0
#define WIN95OSR1	1
#define WIN95OSR2	2
#define WIN95OSR25	3
#define WIN98		4
#define WIN98SE		5
#define WINME		6
#define WIN9XNEW	7
#define WINNT3 		10
#define WINNT4		11
#define WIN2K		12
#define WINXP		13
#define WIN2K3		14
#define WINNTNEW	15
#define WINLH		20

#define FNAME_MAX32 512

//extern "C"
#ifdef __cplusplus
	#define EXTERNC extern "C"
	#define EXTERNC_BEGIN extern "C"{
	#define EXTERNC_END }
#else
	#define EXTERNC
	#define EXTERNC_BEGIN
	#define EXTERNC_END

	//すみません…
	typedef unsigned char bool;
	#define false FALSE
	#define true  TRUE
#endif

//boolを返す場合の警告対策。
#define returnbool(b) return (b) ? true : false 

typedef unsigned short crc16table[256];
typedef unsigned int   crc32table[256];

#endif
