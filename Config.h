//
// Config.h
// Configuration of Archiver DLL.
//
// $Id: Config.h,v 1.3 2005/09/18 05:46:11 halpussy Exp $
//

#ifndef CONFIG_H
#define CONFIG_H

// API_PREFIX: ApiのPrefixを指定します。
// API_NAME: Api名になります。
#define API_PREFIX	UnPma
#define API_NAME(api_suffix)  UnPma##api_suffix

// BUILD:
// GetBuildの返し値になります。
#define BUILD	1

// RUNMODE:
// Command()で同時に複数の書庫を扱えるならFALSE。扱えないならTRUE。
#define RUNMODE	FALSE

// STRING_REGKEY:
// モジュールに格納されていない言語リソースを取得するレジストリキー。
#define STRING_REGKEY "Software\\ArchiverDll\\UnPma32\\String"

// SETTING_REGKEY:
// 設定を格納するレジストリキー。(未使用)
#define SETTING_REGKEY "Software\\ArchiverDll\\UnPma32"

// EXTRACT_CMD:
// 解凍時のコマンド。一個目の%sが書庫名、二個目が解凍先、三個目が該当ファイル名。
#define EXTRACT_CMD "x -- \"%s\" \"%s\" \"%s\""

// 以下、DLL固有の値を記述してください。
#define SFX_WIN32 2001

#define DLL_NAME "UnPma32.dll"

#endif
