//
// UnPma.cpp
// -pm2- decoder of PmaStream.
// Based on LHa for Unix, PMA extension
// LHA-PMA for UNIX V2 (lha-pma-2.tar.gz)
//

#include "CPmaStream.h"
#include "../HALsdk/HALsdk.h"

void PmaStream::DecodeInit(){
	//変数
	tree1.root=0, tree1.leftarr=tree1left, tree1.rightarr=tree1right;
	tree2.root=0, tree2.leftarr=tree2left, tree2.rightarr=tree2right;

	historyBits[0]=3,	historyBits[1]=3,	historyBits[2]=4,	historyBits[3]=5,
	historyBits[4]=5,	historyBits[5]=5,	historyBits[6]=6,	historyBits[7]=6;
	historyBase[0]=0,	historyBase[1]=8,	historyBase[2]=16,	historyBase[3]=32,
	historyBase[4]=64,	historyBase[5]=96,	historyBase[6]=128,	historyBase[7]=192;
	repeatBits[0]=3,	repeatBits[1]=3,	repeatBits[2]=5,
	repeatBits[3]=6,	repeatBits[4]=7,	repeatBits[5]=0;
	repeatBase[0]=17,	repeatBase[1]=25,	repeatBase[2]=33,
	repeatBase[3]=65,	repeatBase[4]=129,	repeatBase[5]=256;

	//InitGetBits
	wBitBuf=0;
	bSubBitBuf=0;
	bBitCount=0;
	FillBuf(16);

	HistInit();

	llNextCount = 0;
	wLastUpdate = 0;

	GetBits(1);
}

WORD PmaStream::Decode1(){
	while (wLastUpdate != wLocation){
		HistUpdate(szText[wLastUpdate]);
		wLastUpdate = (USHORT)( (wLastUpdate + 1) & wDict1 );
	}

	/*while*/if (llWrite >= llNextCount){
		if (llNextCount == 0x0000){
			MakeTree1();
			MakeTree2(5);
			llNextCount = 0x0400;
		}else if (llNextCount == 0x0400){
			MakeTree2(6);
			llNextCount = 0x0800;
		}else if (llNextCount == 0x0800){
			MakeTree2(7);
			llNextCount = 0x1000;
		}else if (llNextCount == 0x1000){
			if (GetBits(1))
				MakeTree1();
			MakeTree2(8);
			llNextCount = 0x2000;
		}else{
			// 0x2000, 0x3000, 0x4000, ...
			if (GetBits(1)){
				MakeTree1();
				MakeTree2(8);
			}
			llNextCount += 0x1000;
		}
	}
	bGetTree1 = TreeGet(&tree1);

    if (bGetTree1 < 8)
		return HistLookup(historyBase[bGetTree1] + GetBits(historyBits[bGetTree1]));
    // ret > 255
	if (bGetTree1 < 23)
		return (USHORT)(0x100 /*wOffset + 2*/ + (bGetTree1 - 8));
    return (USHORT)(wOffset + repeatBase[bGetTree1 - 23] + GetBits(repeatBits[bGetTree1 - 23]));
}

WORD PmaStream::Decode2(){
	if (bGetTree1 == 8){
		// 2-byte repeat with offset 0..63
		return GetBits(6);
	}
	if (bGetTree1 < 28){
		// n-byte repeat with offset 0..8191
		BYTE bGetTree2 = TreeGet(&tree2);
		if (!bGetTree2){
			return GetBits(6);
		}else{
			// bGetTree == 1..7
			BYTE bBits = 5 + bGetTree2;
			return (WORD)( (1 << bBits) + GetBits(bBits) );
		}
	}
	// 256 bytes repeat with offset 0
	//nbits = 0; delta = 0;
	return 0;
}

