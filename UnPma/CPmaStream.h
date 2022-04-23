#ifndef CPMASTREAM_H
#define CPMASTREAM_H

#include "../HArcLib/CommArc.h"

typedef BOOL (WINAPI*PMA_CALLBACK)(void*,INT64);

typedef struct{
	BYTE root, *leftarr, *rightarr;
}TREE;

class PmaStream
{
public:
	PmaStream();
	virtual ~PmaStream();

	DWORD	GetLastError();
	void	SetCallback(void* pParam, PMA_CALLBACK pfnCallback);

	bool	Open(LPCSTR lpszArc);	//ファイルを開く
	bool	Close();				//ファイルを閉じる
	bool	SeekTop();				//先頭へシーク

	bool	ReadHeader(LPINDIVIDUALINFOEX lpSubInfoEx);	//ヘッダ読み込み
	BYTE	GetMemo(char *lpszBuf, DWORD dwLen);
	bool	Extract(LPCSTR lpszDest);		//解凍

	INT64	GetFilePointer();
	bool	SetFilePointer(INT64 llPointer);	//liPointerにはGetFilePointerの返し値を使うこと。

	operator HANDLE() const{return mhArc;} //このハンドルを使う場合は十分に注意すること。

private: //PMA Stream
	void	Init();
	BYTE	HeaderSum(BYTE *b, BYTE cb);

	HANDLE			mhArc;
	DWORD			mdwLastError;
	LARGE_INTEGER	mliPointer;

	bool			mbEOF;
	bool			mbOnFile;

	BYTE			*mlpszHeader;
	BYTE			mcbHeader;
	INDIVIDUALINFOEX	mSubInfoEx;

	BYTE			mszMemo[74];
	BYTE			mcbMemo;

	PMA_CALLBACK	mpfnCallback;
	void			*mpParam;

	BYTE			*szText;
	INT64			llWrite; //-pm2-
	WORD			wLocation;

	const WORD		wDict, wDict1, wOffset;

private: //PMA Decoder

	void	DecodeInit();
	WORD	Decode1();
	WORD	Decode2();
	void	HistInit();
	BYTE	HistLookup(BYTE bBytes);
	void	HistUpdate(BYTE bData);
	void	MakeTree1();
	void	MakeTree2(BYTE bParB);
	BYTE	TreeGet(TREE *t);
	//void	TreeSetSingle(TREE *t, BYTE bValue);
	void	TreeRebuild(TREE *t, BYTE bBound, BYTE bMinDepth, BYTE *table);
	WORD	GetBits(BYTE bBit);
	void	FillBuf(BYTE bBit);

	//Decoder
	INT64	llNextCount;
	WORD	wLastUpdate;
	BYTE	bGetTree1;
	int		historyBits[8]; // = {   3,   3,   4,   5,   5,   5,   6,   6 };
	int		historyBase[8]; // = {   0,   8,  16,  32,  64,  96, 128, 192 };
	int		repeatBits[6]; //  = {   3,   3,   5,   6,   7,   0 };
	int		repeatBase[6]; //  = {  17,  25,  33,  65, 129, 256 };

	//History
	BYTE	prev[0x100];
	BYTE	next[0x100];
	BYTE	bLastByte;

	//Tree
	BYTE	tree1left[32];
	BYTE	tree1right[32];
	TREE	tree1;
	BYTE	table1[32];

	BYTE	tree2left[8];
	BYTE	tree2right[8];
	TREE	tree2;
	BYTE	table2[8];

	BYTE	bTree1Bound;
	BYTE	bMinDepth;

	//Bit In
	BYTE	bSubBitBuf;
	WORD	wBitBuf;
	BYTE	bBitCount;

};

#endif