//
// CCommand.cpp
// コマンド送信APIの実装
//
// $Id: CCommand.cpp,v 1.13 2005/09/19 15:03:49 halpussy Exp $
//

#include "Classes.h"

CreateDialogProc(CCommand_ConfirmProc, CCommand, ConfirmProc)
CreateDialogProc(CCommand_ProgressProc, CCommand, ProgressProc)

CCommand::CCommand(const CMain *main, const HWND hWnd, LPCSTR lpszCmd, LPSTR lpszOut, const DWORD dwSize,
		const OWNERWINDOWINFO Owner, const MEMBERSPROCINFO Members, const THREADINFO ThreadInfo) :
	baseCCommand(main, hWnd, lpszCmd, lpszOut, dwSize, Owner, Members, ThreadInfo)
{}

BOOL WINAPI UnPmaCallback(void *pParam, INT64 llSize){
	CCommand *pCom = (CCommand*)pParam;
	pCom->mnState = ARCEXTRACT_INPROCESS;
	pCom->mExInfo.llWriteSize = llSize;
	return pCom->ArcProc();
}

bool CCommand::ArchiverProc(){
	DoEvents();
	switch(mnState){
		case ARCEXTRACT_BEGIN:{
			char szabb[41];
			char *sz;

			LoadLangString(STID_T_BEGIN, mszBuffer);
			AbbreviationPath(mExInfo.szSourceFileName, szabb, 40);
			sz = (char*)malloc(strlen(mszBuffer)+strlen(szabb)+1);

			sprintf(sz, "%s%s", mszBuffer, szabb);
			SetDlgItemText(hProgress, IDC_FILE, (LPCTSTR)sz);
			SendDlgItemMessage(hProgress, IDC_PROGRESS1, PBM_SETPOS, 0, 0);
			FreeAndNil(sz);
		}break;
		case ARCEXTRACT_INPROCESS:{
			char *sz;
			LoadLangString(STID_DLG_P_WRITESIZE, mszBuffer);
			sz = (char*)malloc(strlen(mszBuffer)+20);
			sprintf(sz, "%s%I64d/%I64d", mszBuffer, mExInfo.llWriteSize, mExInfo.llFileSize);
			SetDlgItemText(hProgress,IDC_PROGRESS,(LPCTSTR)sz);
			SendDlgItemMessage(hProgress, IDC_PROGRESS1, PBM_SETPOS,
				mExInfo.llFileSize ? (int)( (UINT64)(mExInfo.llWriteSize*100/mExInfo.llFileSize) ) : 0, 0);
			FreeAndNil(sz);
		}break;
		case ARCEXTRACT_END:
		break;
		case ARCEXTRACT_OPEN:{
			char szabb[41];
			char *sz;
			LoadLangString(STID_DLG_P_ARCNAME, mszBuffer);
			AbbreviationPath(mExInfo.szSourceFileName, szabb, 40);
			sz = (char*)malloc(strlen(mszBuffer)+strlen(szabb)+1);

			sprintf(sz, "%s%s", mszBuffer, szabb);
			SetDlgItemText(hProgress, IDC_ARCHIVE, (LPCTSTR)sz);
			FreeAndNil(sz);
		}break;
		case ARCEXTRACT_COPY:
		break;
		case ARCEXTRACT_SEARCH:{
			char szabb[41];
			char *sz;

			LoadLangString(STID_T_SEARCH, mszBuffer);
			AbbreviationPath(mExInfo.szSourceFileName, szabb, 40);
			sz = (char*)malloc(strlen(mszBuffer)+strlen(szabb)+1);

			sprintf(sz, "%s%s", mszBuffer, szabb);
			SetDlgItemText(hProgress, IDC_FILE, (LPCTSTR)sz);
			FreeAndNil(sz);
		}break;
		case ARCEXTRACT_ADDEND:
		break;
		case ARCEXTRACT_CHECK:
		break;
	}
	DoEvents();
	if(mMemberInfo.uCommand == LIST_COMMAND) return true; //∵リスティング時はダイアログを表示しない
	if(IsWindowEnabled(hProgress)) return true;
	{
		LoadLangString(STID_T_CANCEL, mszBuffer);
		if(MessageBox(hProgress,mszBuffer,DLL_NAME,MB_YESNO)==IDYES)
			return false;
		EnableWindow(hProgress, TRUE);
	}
	return true;
}