void PmaStream::HistInit(){
	int i=0;
	for(;i<0x100;i++){
		prev[(BYTE)(i-1)] = i;
		next[(BYTE)(i+1)] = i;
	}
	prev[0x7F] = 0x00; next[0x00] = 0x7F;
	prev[0xDF] = 0x80; next[0x80] = 0xDF;
	prev[0x9F] = 0xE0; next[0xE0] = 0x9F;
	prev[0x1F] = 0xA0; next[0xA0] = 0x1F;
	prev[0xFF] = 0x20; next[0x20] = 0xFF;
	bLastByte = 0x20;
}

BYTE PmaStream::HistLookup(BYTE bBytes){
	BYTE b=bLastByte;

	BYTE *direction = bBytes < 0x80 ? prev : next;

	if(bBytes>=0x80)
		bBytes*=-1; //bBytes=0x100-bBytes

	while(bBytes--)
		b = direction[b];
	return b;
}

void PmaStream::HistUpdate(BYTE bData){
    BYTE oldNext, oldPrev, newNext;
    if (bData == bLastByte) return;
    	
    // detach from old position
    oldNext			= next[bData];
    oldPrev			= prev[bData];
    prev[oldNext]	= oldPrev;
    next[oldPrev]	= oldNext;
	
    // attach to new next
    newNext			= next[bLastByte];
    prev[newNext]	= bData;
    next[bData]		= newNext;
    	
    // attach to new prev
    prev[bData]		= bLastByte;
    next[bLastByte]	= bData;
    	
    bLastByte		= bData;
}

void PmaStream::MakeTree1(){
	BYTE b, bBit, bx;

	bTree1Bound = (BYTE)GetBits(5);
	bMinDepth = (BYTE)GetBits(3);

    if (!bMinDepth)
		tree1.root = 128 | (bTree1Bound - 1);
	else{
		for (b = 0; b<32; b++)
			table1[b] = 0;
		bBit = (BYTE)GetBits(3);

		for (b = 0; b < bTree1Bound; b++) {
			bx = (BYTE)GetBits(bBit);
			if(bx)
				table1[b] = bx - 1 + bMinDepth;
		}
		TreeRebuild(&tree1, bTree1Bound, bMinDepth, table1);
    }
}

void PmaStream::MakeTree2(BYTE bParB){ // in use: 5 <= bParB <= 8
    BYTE b=0, bCount=0, bIndex=0;

    if (bTree1Bound < 10) return;
    if (bTree1Bound == 29 && !bMinDepth) return;

	for (; b < 8; b++)
		table2[b] = 0;
	for (b = 0; b < bParB; b++)
		table2[b] = (BYTE)GetBits(3);

	for (b = 0; b < 8; b++){
		if (table2[b]){
			bIndex = b;
			bCount++;
		}
	}

	if (bCount == 1){
		tree2.root = 128 | bIndex;
		return;
	}
	if (bCount){
		bMinDepth = 1;
		TreeRebuild(&tree2, 8, bMinDepth, table2);
	}
}

BYTE PmaStream::TreeGet(TREE *t){
	BYTE b = t->root;
	while (b < 0x80){
		b = !GetBits(1) ? t->leftarr[b] : t->rightarr[b];
    }
    return b + 0x80; //上位1bitを捨てる
}

void PmaStream::TreeRebuild(TREE *t, BYTE bBound, BYTE bMinDepth, BYTE *table){
    BYTE *parentarr;
    BYTE b, bCurrent=bMinDepth-1, bEmpty=bMinDepth, d=bMinDepth, n;

    parentarr = (BYTE*)malloc(bBound * sizeof(unsigned char));
    t->root = 0;
    for (b = 0; b < bBound; b++)
    {
        t->leftarr[b]   = 0;
        t->rightarr[b]  = 0;
        parentarr[b] = 0;
    }

    for (b = 0; b < bMinDepth - 1; b++)
    {
       	t->leftarr[b] = b + 1;
       	parentarr[b+1] = b;
    }

	for(;;d++){
		for (b = 0; b < bBound; b++){
			if (table[b] == d){
				if (!t->leftarr[bCurrent])
					t->leftarr[bCurrent] = b | 0x80;
				else{
					t->rightarr[bCurrent] = b | 0x80;

					n = 0;
					while (t->rightarr[bCurrent]){
						if (!bCurrent){ // root? -> done
							free(parentarr);
							return;
						}
						bCurrent = parentarr[bCurrent];
						n++;
					}

					t->rightarr[bCurrent] = bEmpty;
					for (;;){
						parentarr[bEmpty] = bCurrent;
						bCurrent = bEmpty;
						bEmpty++;
						n--;

						if (!n) break;
						t->leftarr[bCurrent] = bEmpty;
					}
				}
			}
		}
       	if (!t->leftarr[bCurrent])
			t->leftarr[bCurrent] = bEmpty;
		else
			t->rightarr[bCurrent] = bEmpty;
		parentarr[bEmpty] = bCurrent;
		bCurrent = bEmpty;
		bEmpty++;
	}
}

