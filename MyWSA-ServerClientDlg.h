
// MyWSA-ServerClientDlg.h : ��� ����
//

#pragma once
#include "MyWSA.h"
#include "MyWSAWnd.h"

// CMyWSAServerClientDlg ��ȭ ����
class CMyWSAServerClientDlg : public CDialogEx, public CMyWSA
{
// �����Դϴ�.
public:
	CMyWSAServerClientDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_MYWSASERVERCLIENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	int m_nMode;
	CMyWSAWnd m_myWSASock;
	CString m_strSendData;
	virtual int MyAccept(long lRet);
	virtual int MyReceive(CString strMsg);
	virtual int MyConnect(long lRet);
	virtual int MyClose(long lRet);
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
};
