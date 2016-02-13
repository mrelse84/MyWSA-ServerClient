#pragma once

#include "MyWSA.h"

#define MAX_CLIENT_COUNT	100
#define MAX_MSG_SIZE		2048
#define MAX_MSG_COUNT		200
#define SOCK_MSG			20000

// Definitions of Socket Errors
#define ERR_SOCKET			-9001
#define ERR_BIND			-9002
#define ERR_LISTEN			-9003
#define ERR_CONNECT			-9004
#define ERR_WSAASYNCSELECT	-9100

#define MODE_NONE -1
#define MODE_SERVER	0
#define MODE_CLIENT 1

// CMyWSAWnd

class CMyWSAWnd : public CWnd
{
	DECLARE_DYNAMIC(CMyWSAWnd)

public:
	CMyWSAWnd();
	virtual ~CMyWSAWnd();

protected:
	DECLARE_MESSAGE_MAP()
private:
	SOCKET m_hServerSock;
	SOCKET m_hDataSock[MAX_CLIENT_COUNT]; // Server가 각 Client와 연결되는 Socket임. Client Sock과 구분하기 위해 Data Sock으로 명명함.
	SOCKET m_hClientSock;
	CString m_strServerIP;
	int m_nServerPort;
	int m_nMode; // -1: None, 0: Server, 1: Client
	char m_chClientIP[MAX_CLIENT_COUNT][24];
	int m_nClientPort[MAX_CLIENT_COUNT];
	int m_nClientInx;
	int m_nRecvSize[MAX_MSG_COUNT]; // 각 MSG의 Size를 저장
	int m_nRecvPos; // m_chRecv 버퍼의서 recv할 현재 위치
	int m_nRecvInx; // MSG의 현재 개수
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg LRESULT OnSockMsg(WPARAM wParam, LPARAM lParam);
public:
	//CObject* m_pOwner;
	CMyWSA* m_pOwner;
	char m_chRecv[MAX_MSG_SIZE*MAX_MSG_COUNT];
	//int SetOwner(CObject* pOwner);
	int SetOwner(CMyWSA* pOwner);
	int OpenSock(int nMode, int nServerPort = 7979, CString strServerIP = "127.0.0.1", CString strClientIP = "127.0.0.1");
	int CloseSock(SOCKET hSock);
	int CloseSock(void);
	int SendMsg(CString strMsg);
};