USHORT PmaStream::GetBits(BYTE bBit){
	//wBitBufの上位bBitビットを返す。
	//処理終了後はwBitBufを左にbBit回シフトして不必要な部分を除去。

	//上位ビットが有効。

	USHORT ret;
/*
printf("GBbegin %u %u %04X\n",bBit,bBitCount,wBitBuf);
	if(bBitCount>=bBit){
		ret = wBitBuf >> (16-bBit);
		wBitBuf<<=bBit;
		bBitCount-=bBit;
printf("GBend0 %u %04X %04X\n",bBitCount,wBitBuf,ret);
		return ret;
	}else{
		DWORD dw;

		if(!bBitCount){
			//二文字読む
			ReadFile(mhArc, &wBitBuf, 2, &dw, 0);
			wBitBuf=RotateLeft(wBitBuf, 8, 16); //CHAR_BIT*2
printf("--1 %04X\n",wBitBuf);
			bBitCount=16;

			ret = wBitBuf >> (16-bBit);
			wBitBuf<<=bBit;
			bBitCount-=bBit;
printf("GBend3 %u %04X %04X\n",bBitCount,wBitBuf,ret);
			return ret;
		}

		if(bBitCount<=8){
			//一文字読む
			USHORT us;
			//wBitBuf<<=8;
			ReadFile(mhArc, &us, 1, &dw, 0);
printf("--2 %02X\n",us);
			wBitBuf += (us<< (8-bBitCount) ); //上位bBitCount+8ビットが埋まった
			bBitCount+=8;

			if(bBitCount>=bBit){
				ret = wBitBuf >> (16-bBit);
				wBitBuf<<=bBit;
				bBitCount-=bBit;
printf("GBend1 %u %04X %04X\n",bBitCount,wBitBuf,ret);
				return ret;
			}
		}

		ret = wBitBuf >> (16-bBit); //この時点でbBit中上位bBitCountビットはOK
		wBitBuf=0;
		ReadFile(mhArc, &wBitBuf, 1, &dw, 0);
		wBitBuf<<=8; //上位8ビットが埋まった
		ret += wBitBuf >> (16-bBit+bBitCount);

		//8ビット中8-(bBit-bBitCount)ビット残すから、bBit-bBitCountビット消去する
		wBitBuf<<=bBit-bBitCount;
		bBitCount = 8-bBit+bBitCount;
printf("GBend2 %u %04X %04X\n",bBitCount,wBitBuf,ret);
		return ret;
	}
*/
	ret = wBitBuf >> (16 - bBit);  FillBuf(bBit);
	return ret;
}

void PmaStream::FillBuf(BYTE bBit){
	DWORD dw;

	while (bBit > bBitCount){
		bBit -= bBitCount;
		wBitBuf = (wBitBuf << bBitCount) + (bSubBitBuf >> (8 - bBitCount));
		if (mSubInfoEx.llCompressedSize) {
			mSubInfoEx.llCompressedSize--;
			ReadFile(mhArc, &bSubBitBuf, 1, &dw, 0);
		}else
			bSubBitBuf = 0;
		bBitCount = 8;
	}
	bBitCount -= bBit;
	wBitBuf = (wBitBuf << bBit) + (bSubBitBuf >> (8 - bBit));
	bSubBitBuf <<= bBit;
}