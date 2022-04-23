//
// HALsdk.c
// Main Routine of HALsdk.
//
// $Id: HALsdk.c,v 1.2 2005/10/10 14:18:28 halpussy Exp $
//

#include "../HALsdk.h"
#include <stdio.h>
#include <direct.h> //mkdir
#include <windows.h>

int MakeDirectory(LPCSTR lpszDir){
	char name[FNAME_MAX32+1];
	int l;
	int ret=-1,i=0,Ascii=0;

	if(!lpszDir) return ret;
	l=strlen(lpszDir);

	NullMemory(name,FNAME_MAX32+1);
	for(;i<l;i++){
		Ascii=isJMS(lpszDir+i,Ascii);
		strncpy(name,lpszDir,i+1);
		if((lpszDir[i]=='\\'||lpszDir[i]=='/')&&!Ascii)
			ret=mkdir(name);
	}
	return ret;
}

int jmsmatch(LPCSTR pattern, LPCSTR compare){
    for (;*pattern;++pattern){
		switch(*pattern){
			case '?' : //0x3f
				return jmsmatch(pattern+1,jmsnext(compare));
			case '*' : //0x2a
				return jmsmatch(pattern+1,compare)||(*compare&&jmsmatch(pattern,jmsnext(compare)));
			default :
				if(jmscode(compare)!=jmscode(pattern)) return 0;
				return jmsmatch(jmsnext(pattern),jmsnext(compare));
		}
	}
	if (!*compare)return 1;
	return 0;
}

int IsMatchWildcard(LPCSTR lpszWildcard, LPCSTR lpszString){
	return IsMatchWildcard2(lpszWildcard, lpszString, WILD_MODE_STRING);
}

int IsMatchWildcard2(LPCSTR lpszWildcard, LPCSTR lpszString, const int iMode){
	int Ascii=0;
	UINT u=0,u1=0,u2=0;
	char pattern[513];
	char compare[513];

	if(!lpszWildcard||!lpszString)return 0;
	strcpy(pattern,lpszWildcard);
	strcpy(compare,lpszString);
	for(;u<strlen(pattern);u++){
		Ascii=isJMS(pattern+u,Ascii);
		if(!Ascii){
			pattern[u]=Upcase(pattern[u]);
			if(pattern[u]=='\\'){
				pattern[u]='/';
				u1++;
			}
		}
	}
	Ascii=0;u=0;

	for(;u<strlen(compare);u++){
		Ascii=isJMS(compare+u,Ascii);
		if(!Ascii){
			compare[u]=Upcase(compare[u]);
			if(compare[u]=='\\'){
				compare[u]='/';
				u2++;
			}
		}
	}
	Ascii=0;u=0;

	switch(iMode){
		case WILD_MODE_STRING: /* [条件]ディレクトリ構造を無視 */
		break;
		case WILD_MODE_SAMEDIRECTORY: /* [条件]ディレクトリ位置が同じ */
			if(u1!=u2){
				return FALSE;
			}
		break;
		case WILD_MODE_RECURSIVE: /* [条件]ディレクトリ位置が指定より下 */
			if(u1>u2){
				return FALSE;
			}
		break;
		default:
			return FALSE;
	}

	return jmsmatch(pattern, compare);
}

bool FileExists(LPCSTR lpszFile){
	//この関数でディレクトリの存在は調べられないので注意すること。
	HANDLE hExist;
	if(!lpszFile) return FALSE;
	hExist = CreateFile(lpszFile,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,
				FILE_ATTRIBUTE_NORMAL,NULL );
	CloseHandle(hExist);
	return (hExist!=INVALID_HANDLE_VALUE);
}

int SFXer(LPCSTR lpszSFX, LPCSTR lpszArc, LPCSTR lpszSFXArc){
	char Path1[MAX_PATH+1];
	HANDLE hSFX;
	HANDLE hArchive;
	HANDLE hSFXArc;
	DWORD bufsize;
	char *buf=(char*)malloc(65535);
	if(!buf) return 1;

	if(!lpszSFX||!lpszArc||!lpszSFXArc) {free(buf);return 1;}

	if( !SearchPath( NULL, lpszSFX, NULL, MAX_PATH, Path1, NULL ) ){
		free(buf);
		return 2;
	}

	hSFX     = CreateFile(Path1,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,
							FILE_ATTRIBUTE_NORMAL|FILE_FLAG_SEQUENTIAL_SCAN,NULL );
	hArchive = CreateFile(lpszArc,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,
							FILE_ATTRIBUTE_NORMAL|FILE_FLAG_SEQUENTIAL_SCAN,NULL );
	hSFXArc  = CreateFile(lpszSFXArc,GENERIC_WRITE,FILE_SHARE_READ,NULL,OPEN_ALWAYS,
							FILE_ATTRIBUTE_NORMAL|FILE_FLAG_SEQUENTIAL_SCAN,NULL );
	if( hSFX==INVALID_HANDLE_VALUE || hArchive==INVALID_HANDLE_VALUE || hSFXArc==INVALID_HANDLE_VALUE )
	{CloseHandle(hSFX),CloseHandle(hArchive),CloseHandle(hSFXArc);free(buf);return 3;}

	bufsize=GetFileSize(hArchive,NULL);

	while( ReadFile(hSFX,buf,65535,&bufsize,NULL) ){
		if( bufsize==0 )break;
		WriteFile(hSFXArc,buf,bufsize,&bufsize,NULL);
	}CloseHandle(hSFX);
	while( ReadFile(hArchive,buf,65535,&bufsize,NULL) ){
		if( bufsize==0 )break;
		WriteFile(hSFXArc,buf,bufsize,&bufsize,NULL);
	}CloseHandle(hArchive);
	CloseHandle(hSFXArc);

	free(buf);
	return 0;
}

