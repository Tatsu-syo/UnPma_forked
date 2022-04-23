//
// Altrnate.cpp
// 副ストリームの検索
//
// $Id: Altrnate.cpp,v 1.3 2005/10/30 04:43:28 halpussy Exp $
//

#include "../HALsdkCp.h"

typedef BOOL (WINAPI*BackupRead_proc)(HANDLE hFile,LPBYTE lpBuffer,DWORD nNumberOfBytesToRead,LPDWORD lpNumberOfBytesRead,BOOL bAbort,BOOL bProcessSecurity,LPVOID *lpContext);
typedef BOOL (WINAPI*BackupSeek_proc)(HANDLE hFile,DWORD dwLowBytesToSeek,DWORD dwHighBytesToSeek,LPDWORD lpdwLowByteSeeked,LPDWORD lpdwHighByteSeeked,LPVOID* lpContext);

//#define TEST

// Thx HyperBeat, to make this function be able to be used in any windows.

//副ストリームの検索。
//BackupReadはNT3.1以上でしか使用できないので注意。
//※この関数は全てのWindowsで使用できます。
//あくまで名前の列挙しかしないので、その他の情報を得るには
//このコードを書き換えるか、もしくは列挙された名前それぞれに対し
//ハンドルを取得すること。

// NT3.1以降で無い場合はエラーを返すように変更。
// BackupReadの無い環境では落ちる可能性があるので、KERNEL32.DLLからアドレスを取得して実行するように変更。
bool EnumAlternativeStream(Strings &sRet, LPCSTR lpszFileName){
	HANDLE	hStream = CreateFile(lpszFileName,GENERIC_READ,FILE_SHARE_READ,NULL,
				OPEN_EXISTING,FILE_FLAG_BACKUP_SEMANTICS,NULL);
	BYTE	Buf[sizeof(WIN32_STREAM_ID)+1024];
	DWORD	dwStreamHeaderSize = sizeof(Buf);
	LPVOID	pContext=NULL;
	BOOL	bContinue=TRUE;
	DWORD	dwRead=0;

	BackupRead_proc pBackupRead;
	BackupSeek_proc pBackupSeek;

	if(hStream==INVALID_HANDLE_VALUE) return FALSE;

	if(GetOSType()<WINNT3){
		CloseHandle(hStream);
		return false;
	}

	if( !(pBackupRead=(BackupRead_proc)GetProcAddress(GetModuleHandle("kernel32"),"BackupRead"))
		|| !(pBackupSeek=(BackupSeek_proc)GetProcAddress(GetModuleHandle("kernel32"),"BackupSeek"))
	){
		CloseHandle(hStream);
		return false;
	}

	for(;;){
		if(!pBackupRead(hStream,Buf,dwStreamHeaderSize,&dwRead,FALSE,FALSE,&pContext)){
			CloseHandle(hStream);
			return false;
		}

		if(dwRead==0) break;

		{ //Enum.
			WIN32_STREAM_ID *sid=(WIN32_STREAM_ID*)Buf;

			while( (DWORD)( (BYTE*)sid-Buf ) < dwRead ){
				if(sid->dwStreamNameSize>0&&sid->dwStreamId==BACKUP_ALTERNATE_DATA){
					char* pAltStreamName=(char*)malloc(sid->dwStreamNameSize);
					char *pStreamName;
					NullMemory(pAltStreamName, sid->dwStreamNameSize);

					WideCharToMultiByte(CP_ACP,0,sid->cStreamName,sid->dwStreamNameSize/sizeof(WCHAR),
								pAltStreamName,sid->dwStreamNameSize,NULL,NULL);
					//後ろから6文字目をNULLにする。
					//Filename:Stream:$DATA\0という形式なので、2つ目の:以降は不要。
					//なくてもアクセスすることは可能。
					pAltStreamName[(strlen(pAltStreamName))-6]=0;
					pStreamName=(char*)malloc(strlen(lpszFileName)+strlen(pAltStreamName));
					strcpy(pStreamName,lpszFileName);
					strcat(pStreamName,pAltStreamName);
					if(!sRet.Duplicate(pStreamName,0))
						sRet.Add(pStreamName);
					free(pAltStreamName);
					free(pStreamName);//sRet側で別にメモリを確保しているので問題なし
				}

				//次のストリームを検索する。
				sid = (WIN32_STREAM_ID*)
						( (BYTE*)sid + 
							sizeof(WIN32_STREAM_ID) - sizeof(WCHAR*) +
							sid->dwStreamNameSize + // offset for name
							sid->Size.QuadPart );   // offset for data
			}
		}

		if(dwRead==dwStreamHeaderSize){
			DWORD dw0=0,dw1=0;
			if(!pBackupSeek(hStream,~0,~0,&dw0,&dw1,&pContext)){
				DWORD dwErr=GetLastError();
				if(dwErr==ERROR_INVALID_HANDLE) break; //EOF
				if(dwErr==ERROR_SEEK) continue;
				CloseHandle(hStream);
				return false;
			}
		}
	}

	//Finishing.
	if(!pBackupRead(hStream,NULL,0,&dwRead,TRUE,FALSE,&pContext))
		CloseHandle(hStream);
	return TRUE;
}

#ifdef TEST

void main(int argc, char **argv){
	Strings Strs;
	for(int i=1;i<argc;i++)
		EnumAlternativeStream(Strs,argv[i]);

	for(UINT u=0;u<Strs.length();u++)
		printf("%s\n",Strs[u]);
}

#endif
