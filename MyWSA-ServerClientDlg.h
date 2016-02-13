
// MyWSA-ServerClientDlg.h : 헤더 파일
//

#pragma once
#include "MyWSA.h"
#include "MyWSAWnd.h"

// CMyWSAServerClientDlg 대화 상자
class CMyWSAServerClientDlg : public CDialogEx, public CMyWSA
{
// 생성입니다.
public:
	CMyWSAServerClientDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
	enum { IDD = IDD_MYWSASERVERCLIENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
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
