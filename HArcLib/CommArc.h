//
// CommArc.h
// Definitions of Archiver DLL.
//
// $Id: CommArc.h,v 1.4 2005/10/30 04:19:46 halpussy Exp $
//

#ifndef HARCLIB_COMMARC_H
#define HARCLIB_COMMARC_H

#include "../HALsdk/Macro.h"

#include <wtypes.h>

#define FNAME_MAX32     512

/* Types */
#if defined(__BORLANDC__)
#pragma option -a-
#else
#pragma pack(1)
#endif

#if defined(__BORLANDC__) && __BORLANDC__ < 0x550
typedef struct
{
  DWORD   LowPart;
  LONG    HighPart;
} INT64, *LPINT64;
#endif

typedef HGLOBAL HARC;

typedef struct
{
	DWORD	dwOriginalSize;					// ファイルサイズ
	DWORD	dwCompressedSize;				// 圧縮イメージサイズ
	DWORD	dwCRC;							// チェックサム
	UINT	uFlag;							// 処理結果
	UINT	uOSType;						// OSを示す値
	WORD	wRatio;							// 圧縮率
	WORD	wDate;							// 日付(DOS形式)
	WORD	wTime;							// 時間(DOS形式)
	char	szFileName[FNAME_MAX32+1];		// ファイル名
	char	dummy1[3];						// 拡張領域
	char	szAttribute[8];					// 属性
	char	szMode[8];						// 圧縮方法
} INDIVIDUALINFO, *LPINDIVIDUALINFO;

typedef struct
{
	INT64		llOriginalSize;				// ファイルサイズ
	INT64		llCompressedSize;			// 圧縮イメージサイズ
	DWORD		dwCRC;						// チェックサム
	UINT		uFlag;						// 処理結果
	UINT		uOSType;					// OSを示す値
	WORD		wRatio;						// 圧縮率
	FILETIME	ftCreateTime;				// 作成時間
	FILETIME	ftAccessTime;				// 参照時間
	FILETIME	ftWriteTime;				// 更新時間
	DWORD		dwAttributes;				// 属性
	char		szFileName[FNAME_MAX32 + 1];// ファイル名
	char		szMode[32];					// 圧縮方法
	char        szReserved[32];				// 拡張領域
} INDIVIDUALINFOEX, *LPINDIVIDUALINFOEX;

typedef struct
{
	DWORD	dwFileSize;						// ファイルサイズ
	DWORD	dwWriteSize;					// 書き込んだサイズ
	char	szSourceFileName[FNAME_MAX32+1];// 入力ファイル名
	char	dummy1[3];						// 拡張領域
	char	szDestFileName[FNAME_MAX32+1];	// 出力ファイル名
	char	dummy[3];						// 拡張領域
} EXTRACTINGINFO, *LPEXTRACTINGINFO;

typedef struct
{
	EXTRACTINGINFO	exinfo;					// EXTRACTINGINFO構造体
	DWORD 			dwCompressedSize;		// 圧縮イメージサイズ
	DWORD			dwCRC;					// チェックサム
	UINT			uOSType;				// OSの種類
	WORD			wRatio;					// 圧縮率
	WORD			wDate;					// 日付(DOS形式)
	WORD			wTime;					// 時間(DOS形式)
	char			szAttribute[8];			// 属性
	char			szMode[8];				// 格納方法
} EXTRACTINGINFOEX, *LPEXTRACTINGINFOEX;

typedef struct
{
	DWORD			dwStructSize;			// 構造体サイズ
	EXTRACTINGINFO	exinfo;					// EXTRACTINGINFO構造体
	DWORD			dwFileSize;				// ファイルサイズ
	DWORD			dwCompressedSize;		// 圧縮イメージサイズ
	DWORD			dwWriteSize;			// 書き込んだサイズ
	DWORD			dwAttributes;			// 属性
	DWORD			dwCRC;					// チェックサム
	UINT			uOSType;				// OSの種類
	WORD			wRatio;					// 圧縮率
	FILETIME		ftCreateTime;			// 作成時間
	FILETIME		ftAccessTime;			// 参照時間
	FILETIME		ftWriteTime;			// 更新時間
	char            szMode[8];				// 格納方法
	char            szSourceFileName[FNAME_MAX32+1];// 入力ファイル名
	char            dummy1[3];				// 拡張領域
	char            szDestFileName[FNAME_MAX32+1];	// 出力ファイル名
	char            dummy2[3];				// 拡張領域
} EXTRACTINGINFOEX32, *LPEXTRACTINGINFOEX32;

