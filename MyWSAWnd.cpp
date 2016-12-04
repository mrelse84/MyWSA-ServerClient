// MyWSAWnd.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MyWSAWnd.h"

//#include "MyWSA-ServerClient.h"
//#include "MyWSA-ServerClientDlg.h"

#include <winsock2.h>	// winsock 2
//#include <afxsock.h>	// winsock 1(?)

// CMyWSAWnd

IMPLEMENT_DYNAMIC(CMyWSAWnd, CWnd)

CMyWSAWnd::CMyWSAWnd()
	: m_pOwner(NULL)
{
	// winsock 초기화
	WSADATA data;
	WSAStartup(0x0202, &data);

	//if (!AfxSocketInit())			//소켓초기화
	//{ 
	//	AfxMessageBox("AfxSocketInit Fail");
	//}
	
	// WSAAsyncEvent(winsock의 비동기 Event)와 연결할 Hidden Window 객체 생성
	CString wcn = ::AfxRegisterWndClass(NULL);
	BOOL created = this->CreateEx(0, wcn, _T("Hidden Win"), 0, 0, 0, 0, 0, 0/*HWND_MESSAGE*/, 0);

	// 변수 초기화
	m_nRecvPos = 0; // 버퍼내 읽어 들일 위치 초기화
	m_nRecvInx = 0; // 버퍼내 MSG 개수 초기화
	m_nClientInx = 0;
	m_nMode = MODE_NONE;
	m_strServerIP = "127.0.0.1";
	m_nServerPort = 7979;
	m_hServerSock = NULL;
	m_hClientSock = NULL;
	for (int i=0; i<MAX_CLIENT_COUNT; i++)
		m_hDataSock[i] = NULL;
}

CMyWSAWnd::~CMyWSAWnd()
{
	WSACleanup();
}


BEGIN_MESSAGE_MAP(CMyWSAWnd, CWnd)
	ON_WM_TIMER()
	ON_MESSAGE(SOCK_MSG, OnSockMsg)
END_MESSAGE_MAP()



// CMyWSAWnd 메시지 처리기입니다.

//int CMyWSAWnd::SetOwner(CObject* pOwner)
int CMyWSAWnd::SetOwner(CMyWSA* pOwner)
{
	m_pOwner = pOwner;
	return 0;
}

int CMyWSAWnd::OpenSock(int nMode, int nServerPort, CString strServerIP, CString strClientIP)
{
	int nReturn  = 0; // OK

	m_nMode = nMode;
	m_strServerIP = strServerIP;
	m_nServerPort = nServerPort;

	struct sockaddr_in srv_addr;
	memset(&srv_addr, 0, sizeof(struct sockaddr_in));
	srv_addr.sin_family = AF_INET;
	srv_addr.sin_addr.s_addr = inet_addr(strServerIP);
	srv_addr.sin_port = htons(nServerPort);

	switch (nMode)
	{
	case 0: // Server
		// server socket 생성
		m_hServerSock = socket(AF_INET, SOCK_STREAM, 0);

		if (m_hServerSock!= NULL)
		{
			int nBind = bind(m_hServerSock, (LPSOCKADDR)&srv_addr, sizeof(struct sockaddr_in));
			if (nBind != SOCKET_ERROR)
			{
				int nListen = listen(m_hServerSock, 1);
				if (nListen != SOCKET_ERROR)
				{
					int nSelect = WSAAsyncSelect(m_hServerSock, m_hWnd, SOCK_MSG, FD_ACCEPT);
					if (nSelect != SOCKET_ERROR)
					{
						nReturn = 0;
					}
					else
					{
						nReturn = ERR_WSAASYNCSELECT; // WSAAsyncSelect fail
					}
				}
				else
				{
					nReturn = ERR_LISTEN; // ERR: listen fail
				}
			}
			else
			{
				nReturn = ERR_BIND; // ERR: bind fail
			}
		}
		else
		{
			nReturn = ERR_SOCKET; // ERR: socket fail
		}
		break;
	case 1: // Client
		// client socket 생성
		m_hClientSock = socket(AF_INET, SOCK_STREAM, 0);

		if (m_hClientSock!= NULL)
		{
			int nSelect = WSAAsyncSelect(m_hClientSock, m_hWnd, SOCK_MSG, FD_CONNECT | FD_CLOSE);
			if (nSelect != SOCKET_ERROR)
			{
				int nConnect = connect(m_hClientSock, (LPSOCKADDR)&srv_addr, sizeof(struct sockaddr_in));
				int nWSAError = WSAGetLastError();
				if (!nConnect || nWSAError == WSAEWOULDBLOCK)
				{
					nReturn = 0;
				}
				else
				{
					nReturn = ERR_CONNECT; // ERR: connect fail
				}
			}
			else
			{
				nReturn = ERR_WSAASYNCSELECT; // WSAAsyncSelect fail
			}
		}
		else
		{
			nReturn = ERR_SOCKET; // ERR: socket fail
		}

		if (nReturn != 0)
			CloseSock(m_hClientSock);
		break;
	}


	return nReturn;
}

