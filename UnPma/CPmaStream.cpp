#include "CPmaStream.h"
#include "../HALsdk/HALsdk.h"

PmaStream::PmaStream() :
	mpfnCallback(NULL), mpParam(NULL), wDict(1<<13), wDict1(wDict-1), wOffset(0x100-2)
{
	Init();
}

PmaStream::~PmaStream(){
	Close();
}

DWORD PmaStream::GetLastError(){
	return mdwLastError;
}

void PmaStream::SetCallback(void* pParam, PMA_CALLBACK pfnCallback){
	mpParam			= pParam;
	mpfnCallback	= pfnCallback;
}

void PmaStream::Init(){
	mhArc=INVALID_HANDLE_VALUE;
	mdwLastError=0;
	mliPointer.QuadPart=0;
	mbOnFile=false;
	mbEOF=false;
	mlpszHeader=NULL;
	mcbHeader=0;
	NullMemory(&mSubInfoEx, sizeof(INDIVIDUALINFOEX));
	NullMemory(mszMemo, 74);
	mcbMemo=0;
}

bool PmaStream::Open(LPCSTR lpszArc){
	if(mhArc!=INVALID_HANDLE_VALUE){
		mdwLastError=ERROR_SHARING;
		return false;
	}
	mhArc=CreateFile(lpszArc, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL, NULL);
	if(mhArc==INVALID_HANDLE_VALUE){
		mdwLastError=ERROR_SHARING;
		return false;
	}
	mdwLastError=0;
	return true;
}

bool PmaStream::Close(){
	if(mhArc==INVALID_HANDLE_VALUE){
		mdwLastError=ERROR_INVALID_HANDLE;
		return false;
	}
	CloseHandle(mhArc);
	Init();
	mdwLastError=0;
	return true;
}

bool PmaStream::SeekTop(){
	BYTE *b;
	DWORD dw=2, dwRead;

	if(mhArc==INVALID_HANDLE_VALUE){
		mdwLastError=ERROR_INVALID_HANDLE;
		return false;
	}

	b=(BYTE*)malloc(1000000);
	if(!b){
		mdwLastError=ERROR_ENOUGH_MEMORY;
		return false;
	}

	SetFilePointer(0);
	mbEOF = false; //フラグをクリア
	ReadFile(mhArc, b, 1000000, &dwRead, NULL);

	for(;dw+3<dwRead;dw++){
		if((b[dw] == '-') && (b[dw+1] == 'p') && (b[dw+2] == 'm') && Between('0', b[dw+3], '9') && (b[dw+4] == '-')){
			if (b[dw-2] > 20 && b[dw-1] == HeaderSum(b+dw, b[dw-2])){
			    ::SetFilePointer(mhArc, dw-2, NULL, FILE_BEGIN);
				free(b);
				mdwLastError=0;
	    		return true;
			}
		}
	}

	free(b);
	mdwLastError=ERROR_FILE_STYLE;
	return false; //書庫検索失敗
}