typedef struct
{
	DWORD			dwStructSize;			// 構造体サイズ
	EXTRACTINGINFO	exinfo;					// EXTRACTINGINFO構造体
	INT64			llFileSize;				// ファイルサイズ
	INT64			llCompressedSize;		// 圧縮イメージサイズ
	INT64			llWriteSize;			// 書き込んだサイズ
	DWORD			dwAttributes;			// 属性
	DWORD			dwCRC;					// チェックサム
	UINT			uOSType;				// OSの種類
	WORD			wRatio;					// 圧縮率
	FILETIME		ftCreateTime;			// 作成時間
	FILETIME		ftAccessTime;			// 参照時間
	FILETIME		ftWriteTime;			// 更新時間
	char			szMode[8];				// 格納方法
	char			szSourceFileName[FNAME_MAX32+1];// 入力ファイル名
	char			dummy1[3];				// 拡張領域
	char			szDestFileName[FNAME_MAX32+1];	// 出力ファイル名
	char			dummy2[3];				// 拡張領域
} EXTRACTINGINFOEX64, *LPEXTRACTINGINFOEX64;

typedef BOOL CALLBACK ARCHIVERPROC(HWND _hwnd, UINT _uMsg, UINT _nState, LPVOID _lpEis);
typedef ARCHIVERPROC *LPARCHIVERPROC;

typedef struct
{
	DWORD		dwStructSize;				// 構造体サイズ
	UINT		uCommand;					// 命令
	DWORD		dwOriginalSize;				// ファイルサイズ
	DWORD		dwCompressedSize;			// 圧縮イメージサイズ
	DWORD		dwAttributes;				// 属性
	DWORD		dwCRC;						// チェックサム
	UINT		uOSType;					// OSの種類
	WORD		wRatio;						// 圧縮率
	FILETIME	ftCreateTime;				// 作成時間
	FILETIME	ftAccessTime;				// 参照時間
	FILETIME	ftWriteTime;				// 更新時間
	char		szFileName[FNAME_MAX32+1];	// 入力ファイル名
	char		dummy1[3];					// 拡張領域
	char		szAddFileName[FNAME_MAX32+1];	// 格納ファイル名
	char		dummy2[3];					// 拡張領域
} ENUM_MEMBER_INFO, *LPENUM_MEMBER_INFO;

typedef struct
{
	DWORD		dwStructSize;				// 構造体サイズ
	UINT		uCommand;					// 命令
	INT64		llOriginalSize;				// ファイルサイズ
	INT64		llCompressedSize;			// 圧縮イメージサイズ
	DWORD		dwAttributes;				// 属性
	DWORD		dwCRC;						// チェックサム
	UINT		uOSType;					// OSの種類
	WORD		wRatio;						// 圧縮率
	FILETIME	ftCreateTime;				// 作成時間
	FILETIME	ftAccessTime;				// 参照時間
	FILETIME	ftWriteTime;				// 更新時間
	char		szFileName[FNAME_MAX32+1];	// 入力ファイル名
	char		dummy1[3];					// 拡張領域
	char		szAddFileName[FNAME_MAX32+1];	// 格納ファイル名
	char		dummy2[3];					// 拡張領域
} ENUM_MEMBER_INFO64, *LPENUM_MEMBER_INFO64;

typedef BOOL (CALLBACK*WND_ENUMMEMBPROC)(LPVOID);
//typedef BOOL (CALLBACK*WND_ENUMMEMBPROCEX)(HWND, LPVOID);

#if !defined(__BORLANDC__)
#pragma pack()
#else
#pragma option -a.
#endif

/* Functions */
#define ISARC							0	// (Command)
#define ISARC_GET_VERSION				1	// GetVersion
#define ISARC_GET_CURSOR_INTERVAL		2	// GetCursorInterval
#define ISARC_SET_CURSOR_INTERVAL		3	// SetCursorInterval
#define ISARC_GET_BACK_GROUND_MODE		4	// GetBackGroundMode
#define ISARC_SET_BACK_GROUND_MODE		5	// SetBackGroundMode
#define ISARC_GET_CURSOR_MODE			6   // GetCursorMode
#define ISARC_SET_CURSOR_MODE			7   // SetCursorMode
#define ISARC_GET_RUNNING				8   // GetRunning