int CMyWSAWnd::CloseSock(SOCKET hSock)
{
	if (hSock != NULL && hSock != INVALID_SOCKET)
	{
		WSAAsyncSelect(hSock, m_hWnd, SOCK_MSG, 0); // 비동기 메시지 설정 해제

		LINGER l_linger = {TRUE, 0};

		setsockopt(hSock, SOL_SOCKET, SO_LINGER, (char FAR *)&l_linger, sizeof(l_linger));

		closesocket(hSock);
	}

	return 0;
}

int CMyWSAWnd::CloseSock(void)
{
	switch (m_nMode)
	{
	case 0: // Server
		for (int i=0; i<m_nClientInx; i++)
		{
			if (m_hDataSock[i] != NULL)
			{
				CloseSock(m_hDataSock[i]);
			}
		}

		CloseSock(m_hServerSock);

		m_hServerSock = NULL;

		break;
	case 1: // Client
		CloseSock(m_hClientSock);

		m_hClientSock = NULL;

		break;
	}

	return 0;
}



void CMyWSAWnd::OnTimer(UINT_PTR nIDEvent)
{
	// Timer는 다음에 기능 추가 시 사용 .. 지금은 껍데기만 ..

	CWnd::OnTimer(nIDEvent);
}

LRESULT  CMyWSAWnd::OnSockMsg(WPARAM wParam, LPARAM lParam)
{
	long lRet = S_OK;

	SOCKET hCurSock = (SOCKET)wParam;

	switch (WSAGETSELECTEVENT(lParam))
	{
	case FD_CONNECT:
		if (WSAGETSELECTERROR(lParam) == 0)
		{
			WSAAsyncSelect(hCurSock, m_hWnd, SOCK_MSG, FD_READ | FD_CLOSE);
		}
		else
		{
			lRet = -1;
		}
		//((CMyWSAServerClientDlg*)m_pOwner)->MyConnect(lRet);
		((CMyWSA*)m_pOwner)->MyConnect(lRet);
		break;
	case FD_ACCEPT:
		if (WSAGETSELECTERROR(lParam) == 0)
		{
			if (m_nClientInx < MAX_CLIENT_COUNT)
			{
				struct sockaddr_in client_addr;
				int client_addr_size = sizeof(struct sockaddr_in);
				m_hDataSock[m_nClientInx] = accept(hCurSock, (LPSOCKADDR)&client_addr, &client_addr_size);

				// 비동기 이벤트 설정
				WSAAsyncSelect(m_hDataSock[m_nClientInx], m_hWnd, SOCK_MSG, FD_READ | FD_CLOSE);

				// 접속한 각 Client의 IP 주소와 Port 번호를 저장
				char temp_ip_addr[24];
				strcpy_s(temp_ip_addr, sizeof(temp_ip_addr), inet_ntoa(client_addr.sin_addr));
				strcpy_s(m_chClientIP[m_nClientInx], sizeof(m_chClientIP[m_nClientInx]), temp_ip_addr);
				m_nClientPort[m_nClientInx] = client_addr.sin_port;
				m_nClientInx++;
			}
			else
			{
				lRet = -1; // 최대 접속 Client 개수 초과로 접속 불가
			}
			//((CMyWSAServerClientDlg*)m_pOwner)->MyAccept(lRet);
			((CMyWSA*)m_pOwner)->MyAccept(lRet);
		}
		break;
	case FD_READ:
		{
			// 임시 변수
			CString strTempMsg = "";

			WSAAsyncSelect(hCurSock, m_hWnd, SOCK_MSG, FD_CLOSE);

			// 데이터 수신
			int nRecvSize = 0;

			while (true)
			{
				nRecvSize = recv(hCurSock, &m_chRecv[m_nRecvPos], MAX_MSG_SIZE, 0);

				if (nRecvSize <= 0)
				{
					break; // exit while loop
				}

				m_chRecv[m_nRecvPos+nRecvSize] = '\0';
				strTempMsg.Format("%s", &m_chRecv[m_nRecvPos]);

				nRecvSize = nRecvSize + 1; // '\0' 추가에 따라 1 증가시킴
				m_nRecvPos += nRecvSize;
				m_nRecvSize[m_nRecvInx] = nRecvSize; // Recv 회수마다 RecvSize 저장
				m_nRecvInx++;

				if (m_nRecvInx >= MAX_MSG_COUNT)
				{
					m_nRecvPos = 0;
					m_nRecvInx = 0;
				}
			}

			WSAAsyncSelect(hCurSock, m_hWnd, SOCK_MSG, FD_READ | FD_CLOSE);

			//((CMyWSAServerClientDlg*)m_pOwner)->MyReceive(strTempMsg);
			((CMyWSA*)m_pOwner)->MyReceive(strTempMsg);
		}
		break;
	case FD_CLOSE: // Peer Closed!
		CloseSock(hCurSock);

		if (m_nMode == MODE_SERVER)
		{
			for (int i=0; i<m_nClientInx; i++)
			{
				if (m_hDataSock[i] == hCurSock)
				{
					if (i != m_nClientInx-1) // if NOT Last, replace it(=CurSock) with Last
					{
						m_hDataSock[i] = m_hDataSock[m_nClientInx-1];
						strcpy_s(m_chClientIP[i], sizeof(m_chClientIP[i]), m_chClientIP[m_nClientInx-1]);
						m_nClientPort[i] = m_nClientPort[m_nClientInx-1];
					}
					m_hDataSock[m_nClientInx-1] = NULL;
					m_nClientInx--;
					break;
				}
			}
		}
		else if (m_nMode == MODE_CLIENT)
		{
			m_hClientSock = NULL;
		}

		//((CMyWSAServerClientDlg*)m_pOwner)->MyClose(lRet);
		((CMyWSA*)m_pOwner)->MyClose(lRet);

		break;
	}

	return lRet;
}

int CMyWSAWnd::SendMsg(CString strMsg)
{
	int nSendSize = -1;

	int nLen = strMsg.GetLength();
	char* pData = new char[nLen+2];
	memcpy(pData, strMsg, nLen);
	pData[nLen] = '\r';
	pData[nLen+1] = '\n';

	if (m_nMode == MODE_SERVER)
	{
		for (int i=0; i<m_nClientInx; i++)
		{
			if (m_hDataSock[i] != NULL)
			{
				nSendSize = send(m_hDataSock[i], pData, nLen+2, 0);
			}
		}
	}
	else if (m_nMode == MODE_CLIENT)
	{
		if (m_hClientSock != NULL)
		{
			nSendSize = send(m_hClientSock, pData, nLen+2, 0);
		}
	}


	delete[] pData;

	return nSendSize;
}