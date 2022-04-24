#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Result buffer size
#define RBUFSIZE 1024000

void UNPmaPuts(const char *s)
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
	UNPmaPuts("UnPma32.exe By Tatsuhiko Syoji 2001,2022\n"
		"Usage:\n"
		"UnPma32 [command] [<-|/>switches...] <arcfile> [dir\\] [file...|@filelist]\n"
		"command:\n"
		"l listing.\n"
		"c(v) listing with path.\n"
		"e Extract\n"
		"x Extract with path\n"
		"\n"
		"switches:\n"
		"--? End command line parseing[--@ before archive file name]\n"
		"-a  Extract attribute [-a1]\n"
		"-b  Leave broken file [-b0]\n"
		"-e  Treat warning as error [-e0]\n"
		"-f  Force overwrite [-f0]\n"
		"-n  Supress window [-n0]\n"
		"-p  Match all path [-p0]\n"
		"-t  Extract time stamp [-t1]\n"
		"-w  Use specified working folder [-w.]\n"
		"\n"
		"arcfile:\n"
		"Target archive file.\n"
		"dir:\n"
		"Extract target directory.\n"
		"file:\n"
		"Files to extract.\n"
		"filelist:\n"
		"Extract files name listing file.If you don't specify --?, you must add '@' in listing file name.\n"
	);
}

const char *noMemMes = "No enough memory.\n";
typedef int (WINAPI *TFUNC)(HWND,char *,char *,unsigned int);

int main(int argc,char *argv[])
{
    static HINSTANCE   hInstDLL;       /*  ‚c‚k‚k‚ÌƒCƒ“ƒXƒ^ƒ“ƒXƒnƒ“ƒhƒ‹                */
    static TFUNC       Unpma;    /*  ‚c‚k‚k‚Ì‚l‚™‚e‚•‚‚ƒ‚”‚‰‚‚‚Ö‚Ìƒ|ƒCƒ“ƒ^    */
	char *combuf,*resultbuf;
	int i,comLen,result;

	if (argc == 1) {
		/* If no arguments we call the Usage routine and exit */
		Usage();
		return 1;
	}

	resultbuf = malloc(RBUFSIZE);
	if (resultbuf == NULL){
		UNPmaPuts(noMemMes);
		return 1;
	}

	comLen = 0;
	for (i = 1;i < argc;i++){
		comLen += strlen(argv[i]) + 1;
	}
	comLen++;	// for '\0'
	combuf = malloc(comLen);
	if (combuf == NULL) {
		UNPmaPuts(noMemMes);
		free(resultbuf);
		return 1;
	}
	combuf[0] = '\0';
	for (i = 1;i < argc;i++){
		strcat(combuf,argv[i]);
		strcat(combuf," ");
	}

	/* The code of your application goes here */
    /* Loading UnPma32.DLL */
	hInstDLL = LoadLibrary("UnPma32.DLL");
	if(hInstDLL == NULL) {
		UNPmaPuts("UnPma32.DLL not found.\n");
		free(combuf);
		free(resultbuf);
		return 1;
	}
	Unpma = (TFUNC)GetProcAddress(hInstDLL, "UnPma");
	if (Unpma == NULL) {
		UNPmaPuts("Can't get function address.\n");
		free(combuf);
		free(resultbuf);
		return 1;
	}
	result = Unpma(NULL, combuf, resultbuf, RBUFSIZE);
	FreeLibrary(hInstDLL);

	UNPmaPuts(resultbuf);

	free(combuf);
	free(resultbuf);

	return result;
}

