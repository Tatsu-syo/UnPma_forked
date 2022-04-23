//
// MemmFile.h
// メモリーマップドファイル
//
// $Id: MemmFile.h,v 1.1 2005/09/18 05:49:28 halpussy Exp $
//

#ifndef HALSDK_MEMMFILE_H
#define HALSDK_MEMMFILE_H

#include <windows.h>

class MemmapFile
{ //少し旧CmdLineのmapfileに似せました。
public:
	MemmapFile(){Init();}
	virtual ~MemmapFile(){Close();}

	bool Open( const char* fname );
	void Close();
	void Init(){hFile=INVALID_HANDLE_VALUE;hMap=NULL;Ptr=0;dwSize=0;}

public:
	DWORD size() const       { return dwSize; }
	const void* base() const { return Ptr; }
	operator HANDLE() const  { return hFile; }

protected:
	HANDLE        hFile;
	HANDLE        hMap;
	DWORD         dwSize;
	const void*   Ptr;

private:
	MemmapFile(const MemmapFile&);
	void operator=(const MemmapFile&);
};

#endif