#define ISARC_CHECK_ARCHIVE             16  // CheckArchive
#define ISARC_CONFIG_DIALOG             17  // ConfigDialog
#define ISARC_GET_FILE_COUNT            18  // GetFileCount
#define ISARC_QUERY_FUNCTION_LIST       19  // QueryFunctionList
#define ISARC_HOUT                      20  // (HOut)
#define ISARC_STRUCTOUT                 21  // (StructOut)
#define ISARC_GET_ARC_FILE_INFO         22  // (GetArcFileInfo)

#define ISARC_OPEN_ARCHIVE              23  // OpenArchive
#define ISARC_CLOSE_ARCHIVE             24  // CloseArchive
#define ISARC_FIND_FIRST                25  // FindFirst
#define ISARC_FIND_NEXT                 26  // FindNext
#define ISARC_EXTRACT                   27  // Extract
#define ISARC_ADD                       28  // Add
#define ISARC_MOVE                      29  // Move
#define ISARC_DELETE                    30  // Delete
#define ISARC_SETOWNERWINDOW            31  // SetOwnerWindow
#define ISARC_CLEAROWNERWINDOW          32  // ClearOwnerWindow
#define ISARC_SETOWNERWINDOWEX          33  // SetOwnerWindowEx
#define ISARC_KILLOWNERWINDOWEX         34  // KillOwnerWindowEx

#define ISARC_GET_ARC_FILE_NAME         40  // GetArcFileName
#define ISARC_GET_ARC_FILE_SIZE         41  // GetArcFileSize
#define ISARC_GET_ARC_ORIGINAL_SIZE     42  // GetArcOriginalSize
#define ISARC_GET_ARC_COMPRESSED_SIZE   43  // GetArcCompressedSize
#define ISARC_GET_ARC_RATIO             44  // GetArcRatio
#define ISARC_GET_ARC_DATE              45  // GetArcDate
#define ISARC_GET_ARC_TIME              46  // GetArcTime
#define ISARC_GET_ARC_OS_TYPE           47  // GetArcOSType
#define ISARC_GET_ARC_IS_SFX_FILE       48  // GetArcIsSFXFile
#define ISARC_GET_ARC_WRITE_TIME_EX     49  // GetArcWriteTimeEx
#define ISARC_GET_ARC_CREATE_TIME_EX    50  // GetArcCreateTimeEx
#define ISARC_GET_ARC_ACCESS_TIME_EX    51  // GetArcAccessTimeEx
#define ISARC_GET_ARC_CREATE_TIME_EX2   52  // GetArcCreateTimeEx2
#define ISARC_GET_ARC_WRITE_TIME_EX2    53  // GetArcWriteTimeEx2
#define ISARC_GET_FILE_NAME             57  // GetFileName
#define ISARC_GET_ORIGINAL_SIZE         58  // GetOriginalSize
#define ISARC_GET_COMPRESSED_SIZE       59  // GetCompressedSize
#define ISARC_GET_RATIO                 60  // GetRatio
#define ISARC_GET_DATE                  61  // GetDate
#define ISARC_GET_TIME                  62  // GetTime
#define ISARC_GET_CRC                   63  // GetCRC
#define ISARC_GET_ATTRIBUTE             64  // GetAttribute
#define ISARC_GET_OS_TYPE               65  // GetOSType
#define ISARC_GET_METHOD                66  // GetMethod
#define ISARC_GET_WRITE_TIME            67  // GetWriteTime
#define ISARC_GET_CREATE_TIME           68  // GetCreateTime
#define ISARC_GET_ACCESS_TIME           69  // GetAccessTime
#define ISARC_GET_WRITE_TIME_EX         70  // GetWriteTimeEx
#define ISARC_GET_CREATE_TIME_EX        71  // GetCreateTimeEx
#define ISARC_GET_ACCESS_TIME_EX        72  // GetAccessTimeEx
#define ISARC_SET_ENUM_MEMBERS_PROC     80  // SetEnumMembersProc
#define ISARC_CLEAR_ENUM_MEMBERS_PROC   81  // ClearEnumMembersProc
#define ISARC_GET_ARC_FILE_SIZE_EX      82  // GetArcFileSizeEx
#define ISARC_GET_ARC_ORIGINAL_SIZE_EX  83  // ArcOriginalSizeEx
#define ISARC_GET_ARC_COMPRESSED_SIZE_EX	84 // GetArcCompressedSizeEx
#define ISARC_GET_ORIGINAL_SIZE_EX      85  // GetOriginalSizeEx
#define ISARC_GET_COMPRESSED_SIZE_EX    86  // GetCompressedSizeEx

