//
// RegHandler.h
// レジストリ管理
//
// $Id: RegHandler.h,v 1.1 2005/09/18 05:49:28 halpussy Exp $
//

#ifndef HALSDK_REGHANDLER_H
#define HALSDK_REGHANDLER_H

#include <windows.h>

// Class RegHandler
// アクセス制限は考慮していません…

// RegOpenKeyEx/RegCreateKeyExは、レジストリをロックする働きもあるので注意。

class RegHandler
{
public:
	RegHandler(){mdwType=0;mlpData=NULL;mcbData=0;}
	virtual ~RegHandler(){Close();}

	bool Open(HKEY hParent, LPCSTR szKeyName);
	bool Create(HKEY hParent, LPCSTR szKeyName);
	bool Close();

	bool Delete(HKEY hParent, LPCSTR szKeyName); //開いている最中に呼び出すことも一応可能。

	bool Get(LPCSTR szEntry);
	bool Set(LPCSTR szEntry, const DWORD dwType, const LPBYTE lpData, const DWORD cbData);

	operator HKEY() const{return hKey;} //Open/CreateのhParentに用いることができる。

	DWORD mdwType;
	LPBYTE mlpData; //Getのデータを格納する。このバッファのサイズをmcbDataより獲得し処理を行う。
	DWORD mcbData;

protected:
	HKEY hKey;
};

#endif
