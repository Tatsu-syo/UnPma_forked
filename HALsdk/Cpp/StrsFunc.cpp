//
// StrsFunc.cpp
// Stringsを使った関数
//
// $Id: StrsFunc.cpp,v 1.1 2005/09/18 05:49:28 halpussy Exp $
//

#include "../HALsdkCp.h"

#define TEST

bool EnumFileList(Strings &sRet, LPCSTR wildname, LPCSTR current, const DWORD dwEnumMode){
	//currentでFindFirstFile、ファイル名がwildnameとマッチしsRetとマッチしなければ追加。
	//このため、currentがディレクトリの場合、
	//ワイルドカードを含んではならない。( [Dir]/[File] )
	char szFile[FNAME_MAX32+1];
	char szDir[FNAME_MAX32+3];
	WIN32_FIND_DATA wfd;
	HANDLE hFind;
	BOOL ret;

	strcpy(szDir,current);

	if(!IsDir(current)){
		strcat(szDir,"\\*");
		hFind = ::FindFirstFile(szDir,&wfd);
		szDir[strlen(szDir)-1]=0;
	}else{
		hFind = ::FindFirstFile(/*szDir*/"*",&wfd);
		strcpy(szDir,"");
	}

	if(hFind!=INVALID_HANDLE_VALUE){
		do{
			strcpy(szFile,szDir);
			strcat(szFile,wfd.cFileName);
			if(wfd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY){
				if(strcmp(wfd.cFileName,".")&&strcmp(wfd.cFileName,".." )){
					char szWild[FNAME_MAX32+1];

					if(!(dwEnumMode&ENUM_RECURSEFILE)){
						//無駄に潜っていくことの無いよう規制。
						strcpy(szWild, wildname);
						if(!IsDir(wildname))
							strcat(szWild,"\\*");
						if(!IsMatchWildcard(szWild, szFile))
							continue;
					}

					if(!sRet.Duplicate(szFile,0)&&(dwEnumMode&ENUM_ADDDIRECTORY)){
						if(!sRet.Add(szFile)){
							return FALSE;
						}
					}

					//*.cppでd.cpp\a.cppが検索されるのを規制
					if((dwEnumMode&ENUM_RECURSEFILE)||( (dwEnumMode&ENUM_RECURSEDIR)&&LastByte(szWild)=='*' )){
						ret = EnumFileList(sRet, wildname, szFile, dwEnumMode);
						if(!ret)
							return FALSE;
					}

					if(!(dwEnumMode&ENUM_ADDEMPTYDIR)){
						//空のディレクトリを格納しないのなら、
						//ディレクトリ格納モードでかつファイルが格納されないとき(再帰的検索を行わない場合を含む)
						//ディレクトリを削除する。
						if((dwEnumMode&ENUM_ADDDIRECTORY)&&!strcmp(szFile,sRet[sRet.count()-1]))
							sRet.Erase(sRet.count()-1);
					}else{
						//空のディレクトリを格納するのなら、
						//ディレクトリ格納モードでかつファイルが格納されず、
						//ファイル名のみを再帰的に検索するとき
						//ディレクトリを削除する。
						//要するに、ENUM_RECURSEFILEを指定するとENUM_ADDEMPTYDIRは無意味になります。
						//そうしないと無駄なディレクトリがたくさん格納されてしまうので。。。

						//※ENUM_ADDEMPTYDIRは、ENUM_NOTRECURSEかENUM_RECURSEDIRでのみ動作するようになります。
						if( (dwEnumMode&(ENUM_ADDDIRECTORY|ENUM_RECURSEFILE))
							&&!strcmp(szFile,sRet[sRet.count()-1]))
								sRet.Erase(sRet.count()-1);
					}
				}
			}else{
				if(dwEnumMode&ENUM_RECURSEFILE){
					char szFileName[FNAME_MAX32+1];
					GetFileName(szFileName,wildname);
					if(!IsMatchWildcard(szFileName,wfd.cFileName))continue;
				}else{
					//ディレクトリ名をディレクトリ\*として解釈してみる。
					//※この機能はファイル名のみを再帰的検索する際には無効になります。
					//※「d」は規定ディレクトリ以下全てのファイル「d」を検索するという意味になってしまうため。
					char szWild[FNAME_MAX32+1];
					strcpy(szWild, wildname);
					if(!IsDir(wildname))
						strcat(szWild,"\\*");
					if(!IsMatchWildcard(szWild, szFile))
						continue;
				}

				if(!sRet.Duplicate(szFile,0)){
					if(!sRet.Add(szFile)){
						return FALSE;
					}

					if(dwEnumMode&ENUM_ADDALTSTREAM){
						//UINT u=sRet.length();
						EnumAlternativeStream(sRet,szFile);
						/*
						UINT u0=sRet.length();
						for(;u<u0;){
							if(IsMatchWildcard(wildname,sRet[u])){
								sRet.Erase(u);
								u0--;
							}else
								u++;
						}
						*/
					}

				}
			}
		}while (::FindNextFile( hFind, &wfd ));

		::FindClose( hFind ) ;
	}

	return TRUE;
}

bool UnWildcardS(Strings &sRet, const Strings &sCom, const DWORD dwMakemode){
	//&sComにしないとデストラクトされてしまう。。。
	char szDir[FNAME_MAX32+1];

	for(DWORD d=0;d<sCom.count();d++){
		if(!sCom[d])continue;
		GetDirectory(szDir,sCom[d],0); //ディレクトリ部分を取得
		EnumFileList(sRet,sCom[d],szDir,dwMakemode);
	}

	return TRUE;
}

#ifdef TEST

void main(int argc, char **argv){
	int i=1;
	UINT u=0;
	char szDir[FNAME_MAX32+1];
	Strings sRet;

	for(;i<argc;i++){
		GetDirectory(szDir,argv[i],0);
		EnumFileList(sRet,argv[i],szDir,ENUM_RECURSEDIR);
	}

	for(;u<sRet.count();u++){
		printf("%s\n",sRet[u]);
	}
}

#endif