#define ISARC_SETOWNERWINDOWEX64        87  // SetOwnerWindowEx64
#define ISARC_KILLOWNERWINDOWEX64       88  // KillOwnerWindowEx64
#define ISARC_SET_ENUM_MEMBERS_PROC64   89  // SetEnumMembersProc64
#define ISARC_CLEAR_ENUM_MEMBERS_PROC64 90  // ClearEnumMembersProc64
#define ISARC_OPEN_ARCHIVE2             91  // OpenArchive2
#define ISARC_GET_ARC_READ_SIZE         92  // GetArcReadSize
#define ISARC_GET_ARC_READ_SIZE_EX      93  // GetArcReadSizeEx

#define SET_LANGUE_JAPANESE				94	// SetLangueJapanese
#define SET_LANGUE_ENGLISH				95	// SetLangueEnglish
#define SET_LANGUE_SPECIFIED			96	// SetLangueSpecified
#define ISARC_SET_LANGUE_SPECIFIED		97	// SetLangueSpecified
#define ISARC_SET_LANGUE_JAPANESE		98	// SetLangueJapanese
#define ISARC_SET_LANGUE_ENGLISH		99	// SetLangueEnglish
#define ISARC_SET_PRIORITY				100	// SetPriority

/* CheckArchive */
#define	CHECKARCHIVE_RAPID		0	// 簡易(最初の３ヘッダまで)
#define	CHECKARCHIVE_BASIC		1	// 標準(全てのヘッダ)
#define	CHECKARCHIVE_FULLCRC	2	// 完全(格納ファイルの CRC チェック)

/* ConfigDialog */
#define	UNPACK_CONFIG_MODE		1	// 解凍系の設定
#define PACK_CONFIG_MODE		2	// 圧縮系の設定

/* SetPriority */
#ifndef THREAD_PRIORITY_TIME_CRITICAL
#define THREAD_PRIORITY_TIME_CRITICAL	15
#define THREAD_PRIORITY_HIGHEST			2
#define THREAD_PRIORITY_ABOVE_NORMAL	1
#define THREAD_PRIORITY_NORMAL			0
#define THREAD_PRIORITY_BELOW_NORMAL	-1
#define THREAD_PRIORITY_LOWEST			-2
#define THREAD_PRIORITY_IDLE			-15
#endif

/* ArchiverProc */
#define	WM_ARCEXTRACT	"wm_arcextract"
#define	ARCEXTRACT_BEGIN		0	// 該当ファイルの処理の開始
#define	ARCEXTRACT_INPROCESS	1	// 該当ファイルの展開中
#define	ARCEXTRACT_END			2	// 処理終了、関連メモリを開放
#define ARCEXTRACT_OPEN			3	// 該当書庫の処理の開始
#define ARCEXTRACT_COPY			4	// ワークファイルの書き戻し
#define	ARCEXTRACT_SEARCH		5	// ファイルのサーチ中
#define ARCEXTRACT_ADDEND		6	// 該当ファイルの格納の終了
#define ARCEXTRACT_CHECK		7	// 書庫のチェックの開始

/* OpenArchive */
#define M_INIT_FILE_USE			0x00000001L	// レジストリの設定を使用
#define M_REGARDLESS_INIT_FILE	0x00000002L	// 〃 を使用しない
#define M_NO_BACKGROUND_MODE	0x00000004L	// バックグラウンドを禁止
#define M_NOT_USE_TIME_STAMP	0x00000008L
#define M_EXTRACT_REPLACE_FILE	0x00000010L
#define M_EXTRACT_NEW_FILE		0x00000020L
#define M_EXTRACT_UPDATE_FILE	0x00000040L
#define M_CHECK_ALL_PATH		0x00000100L	// 厳格なファイル名サーチ
#define M_CHECK_FILENAME_ONLY	0x00000200L	// 〃を行わない
#define M_CHECK_DISK_SIZE		0x00000400L
#define M_REGARDLESS_DISK_SIZE	0x00000800L
#define M_USE_DRIVE_LETTER		0x00001000L	// ドライブ名から格納
#define M_NOT_USE_DRIVE_LETTER	0x00002000L	// 〃 を格納しない
#define M_INQUIRE_DIRECTORY		0x00004000L
#define M_NOT_INQUIRE_DIRECTORY 0x00008000L
#define M_INQUIRE_WRITE			0x00010000L
#define M_NOT_INQUIRE_WRITE		0x00020000L
#define M_CHECK_READONLY		0x00040000L
#define M_REGARDLESS_READONLY	0x00080000L
#define M_REGARD_E_COMMAND		0x00100000L
#define M_REGARD_X_COMMAND		0x00200000L
#define M_ERROR_MESSAGE_ON		0x00400000L	// エラーメッセージを表示
#define M_ERROR_MESSAGE_OFF		0x00800000L	// 〃を表示しない
#define M_BAR_WINDOW_ON			0x01000000L
#define M_BAR_WINDOW_OFF		0x02000000L
#define M_CHECK_PATH			0x04000000L
#define M_RECOVERY_ON			0x08000000L // 破損ヘッダの読み飛ばし

