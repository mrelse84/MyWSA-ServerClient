
// MyWSA-ServerClient.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CMyWSAServerClientApp:
// �� Ŭ������ ������ ���ؼ��� MyWSA-ServerClient.cpp�� �����Ͻʽÿ�.
//

class CMyWSAServerClientApp : public CWinApp
{
public:
	CMyWSAServerClientApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CMyWSAServerClientApp theApp;