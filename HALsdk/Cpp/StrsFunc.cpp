//
// StrsFunc.cpp
// Strings���g�����֐�
//
// $Id: StrsFunc.cpp,v 1.1 2005/09/18 05:49:28 halpussy Exp $
//

#include "../HALsdkCp.h"

#define TEST

bool EnumFileList(Strings &sRet, LPCSTR wildname, LPCSTR current, const DWORD dwEnumMode){
	//current��FindFirstFile�A�t�@�C������wildname�ƃ}�b�`��sRet�ƃ}�b�`���Ȃ���Βǉ��B
	//���̂��߁Acurrent���f�B���N�g���̏ꍇ�A
	//���C���h�J�[�h���܂�ł͂Ȃ�Ȃ��B( [Dir]/[File] )
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
						//���ʂɐ����Ă������Ƃ̖����悤�K���B
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

					//*.cpp��d.cpp\a.cpp�����������̂��K��
					if((dwEnumMode&ENUM_RECURSEFILE)||( (dwEnumMode&ENUM_RECURSEDIR)&&LastByte(szWild)=='*' )){
						ret = EnumFileList(sRet, wildname, szFile, dwEnumMode);
						if(!ret)
							return FALSE;
					}

					if(!(dwEnumMode&ENUM_ADDEMPTYDIR)){
						//��̃f�B���N�g�����i�[���Ȃ��̂Ȃ�A
						//�f�B���N�g���i�[���[�h�ł��t�@�C�����i�[����Ȃ��Ƃ�(�ċA�I�������s��Ȃ��ꍇ���܂�)
						//�f�B���N�g�����폜����B
						if((dwEnumMode&ENUM_ADDDIRECTORY)&&!strcmp(szFile,sRet[sRet.count()-1]))
							sRet.Erase(sRet.count()-1);
					}else{
						//��̃f�B���N�g�����i�[����̂Ȃ�A
						//�f�B���N�g���i�[���[�h�ł��t�@�C�����i�[���ꂸ�A
						//�t�@�C�����݂̂��ċA�I�Ɍ�������Ƃ�
						//�f�B���N�g�����폜����B
						//�v����ɁAENUM_RECURSEFILE���w�肷���ENUM_ADDEMPTYDIR�͖��Ӗ��ɂȂ�܂��B
						//�������Ȃ��Ɩ��ʂȃf�B���N�g������������i�[����Ă��܂��̂ŁB�B�B

						//��ENUM_ADDEMPTYDIR�́AENUM_NOTRECURSE��ENUM_RECURSEDIR�ł̂ݓ��삷��悤�ɂȂ�܂��B
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
					//�f�B���N�g�������f�B���N�g��\*�Ƃ��ĉ��߂��Ă݂�B
					//�����̋@�\�̓t�@�C�����݂̂��ċA�I��������ۂɂ͖����ɂȂ�܂��B
					//���ud�v�͋K��f�B���N�g���ȉ��S�Ẵt�@�C���ud�v����������Ƃ����Ӗ��ɂȂ��Ă��܂����߁B
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
	//&sCom�ɂ��Ȃ��ƃf�X�g���N�g����Ă��܂��B�B�B
	char szDir[FNAME_MAX32+1];

	for(DWORD d=0;d<sCom.count();d++){
		if(!sCom[d])continue;
		GetDirectory(szDir,sCom[d],0); //�f�B���N�g���������擾
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