#define M_MAKE_INDEX_FILE		0x10000000L
#define M_NOT_MAKE_INDEX_FILE	0x20000000L
#define EXTRACT_FOUND_FILE		0x40000000L	// 検索されたファイルを解凍
#define EXTRACT_NAMED_FILE		0x80000000L	// 指定したファイルを解凍

/* IsSFXFile */
#define SFX_NOT 0

/* GetAttribute */
#define FA_RDONLY	0x01	// 書き込み保護属性
#define FA_HIDDEN	0x02	// 隠し属性
#define FA_SYSTEM	0x04	// システム属性
#define FA_LABEL	0x08	// ボリューム・ラベル
#define FA_DIREC	0x10	// ディレクトリ
#define FA_ARCH		0x20	// アーカイブ属性
#define FA_ENCRYPT	0x40	// パスワード保護属性

#define ERROR_START				0x8000
/* WARNING */
#define ERROR_DISK_SPACE		0x8005
#define ERROR_READ_ONLY			0x8006
#define ERROR_USER_SKIP			0x8007
#define ERROR_UNKNOWN_TYPE		0x8008
#define ERROR_METHOD			0x8009
#define ERROR_PASSWORD_FILE		0x800A
#define ERROR_VERSION			0x800B
#define ERROR_FILE_CRC			0x800C
#define ERROR_FILE_OPEN			0x800D
#define ERROR_MORE_FRESH		0x800E
#define ERROR_NOT_EXIST			0x800F
#define ERROR_ALREADY_EXIST		0x8010
#define ERROR_TOO_MANY_FILES	0x8011

/* ERROR */
#define ERROR_MAKEDIRECTORY		0x8012
#define ERROR_CANNOT_WRITE		0x8013
#define ERROR_HUFFMAN_CODE		0x8014
#define ERROR_COMMENT_HEADER	0x8015
#define ERROR_HEADER_CRC		0x8016
#define ERROR_HEADER_BROKEN		0x8017
#define ERROR_ARC_FILE_OPEN		0x8018
#define ERROR_NOT_ARC_FILE		0x8019
#define ERROR_CANNOT_READ		0x801A
#define ERROR_FILE_STYLE		0x801B
#define ERROR_COMMAND_NAME		0x801C
#define ERROR_MORE_HEAP_MEMORY	0x801D
#define ERROR_ENOUGH_MEMORY		0x801E
#define ERROR_ALREADY_RUNNING	0x801F
#define ERROR_USER_CANCEL		0x8020
#define ERROR_HARC_ISNOT_OPENED	0x8021
#define ERROR_NOT_SEARCH_MODE	0x8022
#define ERROR_NOT_SUPPORT		0x8023
#define ERROR_TIME_STAMP		0x8024
#define ERROR_TMP_OPEN			0x8025
#define ERROR_LONG_FILE_NAME	0x8026
#define ERROR_ARC_READ_ONLY		0x8027
#define ERROR_SAME_NAME_FILE	0x8028
#define ERROR_NOT_FIND_ARC_FILE 0x8029
#define ERROR_RESPONSE_READ		0x802A
#define ERROR_NOT_FILENAME		0x802B
#define ERROR_TMP_COPY			0x802C
#define ERROR_EOF				0x802D
#define ERROR_ADD_TO_LARC		0x802E
#define ERROR_TMP_BACK_SPACE	0x802F
#define ERROR_SHARING			0x8030
#define ERROR_NOT_FIND_FILE		0x8031
#define ERROR_LOG_FILE			0x8032
#define	ERROR_NO_DEVICE			0x8033
#define ERROR_GET_ATTRIBUTES	0x8034
#define ERROR_SET_ATTRIBUTES	0x8035
#define ERROR_GET_INFORMATION	0x8036
#define ERROR_GET_POINT			0x8037
#define ERROR_SET_POINT			0x8038
#define ERROR_CONVERT_TIME		0x8039
#define ERROR_GET_TIME			0x803a
#define ERROR_SET_TIME			0x803b
#define ERROR_CLOSE_FILE		0x803c
#define ERROR_HEAP_MEMORY		0x803d
#define ERROR_HANDLE			0x803e
#define ERROR_TIME_STAMP_RANGE	0x803f
#define ERROR_MAKE_ARCHIVE		0x8040
#define ERROR_NOT_CONFIRM_NAME	0x8041
#define ERROR_UNEXPECTED_EOF	0x8042
#define ERROR_INVALID_END_MARK	0x8043
#define ERROR_INVOLVED_ARCHIVE	0x8044
#define ERROR_NO_END_MARK		0x8045
#define ERROR_HDR_INVALID_SIZE	0x8046
#define ERROR_UNKNOWN_LEVEL		0x8047
#define ERROR_BROKEN_DATA		0x8048
#define ERROR_INVALID_PATH		0x8049
#define ERROR_TOO_BIG			0x804A
#define ERROR_EXECUTABLE_FILE	0x804B
#define ERROR_UNKNOWN			-1