bool PmaStream::ReadHeader(LPINDIVIDUALINFOEX lpSubInfoEx){
	DWORD dwRead=0;
	INT64 llPointer;

	if(mhArc==INVALID_HANDLE_VALUE){
		mdwLastError=ERROR_INVALID_HANDLE;
		return false;
	}

	if(mbEOF){
		mdwLastError=ERROR_EOF;
		return false;
	}

	if(mbOnFile){
		if(!SetFilePointer(GetFilePointer()+mSubInfoEx.llCompressedSize)){
			mbEOF=true;
			mdwLastError=ERROR_UNEXPECTED_EOF;
			return false;
		}
	}

	llPointer=GetFilePointer();
	ReadFile(mhArc, &mcbHeader, 1, &dwRead, NULL);

	if(!dwRead){
		SetFilePointer(llPointer);
		mdwLastError=ERROR_EOF;
		return false;
	}
	if(mcbHeader==0){
		SetFilePointer(llPointer);
		mbEOF=true;
		mdwLastError=0;
		return false;
	}

	if(mcbHeader<21){
		SetFilePointer(llPointer);
		mdwLastError=ERROR_HEADER_BROKEN;
		return false;
	}

	mlpszHeader=(BYTE*)malloc( (int)(mcbHeader+1) );
	if(!mlpszHeader){
		SetFilePointer(llPointer);
		mdwLastError=ERROR_ENOUGH_MEMORY;
		return false;
	}

	ReadFile(mhArc, mlpszHeader, mcbHeader+1, &dwRead, NULL);
	if(!dwRead){
		SetFilePointer(llPointer);
		free(mlpszHeader);
		mbEOF=true;
		mdwLastError=ERROR_UNEXPECTED_EOF;
		return false;
	}

	if(HeaderSum(mlpszHeader+1, mcbHeader)!=mlpszHeader[0]){
		SetFilePointer(llPointer);
		free(mlpszHeader);
		mdwLastError=ERROR_HEADER_BROKEN;
		return false;
	}

	//ヘッダ読み込み開始
	NullMemory(&mSubInfoEx, sizeof(INDIVIDUALINFOEX));
	memcpy(mSubInfoEx.szMode, mlpszHeader+1, 5);
	memcpy(&mSubInfoEx.llCompressedSize, mlpszHeader+6, 4);
	memcpy(&mSubInfoEx.llOriginalSize, mlpszHeader+10, 4);

	{
		WORD wDate, wTime;
		FILETIME ftLocal;
		memcpy(&wTime, mlpszHeader+14, 2);
		memcpy(&wDate, mlpszHeader+16, 2);

		ftLocal = DosTimeToFileTime(wDate, wTime);
		LocalFileTimeToFileTime(&ftLocal, &mSubInfoEx.ftWriteTime);
		mSubInfoEx.ftCreateTime = mSubInfoEx.ftWriteTime;
		mSubInfoEx.ftAccessTime = mSubInfoEx.ftWriteTime;
	}

	memcpy(&mSubInfoEx.dwAttributes, mlpszHeader+18, 2);
	memcpy(mSubInfoEx.szFileName, mlpszHeader+21, mlpszHeader[20]);
	memcpy(&mSubInfoEx.dwCRC, mlpszHeader+21+mlpszHeader[20], 2);
	mSubInfoEx.uOSType = 15; //OSTYPE_CPM
	mSubInfoEx.wRatio = mSubInfoEx.llOriginalSize
							? (WORD)( (INT64)(mSubInfoEx.llCompressedSize*1000/mSubInfoEx.llOriginalSize) ) : 0;

	NullMemory(mszMemo, 74);
	mcbMemo=mcbHeader-22-mlpszHeader[20];
	memcpy(mszMemo, mlpszHeader+23+mlpszHeader[20], min(74, mcbMemo));

	if(lpSubInfoEx)
		memcpy(lpSubInfoEx, &mSubInfoEx, sizeof(INDIVIDUALINFOEX));

	free(mlpszHeader);
	mdwLastError=0;
	mbOnFile=true;
	return true;
}

