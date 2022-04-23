//
// BufWrite.cpp
// �o�b�t�@��������
//
// $Id: BufWrite.cpp,v 1.2 2005/10/30 04:19:04 halpussy Exp $
//

#include "BufWrite.h"
#include <stdio.h>

#pragma warning(disable:4996) // for New C RunTime Library. I'll not use strcpy_s.

#define PUT(c) printf_put(pd, (c))

//���C�����[�`��
BufWriter::BufWriter(LPSTR BufText, DWORD Length){
	if(BufText&&Length) strcpy(BufText,"");
	mlpszBufText=BufText;
	mdwLength=Length;
	mbBufwrite=true;
}

void BufWriter::Write(LPCSTR lpszStr){
	if(mlpszBufText&&mbBufwrite&&mdwLength)
		strncat(mlpszBufText,lpszStr,mdwLength-strlen(mlpszBufText));
}

void BufWriter::Print(LPCSTR lpszFormat, ...){
	if(mlpszBufText&&mbBufwrite&&mdwLength){
		PRINTF_DATA wpd = { (unsigned char*)mlpszBufText+strlen(mlpszBufText),0,mdwLength-strlen(mlpszBufText) };
		va_list ap;
		va_start(ap, lpszFormat);
		bf_print(lpszFormat, &wpd, ap);
		va_end(ap);
	}
}

void BufWriter::Enable(){mbBufwrite=true;}
void BufWriter::Disable(){mbBufwrite=false;}

//�T�u���[�`��

//�P�ꕶ���o��
void BufWriter::printf_put(PRINTF_DATA *pd, int c){
	// ���E�ɒB�����珑�����݂��~����
	if (pd->num_written++ >= pd->limit) return;

	*(pd->dst) = c;
	pd->dst++;
}

// 32bit�����o��(8/10/16�i��)
void BufWriter::printf_int(
	PRINTF_DATA *pd,
	outf_state *os, 	//�t���O
	unsigned int base,  //�i��
	INT64 v,            //��
	unsigned char f,    //�ϊ��`��
	int is_u)           //�����Ȃ��̂Ƃ���0
{
	static const char nib2x[] = "0123456789abcdef";
	static const char nib2X[] = "0123456789ABCDEF";

	unsigned char rev[32];

	unsigned int len = 0;
	unsigned int jlen;
	unsigned int wlen;
	unsigned int i;
	unsigned char c = 0;
	const char *nib2 = (f == 'x') ? nib2x : nib2X;

	// ���x�������ݒ�
	if (os->prec == 0xffffffff) os->prec = 1; else os->fill = ' ';

	// short�C���q
	if (os->lmod == 'h'){
		if (is_u) v = v & 0xffff; else v = (int)(short)v;
	}

	// rev[] �ɕ������t���ɂ��ē����
	{
		UINT64 vv;

		if (is_u) vv = v; else vv = (v < 0) ? -v : v;

		if ((vv == 0) && os->prec) rev[len++] = '0';

		while (vv > 0){
			rev[len++] = nib2[vv % base];
			vv /= base;
		}
	}

	// ����
	if (!is_u){
		if (v >= 0){
			if (os->sign) c = '+';
			else if (os->blank) c = ' ';
		}else{
			c = '-';
		}
	}

	// �����Z�o
	wlen = (len > os->prec) ? len : os->prec;

	// �񂹕��Z�o
	jlen = (len > os->prec) ? len : os->prec;

	if (c) ++jlen; // ����

	// 8�i���o�͂̓���ȏꍇ:�ړ���0��t�����Ȃ��Ă�����
	if (os->alt && (f == 'o')){
		// 0�ɂ��E��
		if (!os->left && (os->fill == '0') && (jlen < os->width)) os->alt = 0;
		// ���x�̂��߂�0�t��
		if (wlen > len) os->alt = 0;
	}

	if (os->alt){
		switch (base){
			case 16: ++jlen; // 0x/0X (2����)
			case 8:  ++jlen; // 0 (1����)
		}
	}

	// �o��
	// �󔒂ɂ��E��
	if (!os->left && (os->fill == ' ') && (jlen < os->width)){
		for (i = os->width - jlen; i > 0; --i) PUT(' ');
    }

	// ����
	if (c) PUT(c);

	// �ړ���
	if (os->alt){
		switch (base){
			case 16: PUT('0'); PUT(f); break;
			case 8:  PUT('0'); break;
		}
	}

    // 0�ɂ��E��
    if (!os->left && (os->fill == '0') && (jlen < os->width)){
		for (i = os->width - jlen; i > 0; --i) PUT('0');
	}

	// ���x�̂��߂�0�t��
	for (i = wlen - len; i > 0; --i) PUT('0');

	// ��
	for (i = len; i > 0; --i){
		PUT(rev[i - 1]);
	}

	// ����
	if (os->left && (jlen < os->width)){
		for (i = os->width - jlen; i > 0; --i) PUT(os->fill);
	}
}

// 64bit���������_
/*
void BufWriter::printf_float(
	PRINTF_DATA *pd,
	outf_state *os, // �t���O
	unsigned int v[2],   // ��
	unsigned char f)     // �ϊ��`��
{
	unsigned int len = 0;
	unsigned char c = 0;
	int sign = (v[0] & 0x80000000) ? -1 : 1;
	int exp = ((v[0] >> 20) & 0x07ff) - 0x03ff;
	unsigned int manth = v[0] & 0x000fffff;

	// ���x�������ݒ�
	if (os->prec == 0xffffffff) os->prec = 6;

	// �����Ǝw��������
	v[0] &= 0x000fffff;

	if (sign > 0){
		if (os->sign) c = '+';
		else if (os->blank) c = ' ';
    } else {
		c = '-';
	}

	if (c) PUT(c);

	PUT('e');
	printf_int(pd, os, 10, exp, 'd', 0);
	//PUT('/');
	//printf_int(pd, os, 10, v[0], 'u', 1);
	//PUT('.');
	//printf_int(pd, os, 10, v[1], 'd', 0);
}
*/