/* EnumMembersProc */
#define LIST_COMMAND		1
#define ADD_COMMAND			2
#define FRESH_COMMAND		3
#define DELETE_COMMAND		4
#define EXTRACT_COMMAND		5
#define PRINT_COMMAND		6
#define TEST_COMMAND		7
#define MAKESFX_COMMAND		8
#define JOINT_COMMAND		9
#define CONVERT_COMMAND		10
#define RENAME_COMMAND		11

/* OSTYPE */
#define OSTYPE_MSDOS		0	/* MS-DOS */
#define OSTYPE_PRIMOS		1	/* PRIMOS */
#define OSTYPE_UNIX			2	/* unix */
#define OSTYPE_AMIGA		3	/* AMIGA */
#define OSTYPE_MACOS		4	/* MAC-OS */
#define OSTYPE_OS2			5	/* OS/2 */
#define OSTYPE_APPLE		6	/* APPLE GS */
#define OSTYPE_ATARI		7	/* ATARI ST */
#define OSTYPE_NEXT			8	/* NEXT */
#define OSTYPE_VMS			9	/* VAX VMS */
#define OSTYPE_UNKNOWN		10	/* その他 */
#define OSTYPE_OS9			11	/* OS9 */
#define OSTYPE_OS68K		12	/* OS/68K */
#define OSTYPE_OS386		13	/* OS/386 */
#define OSTYPE_HUMAN68K		14	/* Human68K */
#define OSTYPE_CPM			15	/* CP/M */
#define OSTYPE_FLEX			16	/* FLEX */
#define OSTYPE_RUNSER		17	/* Runser */
#define OSTYPE_WINDOWSNT	18	/* Windows NT(LHA) */
#define OSTYPE_CMS			18	/* VM CMS(others) */
#define OSTYPE_WINDOWS95	19	/* Windows 95(LHA) */
#define OSTYPE_ZSYSTEM		19	/* Z System(others) */
#define OSTYPE_TOPS20		20	/* TOPS20 */
#define OSTYPE_NTFS			21	/* NTFS */
#define OSTYPE_QDOS			22	/* QDOS */
#define OSTYPE_VFAT95		23	/* VFAT95 */
#define OSTYPE_MVS			24	/* MVS */
#define OSTYPE_BEBOX		25	/* Be Box */
#define OSTYPE_ERROR		(-1)/* エラー */

/* STID */
#define STID_FONTGOTICK			101
#define STID_FONTPGOTICK		102
#define STID_REG_STANDARD		103

#define STID_CONFDLG1			1001
#define STID_CONFDLG2			1002
#define STID_PROGRESSDLG		1003
#define STID_SIMPLEPROGDLG		1004

#define STID_GETFNAMEDLG		1011
#define STID_GETCMTDLG			1012
#define STID_GETWINSFXDLG		1013
#define STID_OVERWRITEDLG		1014
#define STID_MAKEDIRECTORYDLG	1015

#define STID_DLG_T_PROC			1101
#define STID_DLG_P_ARCNAME		1102
#define STID_DLG_P_FILENAME		1103
#define STID_DLG_P_EXTRACTDIR	1104
#define STID_DLG_P_WRITESIZE	1105
#define STID_DLG_P_CANCEL		1106