bool PmaStream::Extract(LPCSTR lpszDest){
	HANDLE	hFile;
	crc16table crc16;
	WORD	wCRC=0;
	INT64	llNext=GetFilePointer()+mSubInfoEx.llCompressedSize;

	if(mhArc==INVALID_HANDLE_VALUE){
		mdwLastError=ERROR_INVALID_HANDLE;
		return false;
	}

	if(mbEOF){
		mdwLastError=ERROR_EOF;
		return false;
	}

	if(!mbOnFile){
		mdwLastError=-1;
		return false;
	}

	if(memcmp(mSubInfoEx.szMode, "-pm2-", 5)){
		if(memcmp(mSubInfoEx.szMode, "-pm0-", 5)){
			if(!SetFilePointer(llNext))
				mbEOF=true;
			mbOnFile=false;
			mdwLastError=ERROR_UNKNOWN_TYPE;
			return false;
		}
	}

	hFile = CreateFile(lpszDest, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL, NULL);

	if(hFile==INVALID_HANDLE_VALUE){
		if(!SetFilePointer(llNext))
			mbEOF=true;
		mbOnFile=false;
		mdwLastError=ERROR_FILE_OPEN;
		return false;
	}

	InitCRC16(&crc16);

	if(!memcmp(mSubInfoEx.szMode, "-pm0-", 5)){
		//UnStore
		BYTE *b=(BYTE*)malloc(0x10000);
		INT64 llWrite = mSubInfoEx.llCompressedSize;
		DWORD dw;

		if(!b){
			if(!SetFilePointer(llNext))
				mbEOF=true;
			CloseHandle(hFile);
			DeleteFile(lpszDest);
			mbOnFile=false;
			mdwLastError=ERROR_ENOUGH_MEMORY;
			return false;
		}

		for(;;){
			DWORD dwWriteSize = (DWORD)( (INT64)min(0x10000, llWrite) );
			ReadFile(mhArc, b, dwWriteSize, &dw, NULL);

			if(dw<dwWriteSize){
				free(b);
				CloseHandle(hFile);
				DeleteFile(lpszDest);
				mbEOF=true;
				mdwLastError=ERROR_UNEXPECTED_EOF;
				return false;
			}

			WriteFile(hFile, b, dwWriteSize, &dw, NULL);
			wCRC=UpdateCRC16(crc16, b, dwWriteSize, wCRC);
			if(llWrite<0x10000) break;
			llWrite-=0x10000;
			if(mpfnCallback){
				if(!mpfnCallback(mpParam, mSubInfoEx.llCompressedSize-llWrite)){
					if(!SetFilePointer(llNext))
						mbEOF=true;
					free(b);
					CloseHandle(hFile);
					DeleteFile(lpszDest);
					mbOnFile=false;
					mdwLastError=ERROR_USER_CANCEL;
					return false;
				}
			}
		}
		free(b);
		CloseHandle(hFile);
		if(wCRC!=mSubInfoEx.dwCRC){
			mbOnFile=false;
			mdwLastError=ERROR_FILE_CRC;
			//DeleteFile(lpszDest);
			return false;
		}
		mbOnFile=false;
		mdwLastError=0;
		return true; //成功
	}

	{//Extract (-pm2-)
		szText = (BYTE*)malloc(wDict);
		llWrite=0;
		wLocation = 0;

		DWORD	dw;
		WORD	w1, w2, w3, wBytes;

		if(!szText){
			if(!SetFilePointer(llNext))
				mbEOF=true;
			CloseHandle(hFile);
			DeleteFile(lpszDest);
			mbOnFile=false;
			mdwLastError=ERROR_ENOUGH_MEMORY;
			return false;
		}
		NullMemory(szText, wDict);

		DecodeInit();

		while (llWrite < mSubInfoEx.llOriginalSize) {
			w1 = Decode1();
			if(w1<256){
				szText[wLocation++] = (BYTE)w1;
				llWrite++;
				if(wLocation==wDict){
					WriteFile(hFile, szText, wDict, &dw, NULL);
					wLocation = 0;
					wCRC=UpdateCRC16(crc16, szText, wDict, wCRC);
					if(mpfnCallback){
						if(!mpfnCallback(mpParam, llWrite)){
							if(!SetFilePointer(llNext))
								mbEOF=true;
							free(szText);
							CloseHandle(hFile);
							DeleteFile(lpszDest);
							mbOnFile=false;
							mdwLastError=ERROR_USER_CANCEL;
							return false;
						}
					}
				}
			}else{
				wBytes = w1 - wOffset;
				w2 = wLocation - Decode2() - 1;
				llWrite += wBytes;
				for (w3 = 0; w3 < wBytes; w3++) {
					w1 = szText[(w2+w3) & wDict1];
					szText[wLocation++] = (BYTE)w1;
					if (wLocation == wDict) {
					WriteFile(hFile, szText, wDict, &dw, NULL);
						wLocation = 0;
						wCRC=UpdateCRC16(crc16, szText, wDict, wCRC);
						if(mpfnCallback){
							if(!mpfnCallback(mpParam, llWrite)){
								if(!SetFilePointer(llNext))
									mbEOF=true;
								free(szText);
								CloseHandle(hFile);
								DeleteFile(lpszDest);
								mbOnFile=false;
								mdwLastError=ERROR_USER_CANCEL;
								return false;
							}
						}
					}
				}
			}
		}

		if (wLocation) {
			WriteFile(hFile, szText, wLocation, &dw, NULL);
			wCRC=UpdateCRC16(crc16, szText, wLocation, wCRC);
		}

		free(szText);
		CloseHandle(hFile);

		if(wCRC!=mSubInfoEx.dwCRC){
			mbOnFile=false;
			mdwLastError=ERROR_FILE_CRC;
			//DeleteFile(lpszDest);
			return false;
		}
	}//Extract

	mdwLastError=0;
	mbOnFile=false;
	return true; //成功
}

INT64 PmaStream::GetFilePointer(){
	INT64 llRet=0;

	if(mhArc==INVALID_HANDLE_VALUE){
		mdwLastError=ERROR_INVALID_HANDLE;
		return -1;
	}
	llRet=GetFilePointeri64(mhArc);
	mdwLastError=0;
	return llRet;
}

bool PmaStream::SetFilePointer(INT64 llPointer){
	if(mhArc==INVALID_HANDLE_VALUE){
		mdwLastError=ERROR_INVALID_HANDLE;
		return false;
	}
	if(!SetFilePointeri64(mhArc, llPointer))
		return false;
	mdwLastError=0;
	return true;
}

BYTE PmaStream::HeaderSum(BYTE *b, BYTE cb){
	BYTE cbCheck=0;
	UINT u=0;
	for(;u<cb;u++)
		cbCheck+=b[u];
	return cbCheck;
}

BYTE PmaStream::GetMemo(char *lpszBuf, DWORD dwLen){
	if(lpszBuf&&dwLen){
		NullMemory(lpszBuf, dwLen-1);
		memcpy(lpszBuf, mszMemo, min(mcbMemo, dwLen));
		lpszBuf[dwLen-1]=0; //NULL Stop
		return dwLen>mcbMemo ? 0 : -1;
	}
	return mcbMemo ? mcbMemo+1 : 0;
}