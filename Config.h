//
// Config.h
// Configuration of Archiver DLL.
//
// $Id: Config.h,v 1.3 2005/09/18 05:46:11 halpussy Exp $
//

#ifndef CONFIG_H
#define CONFIG_H

// API_PREFIX: Api��Prefix���w�肵�܂��B
// API_NAME: Api���ɂȂ�܂��B
#define API_PREFIX	UnPma
#define API_NAME(api_suffix)  UnPma##api_suffix

// BUILD:
// GetBuild�̕Ԃ��l�ɂȂ�܂��B
#define BUILD	1

// RUNMODE:
// Command()�œ����ɕ����̏��ɂ�������Ȃ�FALSE�B�����Ȃ��Ȃ�TRUE�B
#define RUNMODE	FALSE

// STRING_REGKEY:
// ���W���[���Ɋi�[����Ă��Ȃ����ꃊ�\�[�X���擾���郌�W�X�g���L�[�B
#define STRING_REGKEY "Software\\ArchiverDll\\UnPma32\\String"

// SETTING_REGKEY:
// �ݒ���i�[���郌�W�X�g���L�[�B(���g�p)
#define SETTING_REGKEY "Software\\ArchiverDll\\UnPma32"

// EXTRACT_CMD:
// �𓀎��̃R�}���h�B��ڂ�%s�����ɖ��A��ڂ��𓀐�A�O�ڂ��Y���t�@�C�����B
#define EXTRACT_CMD "x -- \"%s\" \"%s\" \"%s\""

// �ȉ��ADLL�ŗL�̒l���L�q���Ă��������B
#define SFX_WIN32 2001

#define DLL_NAME "UnPma32.dll"

#endif
