//
// Strings.h
// ������|�C���^�Ǘ�
//
// $Id: Strings.h,v 1.1 2005/09/18 05:49:28 halpussy Exp $
//

#ifndef HALSDK_STRINGS_H
#define HALSDK_STRINGS_H

#include <windows.h>
#include <stdio.h>

//
// class Strings
// char**����y�Ɉ������߂̃N���X�B
// HALsdk�̎�v�ȃN���X�ł��B
// Delphi��TStrings��ASTL��vector�����Ȃ�ӎ���������ɂȂ��Ă��܂��B
//

class Strings
{
public:
	Strings(){argc=0;argv=NULL;maximum=0;}
	virtual ~Strings(){Discard();}

	virtual void Discard();

	//Strings[0]��Strings::vector()[0]�͂قړ����Ӗ��ł����A
	//Strings::vector()[argc�ȏ�̒l]���Ăяo���Ƒ��Ă��܂��܂��̂ŁA
	//�ł����Strings[0]�����g�����������B

	char* operator [](const UINT index) const{
		if(index>=argc) return NULL;
		return argv[index];
	}

	UINT count()    const {return argc;} //Argument Count
	char** vector() const {return argv;} //Argument Vector

	BOOL Duplicate(LPCSTR lpszName, const int CheckAa);

	BOOL Secure();
	BOOL Add(LPCSTR lpszName);
	BOOL Reconstruct(UINT &_argc, char** &_argv);
	BOOL Erase(const UINT index);
	void Remove(LPCSTR lpszName, const int CheckAa);

protected:
	UINT argc;
	char **argv;
	UINT maximum;
};

#endif
