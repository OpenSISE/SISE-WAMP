#include "stdafx.h"
#include "MyIni.h"


CMyIni::CMyIni()
{
}


CMyIni::~CMyIni()
{
}

 char* CMyIni::ReadIni()
{
	CString FilePath = this->GetFilePath();
	FilePath += "SISE_Config.ini";
	char* data = new char[64];
	memset(data, 0, 64);
	char iniResult[MAX_PATH];
	DWORD lenResult, iniSize = 256;
	lenResult = GetPrivateProfileSection("SISE_Config", iniResult, iniSize, FilePath);
	if (lenResult > 0)
	{
		char *pdest;
		char key[32];
		int strEnd = 0x00;
		pdest = strchr(iniResult, '=');
		memset(key,strEnd,32);
		memcpy(key, iniResult, pdest - iniResult);		
		memcpy(data, pdest + 1,63);
	}
	else{
		this->CreateDefaulltIni(FilePath);
	}
	
	return data;
}

void CMyIni::CreateDefaulltIni(CString FilePath)
{
	WritePrivateProfileSection("说明", "若不更改Apache和Mysql的目录，则这个文件无需改动.....\r\n\r\n", FilePath);
	WritePrivateProfileSection("SISE_Config", "程序路径=D:\\SISE-WAMP1.0\r\n", FilePath);
	WritePrivateProfileSection("Version", "ApacheVersion=Apache24\r\nMysqlVersion=Mysql5.1", FilePath);
}

CString CMyIni::GetFilePath()
{
	CString FilePath;
	char Temp[64] = { 0 };
	::GetModuleFileName(NULL, Temp, sizeof(Temp));
	(strrchr(Temp,'\\'))[1] = 0;                     //获取当前文件路径
	FilePath = Temp;
	FilePath.Replace("\\", "\\\\");
	
	return FilePath;
}

CString CMyIni::ReadVersionIni()
{
	CString FILEText = GetFilePath();
	FILEText += "SISE_Config.ini";
	CString  Contents,Result;
	char sResult[MAX_PATH];
	DWORD dwResult, iSize = 256;
	dwResult = GetPrivateProfileSection("Version", sResult, iSize, FILEText);
	if (dwResult > 0)
	{
		char item[256];
		int  len = 0;
		int strEnd = 0x00;
		memset(item, 0x00, sizeof(item));
		for (int i = 0; i<dwResult; i++)
		{
			if (sResult[i] != strEnd)
			{
				item[len] = sResult[i];
				len++;
			}
			else
			{
				char* pdest;
				char key[256], data[256];
				pdest = strchr(item, '=');
				memset(key, 0x00, sizeof(key));
				memcpy(key, item, pdest - item);
				memset(data, 0x00, sizeof(data));
				strcpy(data, pdest + 1);

				Contents.Format("%s", data);
				if (strcmp(key, "ApacheVersion") == 0){
					//MessageBox(NULL,Contents,"apache",0);
					Result += "Apache="+Contents+"\r\n";
				}
				else if (strcmp(key, "MysqlVersion") == 0){
					Result += "Mysql="+Contents + "\r\n";
				}
				memset(item, 0x00, sizeof(item));
				len = 0;
				if (sResult[i + 1] == strEnd)
				{
					break;
				}
			}
		}
	}
	return Result;
}