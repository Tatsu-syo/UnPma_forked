//
// Crc.c
// CRC
//
// $Id: Crc.c,v 1.1 2005/10/10 14:18:28 halpussy Exp $
//

#include "../HALsdk.h"

// CRC16: ˆê‰ñ–Ú‚ÌUpdate‚Í0
// CRC32: ˆê‰ñ–Ú‚ÌUpdate‚Í0xffffffff

bool InitCRC16(crc16table *crc16){
	UINT i, j, r;
	if(!crc16)
		return false;

	for(i=0;i<256;i++){
		r=i;
		for(j=0;j<8;j++){
			if(r&1)
				r = (r>>1)^0xa001;
			else
				r >>= 1;
		}
		(*crc16)[i] = r;
	}
	return true;
}

USHORT UpdateCRC16(crc16table crc16, BYTE* p, DWORD dw, WORD crc){
	if(!p)
		return -1;
	while(dw--)
		crc = crc16[(crc ^ (*p++)) & 0xFF] ^ (crc >> 8);
	return crc;
}

bool InitCRC32(crc32table *crc32){
	UINT i, j, r;
	if(!crc32)
		return false;

	for(i=0;i<256;i++){
		r=i;
		for(j=0;j<8;j++){
			if(r&1)
				r = (r>>1)^0xedb88320;
			else
				r >>= 1;
		}
		(*crc32)[i] = r;
	}
	return true;
}

UINT UpdateCRC32(crc32table crc32, BYTE* p, DWORD dw, UINT crc){
	if(!p)
		return -1;
	while(dw--)
		crc = crc32[(crc ^ (*p++)) & 0xFF] ^ (crc >> 8);
	return crc;
}

UINT DoneCRC32(UINT crc){
	return crc^0xffffffff;
}