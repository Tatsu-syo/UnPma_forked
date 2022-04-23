//
// DrvCtrl.c
// ƒhƒ‰ƒCƒuŠÇ—
//
// $Id: DrvCtrl.c,v 1.2 2005/10/30 03:40:03 halpussy Exp $
//

#include "../HALsdk.h"
#include <windows.h>
#include <limits.h>

typedef BOOL (WINAPI*GetDiskFreeSpaceEx_proc)(LPCTSTR,PULARGE_INTEGER,PULARGE_INTEGER,PULARGE_INTEGER);

bool CheckDiskSize(const char *drive,ULONGLONG size){
	ULONGLONG freesize;
	int iOSType=GetOSType();
	if (!drive)
		return false;

	if(iOSType>WIN95OSR1&&iOSType!=WINNT3){
		ULARGE_INTEGER ulFreeBytesAvailableToCaller;
		ULARGE_INTEGER ulTotalNumberOfFreeBytes;
		ULARGE_INTEGER ulTotalNumberOfBytes;
		GetDiskFreeSpaceEx_proc pGetDiskFreeSpaceEx;

		pGetDiskFreeSpaceEx = (GetDiskFreeSpaceEx_proc)GetProcAddress(GetModuleHandle("kernel32"),"GetDiskFreeSpaceExA");
		if (!pGetDiskFreeSpaceEx)
			return false;
		pGetDiskFreeSpaceEx(drive,&ulFreeBytesAvailableToCaller,&ulTotalNumberOfBytes,&ulTotalNumberOfFreeBytes);

		freesize = ulTotalNumberOfFreeBytes.QuadPart;
	}else{
		DWORD Secs,SecBytes,FreeClusters,Clusters;
		GetDiskFreeSpace(drive,&Secs,&SecBytes,&FreeClusters,&Clusters);
		freesize = FreeClusters * Secs * SecBytes;
	}

	if (freesize > size)
		return true;
	return false;
}

bool CheckFileSystem(const char *drive,ULONGLONG size){
	DWORD MaximumComponentLength;
	DWORD FileSystemFlags;
	char FileSystemNameBuffer[50];
	if (!drive)
		return false;

	if (!GetVolumeInformation(drive,NULL,0,NULL,&MaximumComponentLength,&FileSystemFlags,FileSystemNameBuffer,50))
		return false;

	if (!strcmp(FileSystemNameBuffer,"FAT") || !strcmp(FileSystemNameBuffer,"FAT16")){
		if (size > LONG_MAX)
			return false;
	}else if (!strcmp(FileSystemNameBuffer,"FAT32")){
		if (size > ULONG_MAX)
			return false;
	}else if (!lstrcmpi(FileSystemNameBuffer,"NTFS")){

#if defined(LCC)
		if (size > LLONG_MAX)
#else
		if (size > _I64_MAX)
#endif

		return false;
	}else{
		return false;
	}
	return true;
}
