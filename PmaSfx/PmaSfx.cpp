//PmaSfx.exe
//PMA SelF eXtractor

#include "PmaSfx.h"
CreateDialogProc(PmaSfx_DlgProc, PmaSfx, DlgProc)
CreateDialogProc(PmaSfx_ConfirmProc, PmaSfx, ConfirmProc)

BOOL WINAPI UnPmaCallback(void *pParam, INT64 llSize){
	PmaSfx *pSfx = (PmaSfx*)pParam;
	char sz[30];

	sprintf(sz, "%I64d/%I64d", llSize, pSfx->llSize);
	SetDlgItemText(pSfx->hWindow,IDC_PROGRESS,(LPCTSTR)sz);
	SendDlgItemMessage(pSfx->hWindow, IDC_PROGRESS1, PBM_SETPOS,
			pSfx->llSize ? (int)( (UINT64)(llSize*100/pSfx->llSize) ) : 0, 0);
	DoEvents();
	return pSfx->bContinue;
}

int CALLBACK BrowseCallbackProc(HWND hWnd,UINT uMsg,LPARAM lParam,LPARAM lpData){
	char Path[MAX_PATH * 2];
	if (((BROWSEINFO*)lpData)->ulFlags & BIF_STATUSTEXT){
		if (uMsg == BFFM_SELCHANGED){
			SHGetPathFromIDList((LPCITEMIDLIST)lParam,Path);
			SendMessage(hWnd,BFFM_SETSTATUSTEXT,0,(LPARAM)Path);
		}
	}
	return 0;
}

bool PmaSfx::DirectoryRefDialog(LPSTR Result){
	bool r = false;
	LPMALLOC Memory;
	LPCSTR Buffer;
	LPITEMIDLIST Ret;
	LPITEMIDLIST Root;
	BROWSEINFO BI;

	SHGetMalloc(&Memory);
	Buffer = (LPSTR)Memory->Alloc(1000);
	SHGetSpecialFolderLocation(hWindow,CSIDL_DESKTOP,&Root);
	NullMemory(&BI,sizeof(BROWSEINFO));
	BI.hwndOwner = hWindow;
	BI.pidlRoot = Root;
	BI.ulFlags = BIF_STATUSTEXT | BIF_RETURNONLYFSDIRS;
	BI.lpszTitle = "展開先を指定してください";
	BI.pszDisplayName = (LPSTR)Buffer;
	BI.lpfn = BrowseCallbackProc;
	BI.lParam = (LPARAM)(&BI);
	Ret = SHBrowseForFolder(&BI);
	if (SHGetPathFromIDList(Ret,(LPSTR)Buffer)){
		lstrcpy(Result,Buffer);
		r = true;
	}
	Memory->Free(Ret);
	Memory->Free(Root);
	Memory->Free((void*)Buffer);
	return r;
}

void PmaSfx::DoExtract(HWND hWnd){
	char szDir[MAX_PATH+1];
	char szFile[MAX_PATH+1];
	char sz[FNAME_MAX32];
	char szabb[41];

	INDIVIDUALINFOEX SubInfoEx;
	int iPath=0;
	WORD wConfirm=IDYES;

	NullMemory(&SubInfoEx, sizeof(INDIVIDUALINFOEX));

	if(BST_CHECKED==::SendDlgItemMessage(hWindow,IDC_USEPATH,BM_GETCHECK,0,0))
		iPath=1;
	if(BST_CHECKED==::SendDlgItemMessage(hWindow,IDC_OVERWRITE,BM_GETCHECK,0,0))
		wConfirm=IDYESTOALL;

	GetDlgItemText(hWindow,IDC_OUTDIR,szDir,MAX_PATH);
	if (szDir[lstrlen(szDir)-1] != '\\'){
		lstrcat(szDir,"\\");
	}

	while(UnPma.ReadHeader(&SubInfoEx)){
		strcpy(szFile, szDir);
		if(!iPath)
			GetFileName(szFile+strlen(szFile), SubInfoEx.szFileName);
		else
			strcat(szFile, SubInfoEx.szFileName);

		if(memcmp(SubInfoEx.szMode, "-pm2-", 5)&&memcmp(SubInfoEx.szMode, "-pm0-", 5)){
			sprintf(sz, "不明なメソッドです %s: %s\n他のメンバについて展開を続行しますか？", SubInfoEx.szMode,SubInfoEx.szFileName);
			if(MessageBox(hWnd, sz, SFXName, MB_YESNO)==IDNO)
				return;
			continue;
		}

		AbbreviationPath(SubInfoEx.szFileName, szabb, 40);
		sprintf(sz, "格納ファイル: %s", szabb);
		SetDlgItemText(hWindow, IDC_FILENAME, (LPCTSTR)sz);
		SendDlgItemMessage(hWindow, IDC_PROGRESS1, PBM_SETPOS, 0, 0);
		DoEvents();

		llSize = SubInfoEx.llOriginalSize;

		FixPathDirect(szFile);
		if(FileExists(szFile)){
			szName = szFile;
			if(wConfirm!=IDYESTOALL&&wConfirm!=IDNOTOALL)
				wConfirm = DialogBoxParam(GetModuleHandle(NULL),MAKEINTRESOURCE(IDD_CONFIRM),hWindow,PmaSfx_ConfirmProc,(LPARAM)this);
			if(wConfirm==IDNOTOALL||wConfirm==IDNO)
				continue;
		}

		MakeDirectory(szFile);
		if(!UnPma.Extract(szFile)){
			DWORD dwErr=UnPma.GetLastError();
			if(dwErr==ERROR_FILE_CRC){
				DeleteFile(szFile);
				sprintf(sz, "CRCエラーです: %s\n他のメンバについて展開を続行しますか？", SubInfoEx.szMode,SubInfoEx.szFileName);
				if(MessageBox(hWnd, sz, SFXName, MB_YESNO)==IDNO)
					return;
			}else if(dwErr==ERROR_FILE_OPEN){
				//ファイルオープンエラーは無視
				//sprintf(sz, "ファイルオープンエラーです: %s\n他のメンバについて展開を続行しますか？", SubInfoEx.szMode,SubInfoEx.szFileName);
				//if(MessageBox(hWnd, sz, SFXName, MB_YESNO)==IDNO)
				//	return;
			}else{ //if(dwErr==ERROR_USER_CANCEL)
				return;
			}
		}

		SetFileAttributes(szFile, SubInfoEx.dwAttributes);

		{
			HANDLE hFile;
			FILETIME ftLocal;
			SYSTEMTIME st;

			//もともとのローカルタイムに復帰
			FileTimeToLocalFileTime(&SubInfoEx.ftWriteTime, &ftLocal);

			//更新日時は正しいか
			FileTimeToSystemTime(&ftLocal, &st);
			if(st.wYear==1980&&st.wMonth==1&&st.wDay==1&&
				st.wHour==0&&st.wMinute==0&&st.wSecond==0)continue;

			hFile = CreateFile2(szFile, GENERIC_WRITE,
						FILE_SHARE_READ|FILE_SHARE_WRITE, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL);
			if(hFile!=INVALID_HANDLE_VALUE){
				SetFileTime(hFile, NULL, NULL, &ftLocal);
				CloseHandle(hFile);
			}
		}
	}
}

