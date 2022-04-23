//
// Pointers.h
// 汎用ポインタ管理
//
// $Id: Pointers.h,v 1.2 2005/10/10 14:18:53 halpussy Exp $
//

#ifndef HALSDK_POINTERS_H
#define HALSDK_POINTERS_H

#include <windows.h>
#include <stdio.h>
#include "../HALsdk.h"

//
// class Pointers
// ポインタのポインタを手軽に、２次元配列として扱うためのクラス。
// 性質上、ポインタ自体はDuplicateすることはない。
//
// クラスのポインタを扱うのが主な用途です。
// Addするポインタはnewで確保すること。
//
// _Tにはポインタ型を指定すること。
//
// 2005.10.03 template使ったせいでcppと分離できなくなった(^^;;;
//

template<typename _T>
class Pointers
{
public:
	Pointers(){argc=0;argv=NULL;maximum=0;}
	virtual ~Pointers(){Discard();}

	virtual void Discard(){
		UINT n(0);
		if(argc>0){
			for(;n<argc;n++){
				DeleteAndNil(argv[n]);
			}
		}
		FreeAndNil(argv);
		argc=0;
		maximum=0;
	}

	//Pointers[0]とPointers::vector()[0]は同じ意味です。

	_T operator [](const UINT index) const{
		if(index>=argc) return NULL;
		return argv[index];
	}

	UINT	count()   const {return argc;} //Argument Count
	_T		*vector() const {return argv;} //Argument Vector

	BOOL Secure(){
		if(argc==maximum){
			_T *x;
			maximum+=32;
			x = (_T*)realloc(argv,sizeof(LPVOID)*maximum);
			if(!x) return FALSE;
			argv = x;
		}
		return TRUE;
	}

	BOOL Add(_T pointer){
		if(!pointer)
			return FALSE;
		if(!Secure())
			return FALSE;
		argv[argc] = pointer;
		argc++;
		return TRUE;
	}

	BOOL Erase(const UINT index){
		UINT n(index);
		if(index>=argc) return FALSE;
		DeleteAndNil(argv[index]);
		if(argc>1){
			for(;n<argc-1;n++)
				argv[n]=argv[n+1];
			argv[n+1]=NULL;
		}
		argc--;
		return TRUE;
	}

	void Remove(_T pointer){
		UINT n(0);
		for(;n<argc;){
			if(argv[n]==pointer)
				{Erase(n);return;} //continueした後この行にくることはないはず。
		}
	}

	_T Find(_T pointer) const{
		UINT n(0);
		for(;n<argc;){
			if(argv[n]==pointer)
				return argv[n];
		}
		return NULL;
	}

	_T Last() const{
		if(!argc)
			return NULL;
		return argv[argc-1];
	}

protected:
	UINT	argc;
	_T		*argv;
	UINT	maximum;
};

#endif
