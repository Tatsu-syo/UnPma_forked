//
// CmdLine.h
// ƒRƒ}ƒ“ƒh‰ğÍ
//
// $Id: CmdLine.h,v 1.1 2005/09/18 05:49:28 halpussy Exp $
//

#ifndef HALSDK_CMDLINE_H
#define HALSDK_CMDLINE_H

#include "Strings.h"

class CmdLine : public Strings
{
public: //--<action>--
	CmdLine() : resp('@') {}
	virtual ~CmdLine(){}
	int parse( const char* str, int resp_ok ){ return parse( str, str?strlen(str):0, resp_ok ); }

private: //--<private>--
	int parse( const char*, int, int );
	char resp;
	static char *copyarg(const unsigned char*,const unsigned char *,int);
};

#endif
