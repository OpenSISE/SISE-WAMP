#pragma once
class CMyIni
{
public:
	CMyIni();
	~CMyIni();
	char* ReadIni();
	CString GetFilePath();
	void CreateDefaulltIni(CString FilePath);
	CString ReadVersionIni();
	//void CreateVersionIni();
};

