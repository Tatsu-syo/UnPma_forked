//
// Mxd.cpp
// Meltice eXtension Dll
//
// $Id: Mxd.cpp,v 1.1 2005/06/02 15:32:26 halpussy Exp $
//

#include "Protocol.h"
#include <string.h>

#define MXD_PROTOCOL_VERSION  2000  // 2.00.0

bool unpack_archive(const char *file_name, const char *output_dir)
{
  const size_t  BUF_SIZE = 1024;
  char  cmd_line[BUF_SIZE];

  // (手抜き)
  if (NULL == output_dir)
    wsprintf(cmd_line, "x -- \"%s\"", file_name);
  else
    wsprintf(cmd_line, "x -- \"%s\" \"%s\"",file_name,output_dir);

  int  ret = UnPma(NULL, cmd_line, NULL, NULL);

  return (ret) ? false : true;
}

bool pack_files(const char *filenames, const char *base_dir)
{
  return false;
}

// ---

int WINAPI MxdGetVersion(void)
{
  return UnPmaGetVersion()*10+UnPmaGetSubVersion()/100;
}

int WINAPI MxdGetProtocolVersion(void)
{
  return MXD_PROTOCOL_VERSION;
}

bool WINAPI MxdConfigName(char *buf, const int mode, const int lang)
{

  switch (lang)
  {
  case 932:  // 日本語
  case 1:    // 日本語以外？
    {
      wsprintf(buf, "UnPma32*MXD");
    }
    break;

  default:
    return false;
  }

  return true;
}

bool WINAPI MxdConfigDialog(const HWND wh, LPSTR, const int mode)
{
  enum
  {
    MXD_CFG_VERSION = 0,
    MXD_CFG_UNPACK,
    MXD_CFG_PACK
  };

  switch (mode)
  {
  case MXD_CFG_VERSION:
    {
      const size_t  BUF_SIZE = 256;

      char  text[BUF_SIZE];
      wsprintf(text, "%s ver. %d.%d",
               "UnPma32*MXD", UnPmaGetVersion() / 100, UnPmaGetVersion() % 100);
      MessageBox(wh, text, "UnPma32.mxd", MB_OK);
    }
    break;

  case MXD_CFG_UNPACK:
    return UnPmaConfigDialog(wh, NULL, 1) ? true : false;

  case MXD_CFG_PACK:
    return UnPmaConfigDialog(wh, NULL, 2) ? true : false;

  default:
    return false;
  }

  return true;
}

bool WINAPI MxdCheckArchive(const char *file_name)
{
	return UnPmaCheckArchive(file_name, 1) ? true : false;
}

bool WINAPI MxdUnpackArchive(const char *file_name, const char *output_dir)
{
  return unpack_archive(file_name, output_dir);
}

bool WINAPI MxdPackArchive(const char *filenames, const char *base_dir)
{
  return pack_files(filenames, base_dir);
}