#define STID_DLG_T_LOCALSETTING	1111
#define STID_DLG_P_HIGH_MATCH	1112
#define STID_DLG_P_MAKE_DIR		1113
#define STID_DLG_P_CHK_SPACE	1114
#define STID_DLG_P_TOTAL		1115
#define STID_DLG_P_MINI_DLG		1116
#define STID_DLG_P_FLUSH		1117
#define STID_DLG_P_OLD_LOG		1118
#define STID_DLG_P_OLD_GF		1119
#define STID_DLG_P_OK			1120

#define STID_DLG_T_OWDIALOG		1131
#define STID_DLG_P_OWGROUP		1132
#define STID_DLG_P_OWOVWRITE	1133
#define STID_DLG_P_OWNOTOW		1134
#define STID_DLG_P_OWALLOW		1135
#define STID_DLG_P_OWALLNOT		1136
#define STID_DLG_P_BREAK		1137

#define STID_DLG_T_CHGFNAME		1141
#define STID_DLG_P_FNAMETITLE	1142

#define STID_DLG_T_CDDIALOG		1151
#define STID_DLG_P_CDCREATE		1152
#define STID_DLG_P_CDNOTCREATE	1153
#define STID_DLG_P_CDALLCREATE	1154
#define STID_DLG_P_CDALLNOT		1155

#define STID_DLG_T_CMTDIALOG	1161

#define STID_DLG_T_WINSFXSET	1171
#define STID_DLG_P_WSWINTITLE	1172
#define STID_DLG_P_WSDIRTITLE	1173
#define STID_DLG_P_WSINITDIR	1174
#define STID_DLG_P_WSCOMMAND	1175
#define STID_DLG_P_WSSETATTR	1176
#define STID_DLG_P_WSSETPATH	1177
#define STID_DLG_P_WSIGNORETIME	1178
#define STID_DLG_P_WSAUTOOW		1179
#define STID_DLG_P_WSAUTORUN	1180
#define STID_DLG_P_WSTAMPERCHK	1181

#define STID_DLG_T_SETTING		1191
#define STID_DLG_P_SUBASEDIR	1192
#define STID_DLG_P_SUFORCE_E	1193
#define STID_DLG_P_SUSEL_DIR	1194
#define STID_DLG_P_SUABS_DIR	1195
#define STID_DLG_P_SUREL_DIR	1196
#define STID_DLG_P_SUSEL_OW		1197
#define STID_DLG_P_SUALW_OW		1198
#define STID_DLG_P_SUQUE_OW		1199
#define STID_DLG_P_SUNOT_OW		1200
#define STID_DLG_P_SUATR_MODE	1201
#define STID_DLG_P_SULOCAL_SET	1202
#define STID_DLG_P_SUSAVE_ENV	1203
#define STID_DLG_P_SUSEL_PATH	1204
#define STID_DLG_P_SUDNY_NO		1205
#define STID_DLG_P_SUDNY_PAR	1206
#define STID_DLG_P_SUDNY_ABS	1207

#define STID_DLG_T_BPDIALOG		1211
#define STID_DLG_P_BPCHGNAME	1212
#define STID_DLG_P_BPNOT		1213
#define STID_DLG_P_BPALLCHG		1214

#define STID_DLG_T_APDIALOG		1221
#define STID_DLG_P_APRELATIVE	1222
#define STID_DLG_P_APABS		1223
#define STID_DLG_P_APINVALID	1224
#define STID_DLG_T_TLDIALOG		1225

#define STID_MES_T_WARNING		2001
#define STID_MES_T_ERROR		2002

#define STID_MES_T_PROC			2011
#define STID_MES_T_COMPRESS		2012
#define STID_MES_P_PROCFILE		2013
#define STID_MES_P_ADDFILE		2014
#define STID_MES_P_WORKFILE		2015

#define STID_MES_P_FILECOPY		2021

