//
// Strings.cpp
// 文字列ポインタ管理
//
// $Id: Strings.cpp,v 1.2 2005/10/10 15:52:02 halpussy Exp $
//

#include "../HALsdk.h"
#include "Strings.h"

void Strings::Discard(){
	UINT n(0);
	if(argc>0){
		for(;n<argc;n++)
			FreeAndNil(argv[n]);
	}
	FreeAndNil(argv);
	argc=0;
	maximum=0;
}

BOOL Strings::Duplicate(LPCSTR lpszName, const int CheckAa){
	UINT n(0);
	for(;n<argc;n++){
		if(!argv[n])
			continue;
		if(strlen(argv[n])!=strlen(lpszName))
			continue;
		if(CheckAa){
			if(!strcmp(argv[n],lpszName))
				break;
		}else{
			if(!strcmpu(argv[n],lpszName))
				break;
		}
	}
	return (n<argc);
}

BOOL Strings::Secure(){
	if(argc==maximum){
		char **x;
		maximum+=32;
		x = (char**)realloc(argv,sizeof *x *maximum);
		if(!x) return FALSE;
		argv = x;
	}
	return TRUE;
}

BOOL Strings::Add(LPCSTR lpszName){
	if(!lpszName)
		return FALSE;
	if(!Secure())
		return FALSE;
	argv[argc]=(char*)malloc(strlen(lpszName)+1);
	if(!argv[argc])
		return FALSE;
	strcpy(argv[argc],lpszName);
	argc++;
	return TRUE;
}

BOOL Strings::Reconstruct(UINT &_argc, char** &_argv){
	//char**を引数にとる関数用。
	//reallocだと同時に解放されてしまうから。。。
	UINT n(0),n0(0);
	_argv = (char**)malloc(sizeof *_argv *argc);
	if(!_argv) return FALSE;
	for(;n<argc;n++){
		if(!argv[n])continue;
		_argv[n0] = (char*)malloc(FNAME_MAX32+1);
		if(!_argv[n0]){
			for(;n0>0;n0--)
				FreeAndNil(_argv[n0-1]);
			return FALSE;
		}
		strcpy(_argv[n0], argv[n]);
		n0++;
	}
	_argc=n0;
	return TRUE;
}

BOOL Strings::Erase(const UINT index){
	UINT n(index);
	if(index>=argc) return FALSE;
	FreeAndNil(argv[index]);
	if(argc>1){
		for(;n<argc-1;n++)
			argv[n]=argv[n+1];
		argv[n+1]=NULL;
	}
	argc--;
	return TRUE;
}

void Strings::Remove(LPCSTR lpszName, const int CheckAa){
	UINT n(0);
	for(;n<argc;){
		if(argv[n]){
			if(CheckAa){
				if(!strcmp(argv[n],lpszName))
					{Erase(n);continue;}
			}else{
				if(!strcmpu(argv[n],lpszName))
					{Erase(n);continue;}
			}
		}
		n++;
	}
}
