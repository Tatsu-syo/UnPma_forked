//
// IniHandler.h
// INIä«óù
//
// $Id: IniHandler.h,v 1.1 2005/09/18 05:49:28 halpussy Exp $
//

#ifndef HALSDK_INIHANDLER_H
#define HALSDK_INIHANDLER_H

#include <windows.h>

class IniHandler
{
public:
	bool Open(LPCSTR szIniName);

	LPSTR GetString(LPCSTR szSection, LPCSTR szKey);
	UINT GetInt(LPCSTR szSection, LPCSTR szKey);
	bool SetString(LPCSTR szSection, LPCSTR szKey, LPCSTR szValue);
	bool SetInt(LPCSTR szSection, LPCSTR szKey, UINT uValue);

protected:
	char	mszIniName[MAX_PATH+1];
	char	mszBuffer[512];
};

#endif
