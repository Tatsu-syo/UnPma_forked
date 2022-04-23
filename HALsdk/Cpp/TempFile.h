//
// TempFile.h
// �ꎞ�t�@�C��
//
// $Id: TempFile.h,v 1.1 2005/09/18 05:49:28 halpussy Exp $
//

#ifndef HALSDK_TEMPFILE_H
#define HALSDK_TEMPFILE_H

#include <windows.h>

class TempFile
{
public:
	TempFile();
	virtual ~TempFile(){Done(NULL);}

	//�ꎞ�t�@�C���쐬
	UINT Create(LPCSTR lpszPrefix, LPCSTR lpszWorkDir);

	//�ꎞ�t�@�C���̏������I��������Ƃ�ʒm���A�K�v�Ȃ�Ώ����߂�
	bool Done(LPCSTR lpszCopyTo);

public:
	//LPSTR()�ŕԂ��ꂽ�t�@�C�����Ńt�@�C�����J�����Ƃ͂ł��܂���BHANDLE()�ő��삵�Ă��������B
	operator HANDLE() const  { return mhFile; }
	operator LPSTR() const   { return (LPSTR)mszTempFile; }

protected:
	char		mszTempFile[MAX_PATH+1];
	HANDLE		mhFile;
	bool		mbOpened;

private:
	TempFile(const TempFile&);
	void operator=(const TempFile&);
};

#endif