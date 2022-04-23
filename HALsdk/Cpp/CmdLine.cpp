//
// CmdLine.cpp
// コマンド解析
//
// $Id: CmdLine.cpp,v 1.1 2005/09/18 05:49:28 halpussy Exp $
//

#include "CmdLine.h"
#include "MemmFile.h"

char* CmdLine::copyarg(const unsigned char *s,const unsigned char *se,int dq){
	int l = se-s-dq;
	if(!l)
		return "";
	char *d0 = (char*)malloc(l+1);
	if(!d0)
		return NULL;
	char *d=d0;
	for(;s<se;s++)
		if( *s!='"' )
			*d++ = *s;
	*d = 0;
	return d0;
}

int CmdLine::parse(const char *cmdline,int l,int resp_ok){
	if(!cmdline)
		return 0;
	const unsigned char *cp = (const unsigned char*)cmdline;
	const unsigned char *ce = cp + l;
	const unsigned char *c0 = 0;

	int resp_switch = 1;

	int dq = 0;
	for( ;;cp++ ){
		int c = cp == ce ? EOF : *cp;
		switch (c){
		case ' ':
		case '\t':
			if( dq&1 )
				goto normal_char;
		// fall thru...
		case EOF:
		case '\r':
		case '\n':
			if(c0){
				char *arg = copyarg(c0, cp, dq);
				if(!arg)
					return 0x801E;//ERROR_ENOUGH_MEMORY
				if( resp_ok && *arg==resp ){
					MemmapFile mf;
					int e = mf.Open (arg + 1);
					free(arg);
					if(!e)
						return 0x802A;//ERROR_RESPONSE_READ
					
					//if(mf.base()!=&(FARPROC)mf.size()){ //サイズ0は無視。
					e = parse((LPCSTR)mf.base(), mf.size(), 0);
					if(e)
						return e;
					//}
				}else if(*arg){
					if( resp_switch && ((*arg=='-'&&*(arg+1)=='-')||(*arg=='/'&&*(arg+1)=='/')) ){
						resp = *(arg+2);	// resp switch!
						resp_switch = 0; //no longer switch.
					}else{
						if(!Add(arg)) return 0x801e; //ERROR_ENOUGH_MEMORY
					}
					free(arg);
				}
				dq = 0;
				c0 = 0;
			}
			if( c==EOF )
				return 0;
			break;

		case '"':
			dq++;
		// fall thru...
		default:
	normal_char:
			if( !c0 )
				c0 = cp;
			break;
		}
	}
}