bool CCommand::MembersProc(){
	if(mMemberInfo.uCommand!=EXTRACT_COMMAND)  return true;
	if(!FileExists(mMemberInfo.szAddFileName)) return true;
	if(wConfirm!=IDYESTOALL&&wConfirm!=IDNOTOALL)
		wConfirm = DialogBoxParam(mThreadInfo.hDll,MAKEINTRESOURCE(IDD_CONFIRM),hProgress,CCommand_ConfirmProc,(LPARAM)this);
	if(wConfirm==IDYESTOALL||wConfirm==IDYES)
		return true;
	else
		return false;
}

int CCommand::Command(){
//MessageBox(wnd,cmd,"Debug",MB_OK);
	int		iAttr=1;
	int		iError=0;
	int		iPath=0;
	int		iTime=1;
	int		iPreserveBroken=0;
	LPSTR	szWorkDir=".";

	showdlg=true;
	wConfirm=IDNO;

	{//解析
		UINT u=1;
		if(cmd.count()<2)
			return ERROR_COMMAND_NAME;
		for(;u<cmd.count();u++){
			if(cmd[u][0]!='-'&&cmd[u][0]!='/')break;
			if(cmd[u][0]==cmd[u][1]){u++;break;}

			switch(Lowercase(cmd[u][1])){
				case 'a':{
					switch(cmd[u][2]){
						case '0': iAttr=0; break;
						default:  iAttr=1; break;
					}
				}break;
				case 'b':{
					switch(cmd[u][2]){
						case '0': iPreserveBroken=0; break;
						default:  iPreserveBroken=1; break;
					}
				}break;
				case 'e':{
					switch(cmd[u][2]){
						case '0': iError=0; break;
						default:  iError=1; break;
					}
				}break;
				case 'f':{
					switch(cmd[u][2]){
						case '0': wConfirm=IDYES; break;
						case '2': wConfirm=IDNOTOALL; break;
						default:  wConfirm=IDYESTOALL; break;
					}
				}break;
				case 'n':{
					switch(cmd[u][2]){
						case '0': showdlg=false; break;
						default:  showdlg=true;  break;
					}
				}break;
				case 'p':{
					switch(cmd[u][2]){
						case '0': iPath=0; break;
						default:  iPath=1; break;
					}
				}break;
				case 't':{
					switch(cmd[u][2]){
						case '0': iTime=0; break;
						default:  iTime=1; break;
					}
				}break;
				case 'w':{
					if(cmd[u][2]) szWorkDir=cmd[u]+2;
				}break;
			}
		}

		if(!cmd[u])
			return ERROR_COMMAND_NAME;
		u++; // inc and go for filelist

		hProgress=::CreateDialogParam(mThreadInfo.hDll,MAKEINTRESOURCE(IDD_PROGRESS),NULL,CCommand_ProgressProc,(LPARAM)this);

		if(Lowercase(cmd[0][0])=='v') cmd[0][0]='c';

		switch(Lowercase(cmd[0][0])){
			case 'l': case 'c':{
				HANDLE hArc;
				INDIVIDUALINFOEX SubInfoEx;
				int i,c=0;

				INT64 llOriginalSize, llCompressedSize;
				WORD wRatio;
				FILETIME ftLocal;
				SYSTEMTIME stWrite;

				NullMemory(&SubInfoEx, sizeof(INDIVIDUALINFOEX));

				mnState = ARCEXTRACT_OPEN;
				strcpy(mExInfo.szSourceFileName, cmd[u-1]);
				if(!ArcProc()){
					DestroyWindow(hProgress);
					return ERROR_USER_CANCEL;
				}

				hArc = UnPmaOpenArchive(hProgress, cmd[u-1], 0);
				if(!hArc){
					DestroyWindow(hProgress);
					return ERROR_FILE_STYLE;
				}
///
bf.Write("Filename        Original    Packed     Ratio  Date     Time     Attr Type  CRC\n");
bf.Write("==============  ==========  ========== ====== ======== ======== ==== ===== ====\n");
///

				i=UnPmaFindFirstEx(hArc,"*",&SubInfoEx);
				while(!i){
					bool b=true, bFilter=false;
					c++;
					if(u<cmd.count()){
						UINT u0=u;
						for(;u0<cmd.count();u0++){
							if(LastByte(cmd[u0])=='\\')continue;
							bFilter=true;
							if(IsMatchWildcard2(cmd[u0], SubInfoEx.szFileName,
										iPath ? WILD_MODE_SAMEDIRECTORY : WILD_MODE_STRING))break;
						}
						if(bFilter&&u0==cmd.count())
							b=false;
					}

					if(b){
						mnState = ARCEXTRACT_BEGIN;
						strcpy(mExInfo.szSourceFileName, SubInfoEx.szFileName);
						strcpy(mExInfo.szMode, SubInfoEx.szMode);
						mExInfo.llFileSize = SubInfoEx.llOriginalSize;
						mExInfo.llCompressedSize = SubInfoEx.llCompressedSize;
						mExInfo.dwAttributes = SubInfoEx.dwAttributes;
						mExInfo.dwCRC = SubInfoEx.dwCRC;
						mExInfo.uOSType = SubInfoEx.uOSType;
						mExInfo.wRatio = SubInfoEx.wRatio;
						mExInfo.ftCreateTime = SubInfoEx.ftCreateTime;
						mExInfo.ftWriteTime = SubInfoEx.ftWriteTime;
						mExInfo.ftAccessTime = SubInfoEx.ftAccessTime;
						if(!ArcProc()){
							UnPmaCloseArchive(hArc);
							DestroyWindow(hProgress);
							return ERROR_USER_CANCEL;
						}

						mMemberInfo.uCommand = LIST_COMMAND;
						strcpy(mMemberInfo.szFileName, SubInfoEx.szFileName);
						strcpy(mMemberInfo.szAddFileName, SubInfoEx.szFileName);
						mMemberInfo.llOriginalSize = SubInfoEx.llOriginalSize;
						mMemberInfo.llCompressedSize = SubInfoEx.llCompressedSize;
						mMemberInfo.dwAttributes = SubInfoEx.dwAttributes;
						mMemberInfo.dwCRC = SubInfoEx.dwCRC;
						mMemberInfo.uOSType = SubInfoEx.uOSType;
						mMemberInfo.wRatio = SubInfoEx.wRatio;
						mMemberInfo.ftCreateTime = SubInfoEx.ftCreateTime;
						mMemberInfo.ftWriteTime = SubInfoEx.ftWriteTime;
						mMemberInfo.ftAccessTime = SubInfoEx.ftAccessTime;
						if(MembProc()){
							char szFileName[513];
							char szAttr[5];

							if(Lowercase(cmd[0][0])=='l')
								GetFileName(szFileName, SubInfoEx.szFileName);
							else
								strcpy(szFileName, SubInfoEx.szFileName);

							{
								szAttr[0] = SubInfoEx.dwAttributes&FA_DIREC ? 'd' :
									SubInfoEx.dwAttributes&FA_ARCH ? 'a' : '-';
								szAttr[1] = SubInfoEx.dwAttributes&FA_SYSTEM ? 's' : '-';
								szAttr[2] = SubInfoEx.dwAttributes&FA_HIDDEN ? 'h' : '-';
								szAttr[3] = SubInfoEx.dwAttributes&FA_RDONLY ? 'r' : 'w';
								szAttr[4] = 0;
							}

							FileTimeToLocalFileTime(&mMemberInfo.ftWriteTime, &ftLocal);
							FileTimeToSystemTime(&ftLocal, &stWrite);

							FixPathDirect(szFileName);
///
bf.Print( (Lowercase(cmd[0][0])=='l')
			? "%-14s %11I64d %11I64d %3d.%d%% %02d/%02d/%02d %02d:%02d:%02d %s %s %04X\n"
			: "%s\n               %11I64d %11I64d %3d.%d%% %02d/%02d/%02d %02d:%02d:%02d %s %s %04X\n",
		szFileName, SubInfoEx.llOriginalSize, SubInfoEx.llCompressedSize,
		SubInfoEx.wRatio/10, SubInfoEx.wRatio%10,
		stWrite.wYear%100, stWrite.wMonth, stWrite.wDay, stWrite.wHour, stWrite.wMinute, stWrite.wSecond,
		szAttr, SubInfoEx.szMode, SubInfoEx.dwCRC);

if(Lowercase(cmd[0][0])=='c'&&UnPmaGetMemo(hArc,NULL,0)){
	char szMemo[75];
	UnPmaGetMemo(hArc, szMemo, 75);
	bf.Print(" %s\n",szMemo);
}
///
						}
					}

					i=UnPmaFindNextEx(hArc, &SubInfoEx);
				}
				if(i!=-1)
					SetWarning(i);

				{
					FILETIME ftWrite;
					UnPmaGetArcOriginalSizeEx(hArc, &llOriginalSize);
					UnPmaGetArcCompressedSizeEx(hArc, &llCompressedSize);
					wRatio=UnPmaGetArcRatio(hArc);
					UnPmaGetArcWriteTimeEx(hArc, &ftWrite);
					FileTimeToLocalFileTime(&ftWrite, &ftLocal);
					FileTimeToSystemTime(&ftWrite, &stWrite);
///
bf.Write("==============  ==========  ========== ====== ======== ======== ==== ===== ====\n");
bf.Print("%8d files %11I64d %11I64d %3d.%d%% %02u/%02u/%02u %02u:%02u:%02u\n",
		c, llOriginalSize, llCompressedSize, wRatio/10, wRatio%10,
		stWrite.wYear%100, stWrite.wMonth, stWrite.wDay, stWrite.wHour, stWrite.wMinute, stWrite.wSecond);
///
				}

				UnPmaCloseArchive(hArc);
			}break;
			case 'e': case 'x':{
				PmaStream UnPma;
				INDIVIDUALINFOEX SubInfoEx;
				LPSTR szDir=NULL;

				if(showdlg&&!mOwner.iType){
					CenteringWindow(mhWnd,hProgress);
					::ShowWindow(hProgress,SW_SHOW);
					DoEvents();
				}

				{
					UINT u0=u;
					for(;u0<cmd.count();u0++){
						if(LastByte(cmd[u0])=='\\') szDir=cmd[u0];
					}
				}

				mnState = ARCEXTRACT_OPEN;
				strcpy(mExInfo.szSourceFileName, cmd[u-1]);
				if(!ArcProc()){
					DestroyWindow(hProgress);
					return ERROR_USER_CANCEL;
				}

				if(!UnPma.Open(cmd[u-1])){
					DestroyWindow(hProgress);
					return ERROR_SHARING;
				}
				if(!UnPma.SeekTop()){
					DestroyWindow(hProgress);
					return ERROR_FILE_STYLE;
				}

				UnPma.SetCallback(this, UnPmaCallback);

				while(UnPma.ReadHeader(&SubInfoEx)){
					bool b=true, bFilter=false;

					if(u<cmd.count()){
						UINT u0=u;
						for(;u0<cmd.count();u0++){
							if(LastByte(cmd[u0])=='\\')continue;
							bFilter=true;
							if(IsMatchWildcard2(cmd[u0], SubInfoEx.szFileName,
										iPath ? WILD_MODE_SAMEDIRECTORY : WILD_MODE_STRING))break;
						}
						if(bFilter&&u0==cmd.count())
							b=false;
					}

					if(b){
						char szFileName[513];
						NullMemory(szFileName,513);
						if(szDir)
							strcpy(szFileName,szDir);
						if(Lowercase(cmd[0][0])=='e')
							GetFileName(szFileName+strlen(szFileName), SubInfoEx.szFileName);
						else
							strcat(szFileName, SubInfoEx.szFileName);

						if(memcmp(SubInfoEx.szMode, "-pm2-", 5)&&memcmp(SubInfoEx.szMode, "-pm0-", 5)){
							bf.Print("Unknown Method %s: %s\n",SubInfoEx.szMode,SubInfoEx.szFileName);
							if(iError){
								bf.Write("Error occurred, thus stopped.\n");
								DestroyWindow(hProgress);
								return ERROR_UNKNOWN_TYPE;
							}
							SetWarning(ERROR_UNKNOWN_TYPE);
							continue;
						}

						mnState = ARCEXTRACT_BEGIN;
						strcpy(mExInfo.szSourceFileName, SubInfoEx.szFileName);
						strcpy(mExInfo.szDestFileName, szFileName);
						strcpy(mExInfo.szMode, SubInfoEx.szMode);
						mExInfo.llFileSize = SubInfoEx.llOriginalSize;
						mExInfo.llCompressedSize = SubInfoEx.llCompressedSize;
						mExInfo.dwAttributes = SubInfoEx.dwAttributes;
						mExInfo.dwCRC = SubInfoEx.dwCRC;
						mExInfo.uOSType = SubInfoEx.uOSType;
						mExInfo.wRatio = SubInfoEx.wRatio;
						mExInfo.ftCreateTime = SubInfoEx.ftCreateTime;
						mExInfo.ftWriteTime = SubInfoEx.ftWriteTime;
						mExInfo.ftAccessTime = SubInfoEx.ftAccessTime;
						if(!ArcProc()){
							DestroyWindow(hProgress);
							return ERROR_USER_CANCEL;
						}

						mMemberInfo.uCommand = EXTRACT_COMMAND;
						strcpy(mMemberInfo.szFileName, SubInfoEx.szFileName);
						strcpy(mMemberInfo.szAddFileName, szFileName);
						mMemberInfo.llOriginalSize = SubInfoEx.llOriginalSize;
						mMemberInfo.llCompressedSize = SubInfoEx.llCompressedSize;
						mMemberInfo.dwAttributes = SubInfoEx.dwAttributes;
						mMemberInfo.dwCRC = SubInfoEx.dwCRC;
						mMemberInfo.uOSType = SubInfoEx.uOSType;
						mMemberInfo.wRatio = SubInfoEx.wRatio;
						mMemberInfo.ftCreateTime = SubInfoEx.ftCreateTime;
						mMemberInfo.ftWriteTime = SubInfoEx.ftWriteTime;
						mMemberInfo.ftAccessTime = SubInfoEx.ftAccessTime;
						if(MembProc()){//解凍!
							FixPathDirect(mMemberInfo.szAddFileName);
							MakeDirectory(mMemberInfo.szAddFileName);
							bf.Print("Extracting: %s\n", SubInfoEx.szFileName);
							if(!UnPma.Extract(mMemberInfo.szAddFileName)){
								DWORD dwErr=UnPma.GetLastError();
								if(dwErr==ERROR_FILE_CRC){
									bf.Print("CRC Error: %s\n",SubInfoEx.szFileName);
									if(!iPreserveBroken)
										DeleteFile(mMemberInfo.szAddFileName);
								}
								if(dwErr==ERROR_FILE_OPEN)
									bf.Print("File-Open Error: %s\n",SubInfoEx.szFileName);

								if(iError||(dwErr!=ERROR_FILE_OPEN&&dwErr!=ERROR_FILE_CRC)){
									if(dwErr==ERROR_USER_CANCEL)
										bf.Write("User canceled.\n");
									else
										bf.Write("Error occurred, thus stopped.\n");
									DestroyWindow(hProgress);
									return (int)dwErr;
								}
								SetWarning(dwErr);
								continue;
							}
							if(iAttr)
								SetFileAttributes(mMemberInfo.szAddFileName, mMemberInfo.dwAttributes);
							if(iTime){
								HANDLE hFile;
								FILETIME ftLocal;
								SYSTEMTIME st;

								//もともとのローカルタイムに復帰
								FileTimeToLocalFileTime(&mMemberInfo.ftWriteTime, &ftLocal);

								//更新日時は正しいか
								FileTimeToSystemTime(&ftLocal, &st);
								if(st.wYear==1980&&st.wMonth==1&&st.wDay==1&&
									st.wHour==0&&st.wMinute==0&&st.wSecond==0)continue;

								hFile = CreateFile(mMemberInfo.szAddFileName, GENERIC_WRITE,
												FILE_SHARE_READ|FILE_SHARE_WRITE, NULL,
												OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
								if(hFile!=INVALID_HANDLE_VALUE){
									SetFileTime(hFile, NULL, NULL, &ftLocal);
									CloseHandle(hFile);
								}
							}
						}
					}
				}
				if(UnPma.GetLastError())
					SetWarning(UnPma.GetLastError());
			}break;
			case 's':{
				PmaStream UnPma;
				TempFile Temp;

				LPSTR szBuffer;
				DWORD dwRead;

				if(!Between(1,cmd.count()-u,2)){
					DestroyWindow(hProgress);
					return ERROR_COMMAND_NAME;
				}

				mnState = ARCEXTRACT_OPEN;
				strcpy(mExInfo.szSourceFileName, cmd[cmd.count()-2]);
				if(!ArcProc()){
					DestroyWindow(hProgress);
					return ERROR_USER_CANCEL;
				}

				if(!UnPma.Open(cmd[cmd.count()-2])){
					DestroyWindow(hProgress);
					return ERROR_SHARING;
				}
				if(!UnPma.SeekTop()){
					DestroyWindow(hProgress);
					return ERROR_FILE_STYLE;
				}

				if(wConfirm!=IDYESTOALL&&FileExists(cmd[cmd.count()-1])){
					LPSTR sz;

					if(wConfirm==IDNOTOALL){
						DestroyWindow(hProgress);
						return ERROR_ALREADY_EXIST;
					}

					LoadLangString(STID_MES_P_OVERWRITE, mszBuffer);
					sz = (char*)malloc( strlen(mszBuffer)+strlen(cmd[cmd.count()-1])+1 );
					sprintf(sz,mszBuffer,cmd[cmd.count()-1]);
					if(MessageBox(hProgress,(LPCTSTR)sz,"上書き確認",MB_YESNO)==IDNO){
						DestroyWindow(hProgress);
						return ERROR_ALREADY_EXIST;
					}
				}

				if(!(szBuffer=(LPSTR)malloc(0x10000))){
					DestroyWindow(hProgress);
					return ERROR_ENOUGH_MEMORY;
				}

				if(!Temp.Create("pma", szWorkDir)){
					DestroyWindow(hProgress);
					FreeAndNil(szBuffer);
					return ERROR_SHARING;
				}

				if(cmd.count()-u==2){
					//sfxname is specified
					char szSfx[MAX_PATH+1];
					HANDLE hFile;

					if( !SearchPath( NULL, cmd[u-1], NULL, MAX_PATH, szSfx, NULL ) ){
						DestroyWindow(hProgress);
						FreeAndNil(szBuffer);
						return ERROR_NOT_FIND_FILE;
					}

					hFile = CreateFile2(szSfx, GENERIC_READ, FILE_SHARE_READ, OPEN_EXISTING,
								FILE_ATTRIBUTE_NORMAL|FILE_FLAG_SEQUENTIAL_SCAN);
					if(hFile==INVALID_HANDLE_VALUE){
						DestroyWindow(hProgress);
						FreeAndNil(szBuffer);
						return ERROR_SHARING;
					}

					while( ReadFile(hFile,szBuffer,0x10000,&dwRead,NULL) ){
						if(!dwRead)break;
						WriteFile((HANDLE)Temp,szBuffer,dwRead,&dwRead,NULL);
					}
					CloseHandle(hFile);
				}else{
					HRSRC	hRsrc;
					HGLOBAL	hRes;
					void	*pSFX;
					DWORD	dwSize;

					if( !(hRsrc = FindResource(mThreadInfo.hDll,MAKEINTRESOURCE(IDM_SFX),"MODULE"))
						||!(hRes = LoadResource(mThreadInfo.hDll,hRsrc))
						||!(pSFX = (LPWSTR)LockResource(hRes))
					){
						DestroyWindow(hProgress);
						FreeAndNil(szBuffer);
						return ERROR_UNKNOWN;
					}

					dwSize = SizeofResource(mThreadInfo.hDll,hRsrc);
					WriteFile((HANDLE)Temp,pSFX,dwSize,&dwSize,NULL);
				}

				while( ReadFile((HANDLE)UnPma,szBuffer,0x10000,&dwRead,NULL) ){
					if(!dwRead)break;
					WriteFile(HANDLE(Temp),szBuffer,dwRead,&dwRead,NULL);
				}

				FreeAndNil(szBuffer);

				mnState = ARCEXTRACT_COPY;
				strcpy(mExInfo.szSourceFileName, (LPCSTR)Temp);
				if(!ArcProc()){
					DestroyWindow(hProgress);
					return ERROR_USER_CANCEL;
				}

				Temp.Done(cmd[cmd.count()-1]);
			}break;
			case 'u':{
				PmaStream UnPma;
				HANDLE hFile;

				LPSTR szBuffer;
				DWORD dwRead;

				if(cmd.count()-u!=1){
					DestroyWindow(hProgress);
					return ERROR_COMMAND_NAME;
				}

				mnState = ARCEXTRACT_OPEN;
				strcpy(mExInfo.szSourceFileName, cmd[u-1]);
				if(!ArcProc()){
					DestroyWindow(hProgress);
					return ERROR_USER_CANCEL;
				}

				if(!UnPma.Open(cmd[u-1])){
					DestroyWindow(hProgress);
					return ERROR_SHARING;
				}
				if(!UnPma.SeekTop()){
					DestroyWindow(hProgress);
					return ERROR_FILE_STYLE;
				}

				if(wConfirm!=IDYESTOALL&&FileExists(cmd[u])){
					LPSTR sz;

					if(wConfirm==IDNOTOALL){
						DestroyWindow(hProgress);
						return ERROR_ALREADY_EXIST;
					}

					LoadLangString(STID_MES_P_OVERWRITE, mszBuffer);
					sz = (char*)malloc( strlen(mszBuffer)+strlen(cmd[u])+1 );
					sprintf(sz,mszBuffer,cmd[u]);
					if(MessageBox(hProgress,(LPCTSTR)sz,"上書き確認",MB_YESNO)==IDNO){
						DestroyWindow(hProgress);
						return ERROR_ALREADY_EXIST;
					}
				}

				if(!(szBuffer=(LPSTR)malloc(0x10000))){
					DestroyWindow(hProgress);
					return ERROR_ENOUGH_MEMORY;
				}

				hFile = CreateFile2(cmd[u], GENERIC_WRITE, FILE_SHARE_READ, CREATE_ALWAYS,
								FILE_ATTRIBUTE_NORMAL|FILE_FLAG_SEQUENTIAL_SCAN);
				if(hFile==INVALID_HANDLE_VALUE){
					DestroyWindow(hProgress);
					FreeAndNil(szBuffer);
					return ERROR_SHARING;
				}

				while( ReadFile((HANDLE)UnPma,szBuffer,0x10000,&dwRead,NULL) ){
					if(!dwRead)break;
					WriteFile(hFile,szBuffer,dwRead,&dwRead,NULL);
				}

				FreeAndNil(szBuffer);
			}break;
/*
			case 'j':{
				PmaStream UnPma;
				DWORD dw = u;
				bool bPMA = false;

				if(!cmd[u]){
					DestroyWindow(hProgress);
					return ERROR_COMMAND_NAME;
				}

				if(showdlg&&!mOwner.iType){
					CenteringWindow(mhWnd,hProgress);
					::ShowWindow(hProgress,SW_SHOW);
					DoEvents();
				}

				mnState = ARCEXTRACT_OPEN;
				strcpy(mExInfo.szSourceFileName, cmd[u-1]);
				if(!ArcProc()){
					DestroyWindow(hProgress);
					return ERROR_USER_CANCEL;
				}

				if(UnPma.Open(cmd[u-1])){
					if(!UnPma.SeekTop()){
						strcpy(mMemberInfo.szAddFileName, mExInfo.szSourceFileName, cmd[u-1]);
						mMemberInfo.uCommand = EXTRACT_COMMAND;

						if(!MembersProc()){
							DestroyWindow(hProgress);
							return ERROR_ALREADY_EXIST;
						}
					}else
						bPMA = true;
					UnPma.Close();
				}

				{//Unite...
					HANDLE hArchive = FileOpenWriteSequential(cmd[u-1], bPMA ? OPEN_EXISTING : CREATE_ALWAYS);										FILE_FLAG_SEQUENTIAL_SCAN);
					HANDLE hFile;
					WIN32_FIND_DATA Find;
					NullMemory(&Find, sizeof(WIN32_FIND_DATA));

					if(hArchive==INVALID_HANDLE_VALUE){
						DestroyWindow(hProgress);
						return ERROR_SHARING;
					}

					if(bPMA)SetFilePointeri64(GetFileSizei64()-1);

					for(;dw<cmd.count();dw++){
						if(!UnPmaCheckArchive(cmd[dw],0)){
							bf.Print("Not an archive: %s", cmd[dw]);
							if(iError){
								CloseHandle(hArchive);
								DestroyWindow(hProgress);
								return ERROR_FILE_STYLE;
							}
							SetWarning(ERROR_FILE_STYLE);
							continue;
						}

						GetFileInfo(cmd[dw], &Find);

						mnState = ARCEXTRACT_BEGIN;
						strcpy(mExInfo.szSourceFileName, Find.szFileName);
						strcpy(mExInfo.szDestFileName, Find.szFileName);
						strcpy(mExInfo.szMode, "-pma-");
						mExInfo.llFileSize = MAKELONGLONG(Find.nFileSizeLow, Find.nFileSizeHigh);
						mExInfo.llCompressedSize = 0; //(^^;;;
						mExInfo.dwAttributes = Find.dwAttributes;
						mExInfo.dwCRC = 0;
						mExInfo.uOSType = OSTYPE_CPM;
						mExInfo.wRatio = 0;
						mExInfo.ftCreateTime = Find.ftCreationTime;
						mExInfo.ftWriteTime = Find.ftLastWriteTime;
						mExInfo.ftAccessTime = Find.ftLastAccessTime;
						if(!ArcProc()){
							CloseHandle(hArchive);
							DestroyWindow(hProgress);
							return ERROR_USER_CANCEL;
						}

						mMemberInfo.uCommand = JOINT_COMMAND;
						strcpy(mMemberInfo.szFileName, Find.szFileName);
						strcpy(mMemberInfo.szAddFileName, Find.szFileName);
						mMemberInfo.llOriginalSize = MAKELONGLONG(Find.nFileSizeLow, Find.nFileSizeHigh);
						mMemberInfo.llCompressedSize = 0;
						mMemberInfo.dwAttributes = Find.dwAttributes;
						mMemberInfo.dwCRC = 0;
						mMemberInfo.uOSType = OSTYPE_CPM;
						mMemberInfo.wRatio = 0;
						mMemberInfo.ftCreateTime = Find.ftCreationTime;
						mMemberInfo.ftWriteTime = Find.ftLastWriteTime;
						mMemberInfo.ftAccessTime = Find.ftLastAccessTime;
						if(!MembProc()) continue;

						hFile = CreateFile(cmd[dw], GENERIC_READ, FILE_SHARE_READ, NULL,
										OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, FILE_FLAG_SEQUENTIAL_SCAN, NULL);

					}
				}
				if(UnPma.GetLastError())
					iRet=UnPma.GetLastError();
			}break;
*/
		}
	}

	DestroyWindow(hProgress);
	return 0;
}

BOOL CCommand::ProgressProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
	switch(uMsg){
		case WM_INITDIALOG:{
			//Init

			//Dialog
			SetFont(hWnd, 10, ANSI_CHARSET, LoadLangString(STID_FONTPGOTICK, mszBuffer));
			SetIcon(hWnd, LoadIcon(mThreadInfo.hDll,MAKEINTRESOURCE(IDI_ICON)));
			EnableVisualTexture(hWnd);
			BeginCursorSubClass(hWnd);

			//Finalize
			return TRUE;
		}break;
		case WM_COMMAND:
			if(wParam!=IDCANCEL) break;
		case WM_CLOSE:{
			EnableWindow(hWnd, FALSE);
			return TRUE;
		}
		case WM_SETCURSOR:{
			if(mThreadInfo.bCur&&mbChangeCursor&&IsWindowEnabled(hWnd)&&!IsIconic(hWnd)){
				SetCursor(LoadCursor(mThreadInfo.hDll,MAKEINTRESOURCE(IDC_CURSOR)));
				SetSubClassCursor(hWnd, LoadCursor(mThreadInfo.hDll,MAKEINTRESOURCE(IDC_CURSOR)));
				return TRUE; //DefWindowProcを切る
			}else
				SetSubClassCursor(hWnd, NULL);
		}break;
		case WM_DESTROY:{
			//サブクラス化を解除
			EndCursorSubClass(hWnd);
		}break;
	}
	return FALSE;
}

