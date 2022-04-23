//
// MemmFile.cpp
// �������[�}�b�v�h�t�@�C��
//
// $Id: MemmFile.cpp,v 1.1 2005/09/18 05:49:28 halpussy Exp $
//

#include "MemmFile.h"

bool MemmapFile::Open( const char* fname ){
	Close();

	// �t�@�C����ǂ݂Ƃ��p�ŊJ��
	hFile = ::CreateFile(fname,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL|FILE_FLAG_SEQUENTIAL_SCAN,NULL);
	if(hFile==INVALID_HANDLE_VALUE) return false;

	// �T�C�Y���擾
	dwSize = ::GetFileSize(hFile,NULL);

	if(!dwSize){
		// 0�o�C�g�̃t�@�C���̓}�b�s���O�o���Ȃ��̂œK���ɉ��
		// (����ɂ��A�K��base���L���ɂȂ�)
		Ptr = &dwSize;
	}else{
		// �}�b�s���O�I�u�W�F�N�g�����
		hMap = ::CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, 0, NULL);
		if(!hMap){
			::CloseHandle(hFile);
			hFile = INVALID_HANDLE_VALUE;
			return false;
		}
		// �r���[
		Ptr = ::MapViewOfFileEx(hMap, FILE_MAP_READ, 0, 0, 0 ,(VOID*)0x00000000);
		if(!Ptr){
			::CloseHandle(hMap);
			::CloseHandle(hFile);
			hFile = INVALID_HANDLE_VALUE;
			return false;
		}
	}
	return true;
}	

void MemmapFile::Close(){
	if(hFile!=INVALID_HANDLE_VALUE){
		// �w���e�R�}�b�s���O�����ĂȂ���΂����ŉ��
		if(Ptr!=&dwSize) ::UnmapViewOfFile(Ptr);
		if(hMap) ::CloseHandle(hMap);
		::CloseHandle(hFile);
		Init();
	}
}
