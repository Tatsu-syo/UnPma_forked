//
// PathCtrl.c
// パス操作
//
// $Id: PathCtrl.c,v 1.1 2005/09/18 05:49:06 halpussy Exp $
//

#include "../HALsdk.h"

bool AbbreviationPath(LPCSTR szSrc, LPSTR szDest, const int iLength){
	int i=0;
	int j=0;
	int point = (iLength / 2) - 2;
	int lpos;

	if(!szSrc||!szDest)
		return false;
	if ((int)strlen(szSrc) < iLength)
		strcpy(szDest,szSrc);
	else{
		lpos = strlen(szSrc) - (iLength - (point + 3));
		for (;i < iLength;i++){
			if (i < point)
				*(szDest + i) = *(szSrc + i);
			else if (i >= point && i <= point + 2)
				*(szDest + i) = '.';
			else
				*(szDest + i) = *(szSrc + lpos + j++);
		}
		*(szDest + iLength) = '\0';
	}
	return true;
}

bool ConvertAbsPathToRelPath(LPCSTR src,LPSTR dest){
	int i;
	int j=0;
	int l;
	int cut_size = 0;
	char _src[MAX_PATH * 2];

	if(!src||!dest)
		return false;
	strcpy(_src,src);
	l = strlen(_src);

	if(l > 2){
		if (_src[0] == '\\'){
			cut_size = 1;
			if (_src[1] == '\\')
				cut_size = 2;
        }
    }
	if (l > 3){
		if (_src[1] == ':' && _src[2] == '\\')
			cut_size = 3;
	}
	for (i = cut_size;i < l;i++,j++)
		dest[j] = _src[i];
	dest[j] = '\0';
	return true;
}

bool GetRelativeDir(LPCSTR src, LPCSTR base, LPSTR dest){
	//srcもbaseもディレクトリのみの指定、最後の\はつけずにお願いします。
	char fixedsrc[513];
	char fixedbase[513];
	UINT i=0,j=0;
	int Ascii=0;

	if(!src||!base||!dest)
		return false;

	//フルパスか相対パスでそろえられていなければ弾く
	if(strlen(src)>1&&strlen(base)>1&&(src[1]==':'||base[1]==':')&&src[1]!=base[1])
		return false;

	//destにbaseをコピー
	strcpy(dest,base);

	//パス区切りを'/'にする
	strcpy(fixedsrc, src);
	strcpy(fixedbase, base);
	for(;i<strlen(src);i++){
		Ascii=isJMS(src+i,Ascii);
		if(!Ascii){
			fixedsrc[i]=Upcase(src[i]);
			if(src[i]=='\\')fixedsrc[i]='/';
		}
	}
	Ascii=0;i=0;
	for(;i<strlen(base);i++){
		Ascii=isJMS(base+i,Ascii);
		if(!Ascii){
			fixedbase[i]=Upcase(base[i]);
			if(base[i]=='\\')fixedbase[i]='/';
		}
	}
	i=0;

	//srcとbaseをそろえる(src+i以降で比較)
	for(;fixedsrc[i]==fixedbase[i];i++){if(fixedsrc[i]=='/')j=i;}
	strcpy(fixedsrc,fixedsrc+j+1);
	strcpy(fixedbase,fixedbase+j+1);
	i=0;

	//fixedbaseのディレクトリ数を数える
	if(*fixedbase){
		i=1;
		for(j=0;j<(int)strlen(fixedbase);j++)
			if(fixedbase[j]=='/') i++;
	}

	//..\を入れる
	for(j=0;j<=i;j++)
		strcat(dest,"..\\");

	//fixedsrcをコピーする
	if(*fixedsrc)
		strcat(dest,fixedsrc);

	return true;
}

bool FixPath(LPCSTR src, LPSTR dest){
	//destはstrlen(src)分確保しておくこと
	char path[513];
	int i=0,j=0;
	int Ascii=0;

	if(!src||!dest) return false;

	NullMemory(dest,strlen(src)+1);
	NullMemory(path,sizeof(path));

	for(;i<(int)strlen(src);i++){
		Ascii=isJMS(src+i,Ascii);
		if(!Ascii&&(src[i]=='\\'||src[i]=='/')){
			if(strcmp(path,".")){
				if(strlen(path)>1&&path[0]=='.'&&path[1]=='.') path[0]=path[1]='_';
				strcat(dest,path);
				dest[strlen(dest)]=src[i];
			}
			j=0;
			NullMemory(path,sizeof(path));
			continue;
		}
		path[j] = src[i];
		j++;
	}

	if(strlen(path)>1&&path[0]=='.'&&path[1]=='.') path[0]=path[1]='_';
	strcat(dest,path);
	return true;
}

bool FixPathDirect(LPSTR path){
	char sz[513];

	if(!path) return false;
	FixPath(path,sz);
	strcpy(path,sz);
	return true;
}
