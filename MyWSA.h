#pragma once
class CMyWSA
{
public:
	CMyWSA(void);
	~CMyWSA(void);
	virtual int MyAccept(long lRet) = 0;
	virtual int MyReceive(CString strMsg) = 0;
	virtual int MyConnect(long lRet) = 0;
	virtual int MyClose(long lRet) = 0;
};