BOOL PmaSfx::DlgProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam){
	switch (uMsg){
		case WM_INITDIALOG:{
			char szArcname[MAX_PATH+1];
		    GetModuleFileName(NULL,szArcname,MAX_PATH);
			hWindow = hWnd;

			UnPma.Open(szArcname);
			if(!UnPma.SeekTop()){
				SetDlgItemText(hWnd, IDC_FILENAME, "自己解凍書庫ではありません");
				EnableWindow(GetDlgItem(hWnd,IDC_FINDDIR),FALSE);
				EnableWindow(GetDlgItem(hWnd,IDC_EXTRACT),FALSE);
			}else{
				SetDlgItemText(hWnd,IDC_FILENAME,"PmaSfx (C) HAL \"Minthe\" Pussywillow");
				GetCurrentDirectory(MAX_PATH+1, szArcname);
				SetDlgItemText(hWnd,IDC_OUTDIR, szArcname);
				SendDlgItemMessage(hWnd,IDC_USEPATH,BM_SETCHECK,BST_CHECKED,0);
				UnPma.SetCallback(this, UnPmaCallback);
			}
			return TRUE;
		}
	case WM_SYSCOMMAND:
		if (!bExtract&&wParam==SC_CLOSE){
			UnPma.Close();
			EndDialog(hWnd,TRUE);
			return TRUE;
		}
		return FALSE;
	case WM_COMMAND:
		switch (LOWORD(wParam)){
			case IDC_FINDDIR:
				if (!bExtract){
					char szDir[MAX_PATH * 2];
					if (DirectoryRefDialog(szDir)){
						SetDlgItemText(hWnd,IDC_OUTDIR,szDir);
					}
				}
				return TRUE;
			case IDC_EXTRACT:
				if (!bExtract){
					bExtract = true;
					EnableWindow(GetDlgItem(hWnd,IDC_FINDDIR),FALSE);
					SetDlgItemText(hWnd,IDC_EXTRACT,"キャンセル(&C)");
					//_beginthread(extract_archive,0,(void*)hWnd);
					DoExtract(hWnd);
					UnPma.SeekTop();
					SetDlgItemText(hWnd,IDC_PROGRESS,"");
					SendDlgItemMessage(hWnd, IDC_PROGRESS1, PBM_SETPOS, 0, 0);
					EnableWindow(GetDlgItem(hWnd,IDC_FINDDIR),TRUE);
					SetDlgItemText(hWnd,IDC_EXTRACT,"展開(&E)");
					SetDlgItemText(hWnd,IDC_FILENAME,"展開完了しました");
					bExtract = false;
					bContinue = true;
				}else{
					if(MessageBox(hWnd,"展開を中止しますか？",SFXName,MB_YESNO)==IDYES)
						bContinue = false;
				}
				return TRUE;
		}
		return FALSE;
	}
    return FALSE;
}

BOOL PmaSfx::ConfirmProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
	switch(uMsg){
		case WM_INITDIALOG:{
			//Init
			char sz[FNAME_MAX32];

			//Dialog
			CenteringWindow(hWindow,hWnd);

			//String
			sprintf(sz,"'%s'はすでに存在します。上書きしますか？",szName);
			SetDlgItemText(hWnd,IDC_FILE,(LPCTSTR)sz);

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

BOOL APIENTRY WinMain(HINSTANCE hInst,HINSTANCE hInstPrev,LPSTR lpCmdLine,int nShowCmd)
{
	PmaSfx SFX;
	SFX.bExtract = false;
	SFX.bContinue = true;
	DialogBoxParam(hInst,MAKEINTRESOURCE(IDD_MAIN),NULL,PmaSfx_DlgProc,(LPARAM)(&SFX));
	return TRUE;
}
