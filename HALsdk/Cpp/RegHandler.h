//
// RegHandler.h
// ���W�X�g���Ǘ�
//
// $Id: RegHandler.h,v 1.1 2005/09/18 05:49:28 halpussy Exp $
//

#ifndef HALSDK_REGHANDLER_H
#define HALSDK_REGHANDLER_H

#include <windows.h>

// Class RegHandler
// �A�N�Z�X�����͍l�����Ă��܂���c

// RegOpenKeyEx/RegCreateKeyEx�́A���W�X�g�������b�N���铭��������̂Œ��ӁB

class RegHandler
{
public:
	RegHandler(){mdwType=0;mlpData=NULL;mcbData=0;}
	virtual ~RegHandler(){Close();}

	bool Open(HKEY hParent, LPCSTR szKeyName);
	bool Create(HKEY hParent, LPCSTR szKeyName);
	bool Close();

	bool Delete(HKEY hParent, LPCSTR szKeyName); //�J���Ă���Œ��ɌĂяo�����Ƃ��ꉞ�\�B

	bool Get(LPCSTR szEntry);
	bool Set(LPCSTR szEntry, const DWORD dwType, const LPBYTE lpData, const DWORD cbData);

	operator HKEY() const{return hKey;} //Open/Create��hParent�ɗp���邱�Ƃ��ł���B

	DWORD mdwType;
	LPBYTE mlpData; //Get�̃f�[�^���i�[����B���̃o�b�t�@�̃T�C�Y��mcbData���l�����������s���B
	DWORD mcbData;

protected:
	HKEY hKey;
};

#endif
