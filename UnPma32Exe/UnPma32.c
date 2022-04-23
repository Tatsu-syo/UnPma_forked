#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Result buffer size
#define RBUFSIZE 1024000

void UNLHAputs(const char *s)
{
	char c = '\0';

	while(*s){
		switch(*s){
			case '\n':
				fputc(*s,stdout);
			break;
			case '\r':
			break;
			default:
				if (c == '\r'){
					fputc('\r',stdout);
				}
				fputc(*s,stdout);
			break;

		}
		c = *s;
		s++;
	}
}

void Usage(void)
{
	UNLHAputs("UnPma32 By Tatsuhiko Syoji 2001,2022\nUsage:UnPma UnPma option\n\nPlease see UnPma32.DLL's manual.\n");
}

const char *noMemMes = "No enough memory.\n";
typedef int (WINAPI *TFUNC)(HWND,char *,char *,unsigned int);

int main(int argc,char *argv[])
{
    static HINSTANCE   hInstDLL;       /*  ‚c‚k‚k‚ÌƒCƒ“ƒXƒ^ƒ“ƒXƒnƒ“ƒhƒ‹                */
    static TFUNC       Unlha;    /*  ‚c‚k‚k‚Ì‚l‚™‚e‚•‚‚ƒ‚”‚‰‚‚‚Ö‚Ìƒ|ƒCƒ“ƒ^    */
	char *combuf,*resultbuf;
	int i,comLen,result;

	if (argc == 1) {
		/* If no arguments we call the Usage routine and exit */
		Usage();
		return 1;
	}

	resultbuf = malloc(RBUFSIZE);
	if (resultbuf == NULL){
		UNLHAputs(noMemMes);
		return 1;
	}

	comLen = 0;
	for (i = 1;i < argc;i++){
		comLen += strlen(argv[i]) + 1;
	}
	comLen++;	// for '\0'
	combuf = malloc(comLen);
	if (combuf == NULL){
		UNLHAputs(noMemMes);
		free(resultbuf);
		return 1;
	}
	combuf[0] = '\0';
	for (i = 1;i < argc;i++){
		strcat(combuf,argv[i]);
		strcat(combuf," ");
	}

	/* The code of your application goes here */
    /* Loading UNLHA32.DLL */
	if((hInstDLL=LoadLibrary("UnPma32.DLL"))==NULL) {
		UNLHAputs("UnPma32.DLL not found.\n");
		free(combuf);
		free(resultbuf);
		return 1;
	}
	Unlha = (TFUNC)GetProcAddress(hInstDLL,"UnPma");
	if (Unlha == NULL){
		UNLHAputs("Can't get function address.\n");
		free(combuf);
		free(resultbuf);
		return 1;
	}
	result = Unlha(NULL,combuf,resultbuf,RBUFSIZE);
	FreeLibrary(hInstDLL);

	UNLHAputs(resultbuf);

	free(combuf);
	free(resultbuf);

	return result;
}

