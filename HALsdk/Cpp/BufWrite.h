//
// BufWrite.h
// バッファ書き込み
//
// $Id: BufWrite.h,v 1.2 2005/10/30 04:19:04 halpussy Exp $
//

#ifndef HALSDK_BUFWRITE_H
#define HALSDK_BUFWRITE_H

#include <windows.h>

typedef struct {
	unsigned char *dst;
	DWORD num_written;
	DWORD limit;
} PRINTF_DATA;

typedef struct {
	unsigned int  left;
	unsigned int  sign;
	unsigned int  blank;
	unsigned char fill;
	unsigned int  alt;
	unsigned int  width;
	unsigned int  prec;
	unsigned char lmod;
} outf_state;

class BufWriter
{
public:
	BufWriter(LPSTR BufText, DWORD Length);
	virtual ~BufWriter(){}

	int bf_print(const char *lpszFormat,PRINTF_DATA *pd,va_list arg);
	void printf_put(PRINTF_DATA *pd, int c);
	void printf_int(PRINTF_DATA *pd, outf_state *os, unsigned int base,
			INT64 v, unsigned char f, int is_u);
	//void printf_float(PRINTF_DATA *pd, outf_state *os, unsigned int v[2], unsigned char f);

	void Write(LPCSTR lpszStr);
	void Print(LPCSTR lpszFormat, ...);

	void Enable();
	void Disable();

protected:
	char* mlpszBufText;
	DWORD mdwLength;
	bool mbBufwrite;
};

#endif