BOOL CCommand::ConfirmProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
	switch(uMsg){
		case WM_INITDIALOG:{
			//Init
			char *sz;

			//Dialog
			CenteringWindow(mhWnd,hWnd);
			SetFont(hWnd, 10, ANSI_CHARSET, LoadLangString(STID_FONTPGOTICK, mszBuffer));
			EnableVisualTexture(hWnd);

			//String
			LoadLangString(STID_MES_P_OVERWRITE, mszBuffer);
			sz = (char*)malloc( strlen(mszBuffer)+strlen(mMemberInfo.szAddFileName)+1 );
			sprintf(sz,mszBuffer,mMemberInfo.szAddFileName);
			SetDlgItemText(hWnd,IDC_FILE,(LPCTSTR)sz);
			free(sz);

			//Finalize
			DoEvents();
			return FALSE;
		}break;
		case WM_COMMAND:
			if(wParam!=IDYES&&wParam!=IDNO&&wParam!=IDYESTOALL&&wParam!=IDNOTOALL) break;
		case WM_CLOSE:{
			switch(wParam){
				case IDYES: ::EndDialog(hWnd,IDYES); break;
				case IDNO: ::EndDialog(hWnd,IDNO); break;
				case IDYESTOALL: ::EndDialog(hWnd,IDYESTOALL); break;
				case IDNOTOALL: ::EndDialog(hWnd,IDNOTOALL); break;
			}
			return TRUE;
		}break;
	}
	return FALSE;
}
