//
// TempFile.h
// 一時ファイル
//
// $Id: TempFile.h,v 1.1 2005/09/18 05:49:28 halpussy Exp $
//

#ifndef HALSDK_TEMPFILE_H
#define HALSDK_TEMPFILE_H

#include <windows.h>

class TempFile
{
public:
	TempFile();
	virtual ~TempFile(){Done(NULL);}

	//一時ファイル作成
	UINT Create(LPCSTR lpszPrefix, LPCSTR lpszWorkDir);

	//一時ファイルの処理が終わったことを通知し、必要ならば書き戻す
	bool Done(LPCSTR lpszCopyTo);

public:
	//LPSTR()で返されたファイル名でファイルを開くことはできません。HANDLE()で操作してください。
	operator HANDLE() const  { return mhFile; }
	operator LPSTR() const   { return (LPSTR)mszTempFile; }

protected:
	char		mszTempFile[MAX_PATH+1];
	HANDLE		mhFile;
	bool		mbOpened;

private:
	TempFile(const TempFile&);
	void operator=(const TempFile&);
};

#endif