int IsDir(LPCSTR lpszFile){
	WIN32_FIND_DATA Find;
	char szFile[FNAME_MAX32+1];
	UINT i=0; int Ascii=0;
	HANDLE hFind;

	for(;i<strlen(lpszFile);i++){
		if(lpszFile[i]=='*'||lpszFile[i]=='?')
			return 1;
		Ascii=isJMS(lpszFile+i,Ascii);
	}

	strcpy(szFile,lpszFile);

	if( (lpszFile[i-1]=='\\'||lpszFile[i-1]=='/') && !Ascii) //LastChar is Path.
		szFile[i-1]=0;

	hFind=FindFirstFile(szFile,&Find);
	if(hFind==INVALID_HANDLE_VALUE) return -1;
	FindClose(hFind);

	return (Find.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY) ? 0 : 1;
}

bool GetDirectory(LPSTR lpszDir, LPCSTR lpszSource, const int iGetMode){
	UINT i=0,j=0,Ascii=0;
	if(!lpszDir||!lpszSource) return false;
	if(!strcmp(lpszSource, "")){
		strcpy(lpszDir, lpszSource);
		return true;
	}

	for(;j<strlen(lpszSource);j++){
		Ascii=isJMS(lpszSource+j,Ascii);
		if((lpszSource[j]=='\\'||lpszSource[j]=='/')&&!Ascii){
			i=j;
			if(iGetMode)
				break;
		}
	}

	if(i==0) i=strlen(lpszSource);
	strncpy(lpszDir,lpszSource,i+1);
	lpszDir[i]=0; //確実にNULLにしておく。
	return true;
}

bool GetFileName(LPSTR lpszFile, LPCSTR lpszSource){
	UINT u=0,t=0,Ascii=0;
	if(!lpszFile||!lpszSource) return false;
	if(!strcmp(lpszSource, "")){
		strcpy(lpszFile, lpszSource);
		return true;
	}

	for(;u<strlen(lpszSource);u++){
		Ascii=isJMS(lpszSource+u,Ascii);
		if((lpszSource[u]=='\\'||lpszSource[u]=='/')&&!Ascii)
			t=u+1;
	}

	strcpy(lpszFile,lpszSource+t);
	return true;
}

bool GetShorten(LPSTR ret, LPCSTR source, LPCSTR shortener){
	if(!ret||!source||!shortener) return false;
	strncpy(ret,source,min(strlen(source),strlen(shortener))+1);
	ret[min(strlen(source),strlen(shortener))]=0;
	return true;
}

int strcmpu(LPCSTR lpszStr1, LPCSTR lpszStr2){
	int Ascii=0; UINT i=0;
	if(!lpszStr1||!lpszStr2)return -1;
	if(strlen(lpszStr1)!=strlen(lpszStr2))return -1;
	for(;i<strlen(lpszStr1);i++){
		Ascii=isJMS(lpszStr1+i,Ascii);
		if(!Ascii){
			if(Upcase(lpszStr1[i])!=Upcase(lpszStr2[i])) return 1;
		}else{
			if(lpszStr1[i]!=lpszStr2[i]) return 1;
		}
	}
	return 0;
}

WORD LastByte(LPCSTR lpszStr){
	WORD ret;
	UINT i=0;
	for(;i<strlen(lpszStr);){
		ret=jmscode(lpszStr+i);
		i+= (ret>0xff)?2:1;
	}
	return ret;
}

