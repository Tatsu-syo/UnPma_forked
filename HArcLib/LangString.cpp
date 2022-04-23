//
// LangString.cpp
// �X�g�����O�e�[�u���ǂݍ���
//
// $Id: LangString.cpp,v 1.2 2005/10/30 04:19:46 halpussy Exp $
//

#include "Classes.h"

LPSTR LoadLangString(const HMODULE hDll, const WORD wLangID, const WORD wStringID, LPSTR &lpszBuf){
	//���W�X�g�������݂����A�����W���[���̃X�g�����O�e�[�u�������Ă���\��������̂�
	//NULL�̃`�F�b�N���s���ׂ���������܂���c

	//�e�N���X�́A���̃N���X���g��LoadLangString���g�p����ׂ��ł��B

	FreeAndNil(lpszBuf);

	{//�܂��̓��W�X�g��������
		char	szStringID[5];
		char	szRegID[260];
		DWORD	dwType=REG_SZ;
		HKEY	hKey=NULL;
		ULONG	nSize;

		//������ɕϊ�
		sprintf(szStringID, "%u", wStringID);
		sprintf(szRegID, "%s\\%04X", STRING_REGKEY, wLangID);

		//���W�X�g���L�[�擾(1)
		if(!RegOpenKeyEx(HKEY_CURRENT_USER,szRegID,0,KEY_READ,&hKey)){ //�����Ƃ͎v���܂����������݂ł��Ȃ������l��
			//�l���擾
			RegQueryValueEx(hKey, szStringID, NULL, &dwType, NULL, &nSize);
			if( !(lpszBuf = (char*)malloc(nSize+1)) ){
				RegCloseKey(hKey);
				return NULL;
			}
			RegQueryValueEx(hKey, szStringID, NULL, &dwType, (LPBYTE)lpszBuf, &nSize);
			RegCloseKey(hKey);
			return lpszBuf;
		}

		//���W�X�g���L�[�擾(2)
		if(!RegOpenKeyEx(HKEY_LOCAL_MACHINE,szRegID,0,KEY_READ,&hKey)){ //�����Ƃ͎v���܂����������݂ł��Ȃ������l��
			//�l���擾
			RegQueryValueEx(hKey, szStringID, NULL, &dwType, NULL, &nSize);
			if( !(lpszBuf = (char*)malloc(nSize+1)) ){
				RegCloseKey(hKey);
				return NULL;
			}
			RegQueryValueEx(hKey, szStringID, NULL, &dwType, (LPBYTE)lpszBuf, &nSize);
			RegCloseKey(hKey);
			return lpszBuf;
		}
	}

	{//���Ń��W���[����������
	 //Thx. HyperBeat (�܂Ƃ��Ɏg����悤�ɂ��Ă��������܂���)
		int i;
		WORD len;
		HRSRC hRsrc;
		HGLOBAL hRes;
		LPWSTR szwRes;

		if( !(hRsrc = FindResourceEx(hDll,RT_STRING,MAKEINTRESOURCE((wStringID>>4)+1),wLangID))
			||!(hRes = LoadResource(hDll,hRsrc))
			||!(szwRes = (LPWSTR)LockResource(hRes))
		){
			if( !(hRsrc = FindResourceEx(hDll,RT_STRING,MAKEINTRESOURCE((wStringID>>4)+1),0x0409))
				||!(hRes = LoadResource(hDll,hRsrc))
				||!(szwRes = (LPWSTR)LockResource(hRes))
			){
				ErrorMsg(); //���Ȑf�f
				return NULL;
			}
		}

		for (i = 0,len = 0;i < (wStringID&15)+1;i++){
			szwRes += len;
			len = (WORD)(*szwRes & 0xFFFF);
			szwRes++;
		}

		lpszBuf = (LPSTR)malloc(len*2+1);
		NullMemory(lpszBuf,len*2+1);
		//*(szwRes+len) = L'\0'; //LockResource�Ŏ擾����������ɂ͌����J���Ă͂Ȃ�Ȃ��B
		WideCharToMultiByte(CP_ACP,0,szwRes,len,lpszBuf,len*2,NULL,NULL); //len�����̂ݕϊ�����B
		return lpszBuf;
	}

	return NULL;
}