int BufWriter::bf_print(const char *lpszFormat,
				PRINTF_DATA *pd,
				va_list arg)
{
	static const char null[]  = "(null)";
	const unsigned char *f = (const unsigned char *)lpszFormat;

	if (pd->limit == 0) return 0;

	while (*f){
		outf_state os = { 0, 0, 0, ' ', 0, 0, 0xffffffff, 0 };

		// %�܂ŃR�s�[
		while (*f && (*f != '%')) PUT(*f++);

		// �C���q
		while (*f){
			++f;
			if (*f == '-') { os.left  = 1; os.fill = ' ';  continue; }
			if (*f == '+') { os.sign  = 1; os.blank = 0;   continue; }
			if (*f == ' ') { if (!os.sign) os.blank = 1;   continue; }
			if (*f == '0') { if (!os.left) os.fill  = '0'; continue; }
			if (*f == '#') { os.alt   = 1;                 continue; }
			break;
		}

		// ����
		while (isdigit(*f)){
			os.width = 10*os.width + (*f - '0');
			++f;
		}

		if (*f == '*'){
			int v = va_arg(arg, int);
			if (v < 0) { v = -v; os.left = 1; os.fill = ' '; }
			os.width = v;
			++f;
		}

		// ���x
		if (*f == '.'){
			++f;

			os.prec = 0;

			while (isdigit(*f)){
				os.prec = 10*os.prec + (*f - '0');
				++f;
			}

            if (*f == '*'){
				int v = va_arg(arg, int);
				if (v >= 0) os.prec = v;
				++f;
			}
		}

		// �����C���q
		if ((*f == 'h') || (*f == 'l') || (*f == 'L')){
			os.lmod = *f++;
		}

		// ���[�ɗ������~
		if (*f == 0) break;

		// ���
		switch (*f){
		case 'b':{ //�o�C�i���o��(��W��)
			unsigned int v = va_arg(arg, unsigned int);
			printf_int(pd, &os, 2, v, 'b', 1);
		}break;
		case 'd':
		case 'i':{
			int v = va_arg(arg, int);
			printf_int(pd, &os, 10, v, 'd', 0);
		}break;
		case 'o':{
			unsigned int v = va_arg(arg, unsigned int);
			printf_int(pd, &os, 8, v, 'o', 1);
		}break;
		case 'u':{
			unsigned int v = va_arg(arg, unsigned int);
			printf_int(pd, &os, 10, v, 'u', 1);
		}break;
		case 'x':
		case 'X':{
			unsigned int v = va_arg(arg, unsigned int);
			printf_int(pd, &os, 16, v, *f, 1);
		}break;
		/** float cannot be used
		case 'f':
		case 'F':
		case 'e':
		case 'E':
		case 'g':
		case 'G':
		case 'a':
		case 'A':{
			unsigned int v[2];
			double db=va_arg(arg,double);
			v[1] = va_arg(arg, unsigned int);
			v[0] = va_arg(arg, unsigned int);

			printf_float(pd, &os, v, *f);
			//printf("%x.%x\n",va_arg(arg,UINT),va_arg(arg,UINT));
			//printf("%.16f/%.17f/%I64u\n",db,db,db);
		}break;
		*/
		case 'c':{
			int v = va_arg(arg, int);
			PUT((unsigned char) v);
		}break;
		case 's':{
			const char *v = va_arg(arg, char *);
			unsigned int len;
			unsigned int i;

			// ���x�������ݒ�
			if (os.prec == 0xffffffff) os.prec = 0;

			if (v == NULL) v = null;

			len = strlen(v);

			// �񂹕����Z�o
			if (os.prec && (len > os.prec)) len = os.prec;

			// �E�Ɋ񂹂�
			if (!os.left && (len < os.width)){
				for (i = os.width - len; i > 0; --i) PUT(os.fill);
			}

			// ������
			for (i = 0; i < len; ++i){
				PUT(*v++);
			}

			// ���Ɋ񂹂�
			if (os.left && (len < os.width)){
				for (i = os.width - len; i > 0; --i) PUT(os.fill);
			}
		}break;
		case 'p':{
			unsigned int v = va_arg(arg, unsigned int);
			outf_state tmp = { 0, 0, 0, '0', 0, 8, 8, 0 };
			printf_int(pd, &tmp, 16, v, 'X', 1);
		}break;
		case 'n':{
			if (os.lmod == 'h'){
				short *v = va_arg(arg, short *);
				if (v) *v = (short)pd->num_written;
			}else{
				int *v = va_arg(arg, int *);
				if (v) *v = pd->num_written;
			}
		}break;
		case '%':
			PUT('%');
			break;

		//64bit����
		case 'I':{
			if (*(++f)=='6'&&*(++f)=='4'){
				switch(*(++f)){
					case 'd':
					case 'i':{
						INT64 v = va_arg(arg, INT64);
						printf_int(pd, &os, 10, v, 'd', 0);
					}break;
					case 'u':{
						UINT64 v = va_arg(arg, UINT64);
						printf_int(pd, &os, 10, v, 'u', 1);
					}break;
					case 'x':
					case 'X':{
						UINT64 v = va_arg(arg, UINT64);
						printf_int(pd, &os, 16, v, *f, 1);
					}break;
					case 'o':{
						UINT64 v = va_arg(arg, UINT64);
						printf_int(pd, &os, 8, v, 'o', 1);
					}break;
				}
            }
		}break;

		}
		++f;
	}
	return pd->num_written;
}
