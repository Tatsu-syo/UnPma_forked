//
// DlgCtrl.c
// ダイアログ処理
//
// $Id: DlgCtrl.c,v 1.5 2005/10/27 14:52:54 halpussy Exp $
//

#include "../HALsdk.h"
#include <commctrl.h>

bool CenteringWindow(const HWND hParent, const HWND hWnd){
	RECT parentrc,rc;
	HWND mhWnd = hWnd ? hWnd : GetDesktopWindow();
	int a,b;

	GetWindowRect(hParent ? hParent : GetDesktopWindow(),(LPRECT)&parentrc);
	GetWindowRect(mhWnd,(LPRECT)&rc);
	a=rc.right-rc.left,b=rc.bottom-rc.top;
	SetWindowPos(mhWnd,HWND_TOP,(parentrc.right-a)/2,(parentrc.bottom-b)/2,a,b,SWP_SHOWWINDOW);
	UpdateWindow(mhWnd);
	return true;
}

void DoEvents(){
	MSG msg;
	for(;PeekMessage(&msg,0,0,0,PM_REMOVE);){
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

int InfinityLoop(){
	MSG msg;
	for(;;){
		switch(GetMessage(&msg,NULL,0,0)){
			case 0:
			case 1:
				break;
			default:
				TranslateMessage(&msg);
				DispatchMessage(&msg);
				continue;
		}
		break;
	}
	return msg.wParam;
}

bool ResetParent(const HWND hParent, const HWND hWnd){
	/*
	 * この関数を使用して親ウィンドウを変更すると、
	 * ウィンドウが親ウィンドウのコントロールになってしまうバグがあるため
	 * 使用しないでください(^^;;;
	 */

	DWORD dwStyle=0;

	if(!hWnd) return false;

	dwStyle = GetWindowLong(hWnd,GWL_STYLE);

	SetLastError(0);
MessageDlg("1","Debug");
	if(hParent)
		//hWndを子ウィンドウに
		SetWindowLong(hWnd,GWL_STYLE,dwStyle - dwStyle&WS_POPUP + (dwStyle&WS_CHILD)^WS_CHILD);

	if(GetLastError()){
		ErrorMsg();
		return false;
	}
MessageDlg("2","Debug");
//MessageInt(GetWindowLong(hWnd,GWL_STYLE),"Debug");
	SetLastError(0);
	if(!SetParent(hWnd,hParent)&&GetLastError()){
		ErrorMsg();
		return false;
	}
MessageDlg("3","Debug");

	if(!hParent)
		//hWndをトップレベルに
		SetWindowLong(hWnd,GWL_STYLE,dwStyle - dwStyle&WS_CHILD + (dwStyle&WS_POPUP)^WS_POPUP);
MessageDlg("4","Debug");

	return true;
}

bool SetFont(HWND hWnd, int iHeight, DWORD dwCharSet, LPCSTR lpszFontName){
	HANDLE hFont = CreateFont(iHeight, 0, 0, 0, FW_REGULAR, FALSE, FALSE, FALSE,
			dwCharSet, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY,
			DEFAULT_PITCH|FF_MODERN, lpszFontName);
	if(!hFont){
		ErrorMsg();
		return false;
	}
	SendMessage(hWnd, WM_SETFONT, (WPARAM)hFont, 0);
	DeleteObject(hFont);
	return true;
}

bool NotifyIcon(HWND hWnd, DWORD dwMessage, UINT uID, UINT uFlags,
				UINT uCallbackMessage, HICON hIcon, LPCSTR szTip){
	NOTIFYICONDATA	Notify;
	NullMemory(&Notify, sizeof(NOTIFYICONDATA));

	Notify.cbSize	= 88;
	Notify.hWnd		= hWnd;
	Notify.uID		= uID;
	Notify.uFlags	= uFlags;
	Notify.uCallbackMessage = uCallbackMessage;
	Notify.hIcon	= hIcon;
	strcpy(Notify.szTip, szTip);

	if(Shell_NotifyIcon(dwMessage, &Notify))
		return true;
	return false;
}

void SetIcon2(HWND hWnd, HICON hIconBig, HICON hIconSmall){
	SendMessage(hWnd,WM_SETICON,ICON_BIG,(LPARAM)hIconBig);
	SendMessage(hWnd,WM_SETICON,ICON_SMALL,(LPARAM)hIconSmall);
}

void SetIcon(HWND hWnd, HICON hIcon){
	SetIcon2(hWnd, hIcon, hIcon);
}

//Callback
typedef struct{
	HWND	hWnd;
	HCURSOR hCursor;
}SETCURSORINFO, *LPSETCURSORINFO;

BOOL CALLBACK callback_SetClassCursorRecursive(HWND hCtrl, LPARAM lParam){
	LPSETCURSORINFO pCurInfo = (LPSETCURSORINFO)lParam;
	if(GetParent(hCtrl)==pCurInfo->hWnd)
		SetClassLong(hCtrl,GCL_HCURSOR,(LPARAM)pCurInfo->hCursor);
	return TRUE;
}
//

//クラス全体のカーソルを変更してしまうので、使いにくい面もあるかも。。。
void SetClassCursorRecursive(HWND hWnd, HCURSOR hCursor){
	SETCURSORINFO CurInfo;
	CurInfo.hWnd    = hWnd;
	CurInfo.hCursor = hCursor;
	SetClassLong(hWnd,GCL_HCURSOR,(LONG)hCursor);
	EnumChildWindows(hWnd, (WNDENUMPROC)callback_SetClassCursorRecursive, (LPARAM)&CurInfo);
	SetCursor(hCursor); //念のため。
}

void AddStatusText(HWND hCtrl, LPCSTR szText){
	//hCtrlには、エディットボックスのハンドルを指定してください。
	int pos, idx = SendMessage(hCtrl, EM_LINEINDEX, SendMessage(hCtrl, EM_GETLINECOUNT, 0, 0)-1, 0);
	if(idx < 0) return;
	pos = idx+SendMessage(hCtrl, EM_LINELENGTH, idx, 0);
	SendMessage(hCtrl, EM_SETSEL, pos, pos);
	SendMessage(hCtrl, EM_REPLACESEL, 0, (LPARAM)szText);
}

//Callback
typedef struct{
	WNDPROC pOldProc;
	HCURSOR hCursor;
}CURSORSUBCLASSINFO, *LPCURSORSUBCLASSINFO;

BOOL CALLBACK callback_CursorSubClass(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
	LPCURSORSUBCLASSINFO pCursor = (LPCURSORSUBCLASSINFO)GetWindowLong(hWnd,GWL_USERDATA);

	if(uMsg==WM_SETCURSOR){
		if(pCursor->hCursor){
			SetCursor(pCursor->hCursor);
			return TRUE;
		}
	}

	return CallWindowProc(pCursor->pOldProc, hWnd, uMsg, wParam, lParam);
}

BOOL CALLBACK callback_BeginCursorSubClass(HWND hCtrl, LPARAM lParam){
	if(GetParent(hCtrl)==(HWND)lParam){
		LPCURSORSUBCLASSINFO pCursor = (LPCURSORSUBCLASSINFO)malloc(sizeof(LPCURSORSUBCLASSINFO));
		pCursor->pOldProc = (WNDPROC)GetWindowLong(hCtrl,GWL_WNDPROC);
		pCursor->hCursor = NULL;

		SetWindowLong(hCtrl, GWL_USERDATA, (LONG)pCursor);
		SetWindowLong(hCtrl, GWL_WNDPROC, (LONG)callback_CursorSubClass);
	}
	return TRUE;
}

BOOL CALLBACK callback_EndCursorSubClass(HWND hCtrl, LPARAM lParam){
	if(GetParent(hCtrl)==(HWND)lParam){
		LPCURSORSUBCLASSINFO pCursor = (LPCURSORSUBCLASSINFO)GetWindowLong(hCtrl,GWL_USERDATA);

		SetWindowLong(hCtrl, GWL_WNDPROC, (LONG)pCursor->pOldProc);
		free(pCursor);
	}
	return TRUE;
}

BOOL CALLBACK callback_SetSubClassCursor(HWND hCtrl, LPARAM lParam){
	LPSETCURSORINFO pSetCur = (LPSETCURSORINFO)lParam;

	if(GetParent(hCtrl)==pSetCur->hWnd){
		LPCURSORSUBCLASSINFO pCursor = (LPCURSORSUBCLASSINFO)GetWindowLong(hCtrl,GWL_USERDATA);
		pCursor->hCursor = pSetCur->hCursor;
	}
	return TRUE;
}
//

//サブクラスを作成し、そこでSetCursorを行う方法。より安全。
void BeginCursorSubClass(HWND hWnd){
	//必ず、EndCursorSubClassを呼び出し、メモリの解放を行ってください。
	EnumChildWindows(hWnd, (WNDENUMPROC)callback_BeginCursorSubClass, (LPARAM)hWnd);
}

void EndCursorSubClass(HWND hWnd){
	EnumChildWindows(hWnd, (WNDENUMPROC)callback_EndCursorSubClass, (LPARAM)hWnd);
}

void SetSubClassCursor(HWND hWnd, HCURSOR hCursor){
	SETCURSORINFO SetCur;
	SetCur.hWnd    = hWnd;
	SetCur.hCursor = hCursor;
	EnumChildWindows(hWnd, (WNDENUMPROC)callback_SetSubClassCursor, (LPARAM)&SetCur);
}

int SetWhiteBackGround(HDC hDc){
	SetTextColor(hDc, RGB(0, 0, 0));
	SetBkColor(hDc, RGB(255, 255, 255));
	return (int)GetStockObject(WHITE_BRUSH);
}

typedef HRESULT (WINAPI*EnableThemeDialogTexture_proc)(HWND, DWORD);

#ifndef ETDT_DISABLE
#define ETDT_DISABLE       0x01
#define ETDT_ENABLE        0x02
#define ETDT_USETABTEXTURE 0x04
#define ETDT_ENABLETAB     0x06
#endif

bool EnableVisualTexture(HWND hWnd){
	HMODULE	hDll;
	DWORD	dw;
	EnableThemeDialogTexture_proc pEnableThemeDialogTexture;

	if(GetOSType()<WINXP) return false;
	hDll = LoadLibrary("uxtheme.dll");
	if(!hDll) return false;
	pEnableThemeDialogTexture = (EnableThemeDialogTexture_proc)GetProcAddress(hDll, "EnableThemeDialogTexture");
	if(!pEnableThemeDialogTexture){
		FreeLibrary(hDll);
		return false;
	}

	dw = pEnableThemeDialogTexture(hWnd, ETDT_ENABLETAB);
	FreeLibrary(hDll);
	return dw ? false : true;
}