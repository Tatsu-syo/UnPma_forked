//
// IniHandler.cpp
// INIä«óù
//
// $Id: IniHandler.cpp,v 1.2 2005/10/10 15:52:02 halpussy Exp $
//

#include "../HALsdk.h"
#include "IniHandler.h"

bool IniHandler::Open(LPCSTR szIniName){
	if(!szIniName)
		return false;
	strcpy(mszIniName, szIniName);
	return true;
}

LPSTR IniHandler::GetString(LPCSTR szSection, LPCSTR szKey){
	LPSTR psz="";
	NullMemory(mszBuffer, sizeof(mszBuffer));

	GetPrivateProfileString(szSection, szKey, psz, mszBuffer, sizeof(mszBuffer), mszIniName);
	return mszBuffer;
}

UINT IniHandler::GetInt(LPCSTR szSection, LPCSTR szKey){
	return GetPrivateProfileInt(szSection, szKey, -1, mszIniName);
}

bool IniHandler::SetString(LPCSTR szSection, LPCSTR szKey, LPCSTR szValue){
	if(!WritePrivateProfileString(szSection, szKey, szValue, mszIniName))
		if(GetLastError!=NO_ERROR)
			return false;
	return true;
}

bool IniHandler::SetInt(LPCSTR szSection, LPCSTR szKey, UINT uValue){
	char szValue[20];
	sprintf(szValue, "%u", uValue);
	return SetString(szSection, szKey, szValue);
}