#define STID_MES_T_DELETE		2101
#define STID_MES_P_DELETE		2102
#define STID_MES_P_OVERWRITE	2104
#define STID_MES_T_ROWRITE		2105
#define STID_MES_P_ROWRITE		2106
#define STID_MES_P_CRCCHECK		2107
#define STID_MES_T_CRCERR		2108
#define STID_MES_P_CRCERR		2109
#define STID_MES_T_CRCERR2		2110
#define STID_MES_P_CRCERR2		2111
#define STID_MES_T_BADHUF		2112
#define STID_MES_P_BADHUF		2113
#define STID_MES_T_NAMECHANGE	2114
#define STID_MES_P_NAMECHANGE	2115
#define STID_MES_P_SHARE		2116
#define STID_MES_T_MAKEDIR		2117
#define STID_MES_P_MAKEDIR		2118
#define STID_MES_T_CONTINUE		2119
#define STID_MES_P_CONTINUE		2120
#define STID_MES_T_BADARC		2121
#define STID_MES_P_BADARC		2122
#define STID_MES_T_CONFIRM		2123
#define STID_MES_P_DICSIZ_WAR	2124
#define STID_MES_P_DISKSPACE	2125
#define STID_MES_T_SFXSET32		2126
#define STID_LOG_WAR_EXTDATA	2127
#define STID_LOG_WAR_SFX		2128
#define STID_MES_P_BADPATH		2129
#define STID_MES_P_ABSPATH		2130
#define STID_MES_P_TOOBIG		2131
#define STID_MES_P_EXECUTABLE	2132

#define STID_M_CANTOPEN			3001
#define STID_M_MAKEDIR			3002
#define STID_M_DIFFHOST			3003
#define STID_M_FEXISTS			3004
#define STID_M_FNEW				3005
#define STID_M_FOLD				3006
#define STID_M_FSAME			3007
#define STID_M_FNOEXIST			3008
#define STID_M_NOTSPCE			3009
#define STID_M_RDONLY			3010
#define STID_M_SPATTR			3011
#define STID_M_USKIP			3012
#define STID_M_WRITEER			3013
#define STID_M_DANNAME			3014
#define STID_M_SHARE			3015

#define STID_M_TOOLONG			3101
#define STID_M_TOOBIG			3102
#define STID_M_TOOLARGE			3102
#define STID_M_UNKNMETH			3103
#define STID_M_BADPATH			3104
#define STID_M_TOO_BIG2			3105
#define STID_M_EXEFILE			3106

#define STID_M_ERRORCNT			3201
#define STID_M_NOSPCL			3202

#define STID_ERR_8005			9001
#define STID_ERR_8006			9002
#define STID_ERR_8007			9003
#define STID_ERR_8008			9004
#define STID_ERR_8009			9005
#define STID_ERR_800A			9006
#define STID_ERR_800B			9007
#define STID_ERR_800C			9008
#define STID_ERR_800D			9009
#define STID_ERR_800E			9010
#define STID_ERR_800F			9011
#define STID_ERR_8010			9012
#define STID_ERR_8011			9013

#define STID_ERR_8012			9014
#define STID_ERR_8013			9015
#define STID_ERR_8014			9016
#define STID_ERR_8015			9017
#define STID_ERR_8016			9018
#define STID_ERR_8017			9019
#define STID_ERR_8018			9020
#define STID_ERR_8019			9021
#define STID_ERR_801A			9022
#define STID_ERR_801B			9023
#define STID_ERR_801C			9024
#define STID_ERR_801D			9025
#define STID_ERR_801E			9026
#define STID_ERR_801F			9027
#define STID_ERR_8020			9028
#define STID_ERR_8021			9029
#define STID_ERR_8022			9030
#define STID_ERR_8023			9031
#define STID_ERR_8024			9032
#define STID_ERR_8025			9033
#define STID_ERR_8026			9034
#define STID_ERR_8027			9035
#define STID_ERR_8028			9036
#define STID_ERR_8029			9037
#define STID_ERR_802A			9038
#define STID_ERR_802B			9039
#define STID_ERR_802C			9040
#define STID_ERR_802D			9041
#define STID_ERR_802E			9042
#define STID_ERR_802F			9043
#define STID_ERR_8030			9044
#define STID_ERR_8031			9045
#define STID_ERR_8032			9046
#define STID_ERR_8033			9047
#define STID_ERR_8034			9048
#define STID_ERR_8035			9049
#define STID_ERR_8036			9050
#define STID_ERR_8037			9051
#define STID_ERR_8038			9052
#define STID_ERR_8039			9053
#define STID_ERR_803A			9054
#define STID_ERR_803B			9055
#define STID_ERR_803C			9056
#define STID_ERR_803D			9057
#define STID_ERR_803E			9058
#define STID_ERR_803F			9059
#define STID_ERR_8040			9060
#define STID_ERR_8041			9061
#define STID_ERR_8042			9062
#define STID_ERR_8043			9063
#define STID_ERR_8044			9064
#define STID_ERR_8045			9065
#define STID_ERR_8046			9066
#define STID_ERR_8047			9067
#define STID_ERR_8048			9068
#define STID_ERR_8049			9069
#define STID_ERR_804A			9070
#define STID_ERR_804B			9071

#endif
