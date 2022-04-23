//
// Strings.h
// 文字列ポインタ管理
//
// $Id: Strings.h,v 1.1 2005/09/18 05:49:28 halpussy Exp $
//

#ifndef HALSDK_STRINGS_H
#define HALSDK_STRINGS_H

#include <windows.h>
#include <stdio.h>

//
// class Strings
// char**を手軽に扱うためのクラス。
// HALsdkの主要なクラスです。
// DelphiのTStringsや、STLのvectorをかなり意識したつくりになっています。
//

class Strings
{
public:
	Strings(){argc=0;argv=NULL;maximum=0;}
	virtual ~Strings(){Discard();}

	virtual void Discard();

	//Strings[0]とStrings::vector()[0]はほぼ同じ意味ですが、
	//Strings::vector()[argc以上の値]を呼び出すと堕ちてしまいますので、
	//できればStrings[0]をお使いください。

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
