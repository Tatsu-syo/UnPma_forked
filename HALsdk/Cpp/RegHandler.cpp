//
// RegHandler.cpp
// ÉåÉWÉXÉgÉää«óù
//
// $Id: RegHandler.cpp,v 1.2 2005/10/10 15:52:02 halpussy Exp $
//

#include "../HALsdk.h"
#include "RegHandler.h"

bool RegHandler::Open(HKEY hParent, LPCSTR szKeyName){
	Close();
	if(RegOpenKeyEx(hParent, szKeyName, 0, KEY_ALL_ACCESS, &hKey))
		return false;
	return true;
}

bool RegHandler::Create(HKEY hParent, LPCSTR szKeyName){
	DWORD dw;
	Close();
	if(RegCreateKeyEx(hParent, szKeyName, 0, NULL, REG_OPTION_NON_VOLATILE, 
						KEY_ALL_ACCESS, NULL, &hKey, &dw))
		return false;
	return true;
}

bool RegHandler::Close(){
	if(RegCloseKey(hKey))
		return false;
	hKey = 0;
	return true;
}

bool RegHandler::Delete(HKEY hParent, LPCSTR szKeyName){
	if(RegDeleteKey(hParent, szKeyName))
		return false;
	return true;
}

bool RegHandler::Get(LPCSTR szEntry){
	FreeAndNil(mlpData);
	if(RegQueryValueEx(hKey, szEntry, NULL, &mdwType, NULL, &mcbData))
		return false;
	if( !(mlpData = (LPBYTE)malloc(mcbData+2)) ) //îOÇÃÇΩÇﬂ
		return false;
	if(RegQueryValueEx(hKey, szEntry, NULL, &mdwType, mlpData, &mcbData))
		return false;
	return true;
}

bool RegHandler::Set(LPCSTR szEntry, const DWORD dwType, const LPBYTE lpData, const DWORD cbData){
	if(RegSetValueEx(hKey, szEntry, 0, dwType, lpData, cbData))
		return false;
	return true;
}