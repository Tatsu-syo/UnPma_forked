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
//*p��2byte������2byte�ڂł͂Ȃ����Ƃ�z�肵�Ă��܂��B
#define jmscode(p) ((isJMS(p,0)==1)?(*(p)<<8)+*(p+1):*(p))

//Debug�p�B
#define MessageDlg(c0,c1) MessageBox(NULL,c0,c1,MB_OK)
//Debug�I�����悷���ɍ폜���邱�ƁB
#define MessageInt(c0,c1) {char IDebug[9];sprintf(IDebug,"%08X",c0);MessageBox(NULL,IDebug,c1,MB_OK);}
//���f�[�^�{���ɁB
#define WriteRawData(p, l) fwrite(p, l, 1, stdout)

//ptr�������NULL�ɂ���B
#define FreeAndNil(ptr) {free(ptr);ptr=NULL;}
#define DeleteAndNil(ptr) {delete ptr;ptr=NULL;}

//�㋉�Ҍ���(^^;;; ���Ă�����ReverseBytes/memcpy�ɑ���ȊO�ɗp�r���v�����Ȃ��c
#define MakePointer(x, l) (((BYTE*)&(x))+(l))

//���E��n�r�b�g�V�t�g�B
//cb�ɂ͐����^�̃r�b�g�T�C�Y�����邱�ƁB(ex. int:32)
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

//�\�[�X�̏�̂ق���CreateDialogProc(�R�[���o�b�N��, �N���X��, �����o�֐���);��
//�錾����ƁA�E�B���h�E�v���V�[�W�����N���X�̃����o�ɂ��邽�߂̊֐�������Ă���܂��B
//�������A�_�C�A���O��"�`Param" API�ō쐬�AlParam�ɃN���X�̃|�C���^���w�肵�A
//�����o�֐����ł�SetWindowLong(hWnd, GWL_USERDATA, ...)���Ȃ��ł��������B
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
#define ENUM_SHOWDOSNAME		4 //������
#define ENUM_ADDDIRECTORY		8
#define ENUM_CONFIRMOVERWRITE	16 //������
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

	//���݂܂���c
	typedef unsigned char bool;
	#define false FALSE
	#define true  TRUE
#endif

//bool��Ԃ��ꍇ�̌x���΍�B
#define returnbool(b) return (b) ? true : false 

typedef unsigned short crc16table[256];
typedef unsigned int   crc32table[256];

#endif
