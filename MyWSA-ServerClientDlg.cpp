
// MyWSA-ServerClientDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "MyWSA-ServerClient.h"
#include "MyWSA-ServerClientDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

// �����Դϴ�.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMyWSAServerClientDlg ��ȭ ����




CMyWSAServerClientDlg::CMyWSAServerClientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMyWSAServerClientDlg::IDD, pParent)
	, m_nMode(0)
	, m_strSendData(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMyWSAServerClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO1, m_nMode);
	DDX_Text(pDX, IDC_EDIT1, m_strSendData);
}

BEGIN_MESSAGE_MAP(CMyWSAServerClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CMyWSAServerClientDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CMyWSAServerClientDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CMyWSAServerClientDlg::OnBnClickedButton3)
END_MESSAGE_MAP()


// CMyWSAServerClientDlg �޽��� ó����

BOOL CMyWSAServerClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �ý��� �޴��� "����..." �޴� �׸��� �߰��մϴ�.

	// IDM_ABOUTBOX�� �ý��� ��� ������ �־�� �մϴ�.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// �� ��ȭ ������ �������� �����մϴ�. ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	//m_myWSASock.SetOwner((CObject*)this);
	m_myWSASock.SetOwner((CMyWSA*)this);

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

void CMyWSAServerClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�. ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CMyWSAServerClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CMyWSAServerClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

int CMyWSAServerClientDlg::MyAccept(long lRet)
{
	if (lRet != 0)
	{
		MessageBox("Accept Fail");
		return -1;
	}
	else
	{
		CString strMsg = "[Accept Ok]";
		((CListBox*)GetDlgItem(IDC_LIST1))->AddString(strMsg);
	}

	return 0;
}


int CMyWSAServerClientDlg::MyReceive(CString strMsg)
{
	((CListBox*)GetDlgItem(IDC_LIST1))->AddString(strMsg);

	return 0;
}


int CMyWSAServerClientDlg::MyConnect(long lRet)
{
	if (lRet != 0)
	{
		MessageBox("Connect Fail");
		return -1;
	}
	else
	{
		CString strMsg = "[Connect Ok]";
		((CListBox*)GetDlgItem(IDC_LIST1))->AddString(strMsg);
	}

	return 0;
}

int CMyWSAServerClientDlg::MyClose(long lRet)
{
	if (lRet != 0)
	{
		MessageBox("Peer Close Fail");
		return -1;
	}
	else
	{
		CString strMsg = "[Peer Closed]";
		((CListBox*)GetDlgItem(IDC_LIST1))->AddString(strMsg);
	}

	return 0;
}

void CMyWSAServerClientDlg::OnBnClickedButton1()
{
	UpdateData(TRUE);	// Dialog�� ��Ʈ�� ���� �� ���� ����

	if (m_nMode == 0)	// Server
	{
		if (m_myWSASock.OpenSock(0,7979))			// Server Port : 7979
		{ 
			MessageBox("Server Sock Open Fail");
			return;
		}
	}
	else				// Client
	{
		if (m_myWSASock.OpenSock(1,7979,"127.0.0.1"))
		{ 
			MessageBox("Client Sock Open Fail");
			return;
		}
	}

	CString strMsg = "[Open Ok]";
	((CListBox*)GetDlgItem(IDC_LIST1))->AddString(strMsg);
}


void CMyWSAServerClientDlg::OnBnClickedButton2()
{
	UpdateData(TRUE);

	int nSent = m_myWSASock.SendMsg(m_strSendData);

	if (nSent == -1)
	{
		CString strMsg = "[Send Fail]";
		((CListBox*)GetDlgItem(IDC_LIST1))->AddString(strMsg);
	}
}


void CMyWSAServerClientDlg::OnBnClickedButton3()
{
	m_myWSASock.CloseSock();

	CString strMsg = "[Close Ok]";
	((CListBox*)GetDlgItem(IDC_LIST1))->AddString(strMsg);
}