int GetOSType(){
	OSVERSIONINFO osVersionInfo;
	memset(&osVersionInfo,0,sizeof(OSVERSIONINFO));
	osVersionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	if(!GetVersionEx(&osVersionInfo)) return -1;
	switch (osVersionInfo.dwPlatformId){
	case VER_PLATFORM_WIN32s:
		return WIN31;
	case VER_PLATFORM_WIN32_WINDOWS:
		if (osVersionInfo.dwMajorVersion == 4){
			if (osVersionInfo.dwMinorVersion > 89) //ME
                return WINME;
			if (osVersionInfo.dwMinorVersion > 9) //98
				return (osVersionInfo.dwBuildNumber > 2221) ? WIN98SE : WIN98;
			//95
			return (osVersionInfo.dwBuildNumber > 1213) ? WIN95OSR25
					: (osVersionInfo.dwBuildNumber > 1080) ? WIN95OSR2 : WIN95OSR1;
		}
		if (osVersionInfo.dwMajorVersion > 4)
			return WIN9XNEW;
		break;
	case VER_PLATFORM_WIN32_NT:
		if (osVersionInfo.dwMajorVersion == 3)
			return WINNT3;
		if (osVersionInfo.dwMajorVersion == 4)
			return WINNT4;
		if (osVersionInfo.dwMajorVersion == 5){
			return (osVersionInfo.dwMinorVersion == 0) ? WIN2K
					: (osVersionInfo.dwMinorVersion == 1) ? WINXP
					: (osVersionInfo.dwMinorVersion == 2) ? WIN2K3
					: WINLH;
		}
		if (osVersionInfo.dwMajorVersion > 5)
			return WINLH;
		break;
	}
	return -1;
}

void NullMemory(void* buf, DWORD n){
	while(n){*((BYTE*)buf+(--n))=0;}
}


void ErrorMsg(){
	LPVOID lpMsgBuf;
	char szMsg[510];
	sprintf(szMsg,"0x%08X\n",GetLastError());
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,NULL,GetLastError(),0,(LPTSTR) &lpMsgBuf,500,NULL);
	strcat(szMsg,(char*)lpMsgBuf);
	MessageBox( NULL,szMsg, "Debug", MB_OK | MB_ICONINFORMATION );
	LocalFree(lpMsgBuf);
}

//
int* _get_osfhandle(int);
#if defined(__BORLANDC__) || defined(__TURBOC__)
#define FDESC fd
#elif defined(_MSC_VER) || defined(__GNUC__) || defined(__DMC__)
#define FDESC _file
#endif
//

#ifdef FDESC //(^^;;;
bool rewrite(FILE *fp){
	//Delphiのまねです。
	//書き込んでいる最中に先頭をEOFにしたいなあと思った場合に使えます。

	//fpは書き込みが可能であること。

	if(!fp)
		return false;
	fflush(fp);
	rewind(fp);

	if(!SetEndOfFile( (HANDLE)(_get_osfhandle(fp->FDESC)) )){
		ErrorMsg();
		return false;
	}

	return true;
}
#endif

bool IsExecutable(LPCSTR szFileName){
	HANDLE hModule = LoadLibraryEx(szFileName, NULL, LOAD_LIBRARY_AS_DATAFILE);
	if(hModule){
		FreeLibrary(hModule);
		return true;
	}
	return false;
}

bool GetFileInfo(LPCSTR szFileName, WIN32_FIND_DATA *pFind){
	if(!szFileName||!pFind) return false;

	{//FindFirstで検索(ディレクトリはNT5以上でしかCreateFileできない)
		HANDLE hFind = FindFirstFile(szFileName, pFind);
		if(hFind!=INVALID_HANDLE_VALUE){
			FindClose(hFind);
			return true;
		}
	}

	{//CreateFile/Get*で検索(副ストリーム用)
		HANDLE hFile = CreateFile( szFileName, GENERIC_READ, FILE_SHARE_READ|FILE_SHARE_WRITE,
								NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
		if(hFile==INVALID_HANDLE_VALUE)
			return false;

		pFind->nFileSizeLow = GetFileSize(hFile, &pFind->nFileSizeHigh);
		if((pFind->nFileSizeLow==INVALID_FILE_SIZE)&&(GetLastError())){
			ErrorMsg();
			CloseHandle(hFile);
			return false;
		}

		if(!GetFileTime(hFile, &pFind->ftCreationTime, &pFind->ftLastAccessTime, &pFind->ftLastWriteTime)){
			ErrorMsg();
			CloseHandle(hFile);
			return false;
		}

		if(!(pFind->dwFileAttributes=GetFileAttributes(szFileName))){
			ErrorMsg();
			CloseHandle(hFile);
			return false;
		}

		GetFileName(pFind->cFileName, szFileName); //WIN32_FIND_DATAとして使い物にならないのは…
		CloseHandle(hFile);
		return true;
	}

	return false; //実際は実行されないが。。。
}

bool GetCompletePath(LPCSTR FileName, DWORD PathNameSize, LPSTR PathName){
	char *szFile;

	if(!FileName||!PathName)
		return false;
	GetFullPathName(FileName, PathNameSize, PathName, &szFile);

	{//FileNameのファイル名部分をszFileに入れる
		char szFName[513];
		GetFileName(szFName, FileName);
		strcpy(szFile, szFName);
	}

	return true;
}

void ReverseBytes(void *pDest, void *pIn, DWORD dwLen){
	//かならずpDestの開始位置を調整してください。
	//特にポインタでないものを参照渡しするとき。
	//ex. pDestがint*でdwLenが2の時は、pDestを6つずらさなければなりません。

	while(dwLen--)
		*( ((BYTE*)pDest)++ ) = ((BYTE*)pIn)[dwLen];
}
