//
// DateUtil.c
// “ú•t•ÏŠ·
//
// $Id: DateUtil.c,v 1.2 2005/10/10 14:18:28 halpussy Exp $
//

#include "../HALsdk.h"

#define YEAR_SEC  31536000L  // ‚P”N‚Ì•b”
#define DAY_SEC   86400L     // ‚P“ú‚Ì•b”    60 * 60 * 24
#define HOUR_SEC  3600L      // ‚PŽžŠÔ‚Ì•b”  60 * 60
#define MIN_SEC   60L        // ‚P•bŠÔ‚Ì•b”  60
#define SEVENTIES_SEC  315532800L // 1970-1979 ”N‚Ì•b”

unsigned long FileTimeToUTC(const FILETIME in){
	return (ULONG)( (ULONGLONG)((in.dwHighDateTime*0x100000000+in.dwLowDateTime-0x19DB1DED53E8000)/10000000) );
}

FILETIME UTCToFileTime(const DWORD UTC){
	FILETIME ftime;
	ULONGLONG ll = ((DWORDLONG)(UTC)*(DWORDLONG)(10000000)) + 116444736000000000; //0x19DB1DED53E8000
	ftime.dwLowDateTime  = (DWORD) ll;
	ftime.dwHighDateTime = (DWORD)(ll >>32);
	return ftime;
}

bool FileTimeToDosTime(const FILETIME *in, WORD *Date, WORD *Time){
	FILETIME ftLocal;
	SYSTEMTIME st;
	if( !FileTimeToLocalFileTime( in, &ftLocal ) )	return false;
	if( !FileTimeToSystemTime( &ftLocal, &st) )		return false;

	if(st.wYear>2107) return false;

	if(Date) *Date = ((st.wYear-1980)<<9)+(st.wMonth<<5)+(st.wDay);
	if(Time) *Time = (st.wHour<<11)+(st.wMinute<<5)+((st.wSecond+1)>>1);

	return true;
}

bool IsInterCalary(const int iYear){
	//Year‚Í‚¤‚é‚¤”NH
	if((iYear/4)*4==iYear){
		if((iYear/400)*400==iYear)
			return true; //400‚ÅŠ„‚èØ‚ê‚é (Y2K‘æ“ñ’e‚Å‚µ‚½‚Ë`‚»‚¤‚¢‚¦‚Î(^^;;;)
		else if((iYear/100)*100==iYear)
			return false; //100‚ÅŠ„‚èØ‚ê‚ê‚Î‚¤‚é‚¤”N‚Å‚È‚¢
		else
			return true; //100‚ÅŠ„‚èØ‚ê‚È‚¢‚ª4‚ÅŠ„‚èØ‚ê‚é
	}else
		return false; //4‚ÅŠ„‚èØ‚ê‚È‚¢
}

unsigned long DosTimeToUTC(const WORD Date, const WORD Time){
	return FileTimeToUTC(DosTimeToFileTime(Date,Time));
}

FILETIME DosTimeToFileTime(const WORD Date, const WORD Time){
	FILETIME ft;
	SYSTEMTIME st;

	NullMemory(&ft, sizeof(FILETIME));
	NullMemory(&st, sizeof(SYSTEMTIME));

	st.wYear   = (Date >> 9) + 1980;
	st.wMonth  = (Date >> 5) & 0x0F;
	st.wDay    = (Date & 0x1F);
	st.wHour   = (Time >> 11);
	st.wMinute = (Time >> 5) & 0x3F;
	st.wSecond = (Time & 0x1F) << 1;

	//•ÛŒ¯B
	if(st.wYear<1601) st.wYear = 1601;
	if(st.wYear>30827) st.wYear = 30827;
	if(!st.wMonth) st.wMonth = 1;
	if(!st.wDay) st.wDay = 1;

	if(!SystemTimeToFileTime(&st,&ft))
		ErrorMsg();
	return ft;
}