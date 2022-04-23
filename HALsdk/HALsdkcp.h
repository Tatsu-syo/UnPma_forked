//
// HALsdkCp.h
// C++êÍópä÷êîÇÃêÈåæ
//
// $Id: HALsdkcp.h,v 1.5 2005/09/18 05:50:27 halpussy Exp $
//

#ifndef HALSDKCP_H
#define HALSDKCP_H

#include "HALsdk.h"
#include "Cpp/Strings.h"
#include "Cpp/BufWrite.h"
#include "Cpp/CmdLine.h"
#include "Cpp/MemmFile.h"
#include "Cpp/Pointers.h"
#include "Cpp/RegHandler.h"
#include "Cpp/TempFile.h"
#include "Cpp/IniHandler.h"

//StrsFunc.cpp
bool UnWildcardS(Strings &sRet, const Strings &sCom, const DWORD dwMakemode);
	//sRet: The UnWildcarded strings.
	//sCom: The strings for UnWildcarding.
	//dwMakemode: The flag for UnWildcarding.

bool EnumFileList(Strings &sRet, LPCSTR wildname, LPCSTR current, const DWORD dwEnumMode);
	//sRet:       The UnWildcarded strings.
	//wildname:   String for searching.
	//current:    Current searching wildname(the same as wildname in the first call)
	//dwMakemode: The flag for UnWildcarding.

//Altrnate.cpp
bool EnumAlternativeStream(Strings &sRet, LPCSTR lpszFileName);
	//Get Alternative Streams.
	//sRet:         The enumed Strings.
	//lpszFileName: The filename for getting Alternative Streams.

#endif