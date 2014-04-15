
// ChatClient1Dlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ChatClient1.h"
#include "ChatClient1Dlg.h"
#include "afxdialogex.h"
#include "Login.h"
#include "OLList.h"
#include "TalkDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	// ʵ��
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


// CChatClient1Dlg �Ի���



CChatClient1Dlg::CChatClient1Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CChatClient1Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CChatClient1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_tab);
}

BEGIN_MESSAGE_MAP(CChatClient1Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(UM_SOCK, &CChatClient1Dlg::OnUmSock)
	ON_WM_CLOSE()
	ON_MESSAGE(UM_FEED, &CChatClient1Dlg::OnUmFeed)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CChatClient1Dlg::OnTcnSelchangeTab1)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CChatClient1Dlg::OnCbnSelchangeCombo1)
END_MESSAGE_MAP()


// CChatClient1Dlg ��Ϣ�������

BOOL CChatClient1Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	//����ClientSocket�����msg���󲢽��ж���ĳ�ʼ��
	this->Init();
	//CLogin	LogDlg;
	//m_WndLogin = &LogDlg; //��֪�в���
	m_WndLogin->DoModal();

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CChatClient1Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CChatClient1Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CChatClient1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CChatClient1Dlg::Init()
{
	m_WndLogin	= new CLogin();
	m_Comm		= CClientSocket();
	m_Data		= CMsg();
	m_WndLogin->SetMainDlg(this);
	m_Comm.SetMainDlg(this);
	m_Data.SetMainDlg(this);
	m_Comm.InitSocket();
	m_Data.SetLoginDlg(m_WndLogin);



	//����Ĭ��IP
	
	DWORD   dwIP;  
	CString csIP = m_Comm.GetLocalIP();
	dwIP   =   inet_addr(csIP); 
	//m_WndLogin->SetIP(dwIP);

	ZeroMemory(&msg_recv, sizeof(msg_recv));

	((CTabCtrl*)GetDlgItem(IDC_TAB1))->InsertItem(0, "�����û�");
	((CTabCtrl*)GetDlgItem(IDC_TAB1))->InsertItem(1, "�����б�");
	((CTabCtrl*)GetDlgItem(IDC_TAB1))->InsertItem(2, "�����б�");

	((CComboBox*)GetDlgItem(IDC_COMBO1))->AddString("����");
	((CComboBox*)GetDlgItem(IDC_COMBO1))->AddString("�뿪");


	//�����б���

	CRect	rect;
	GetClientRect(&rect);	
	rect.left += 20;
	rect.top += 120;
	rect.right -= 20;
	rect.bottom -= 20;

	m_OLList = new COLList;
	m_OLList->SetMainDlg(this);
	if (!m_OLList->Create(WS_CHILD | WS_VISIBLE | LVS_REPORT, rect, this, IDC_LISTBOX))
	{
		TRACE("Failed to create view for CMyBarLeft\n");
	}
	m_OLList->SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	m_OLList->InsertColumn(0,"ID",LVCFMT_LEFT, 30);
	m_OLList->InsertColumn(1,"�����б���",LVCFMT_LEFT, 180);
	ResizeComboBox();

}

void CChatClient1Dlg::UnInit()
{
	m_Comm.Stop();
	m_Comm.UnInitSocket();
	delete m_WndLogin;
}

void CChatClient1Dlg::ResizeComboBox()
{
	//CComboBox*							pCom = ((CComboBox*)GetDlgItem(IDC_COMBO1));
	//CString								str;
	//CSize								sz;
	//int									dx=0; 
	//CDC*								pDC = pCom->GetDC();
	//for (int i=0;i < pCom->GetCount();i++)	
	//{        
	//	pCom->GetLBText( i, str );
	//	sz = pDC->GetTextExtent(str);       
	//	
	//	if (pCom->GetItemHeight(i) < sz.cy)           
	//		pCom->SetItemHeight(i, sz.cy);      
	//	
	//	if ( (dx = pCom->GetDroppedWidth()) < sz.cx)        
	//	{            
	//		pCom->SetDroppedWidth(sz.cx + 20);       
	//	}   
	//}   
	//pCom->ReleaseDC(pDC);
}


//�˺�����Ӧ����˴�������Ϣ
afx_msg LRESULT CChatClient1Dlg::OnUmSock(WPARAM wParam, LPARAM lParam)
{
	if(WSAGETSELECTERROR(lParam))
	{
		TRACE("������Ϣ�����쳣! �������: %d \n", WSAGETSELECTERROR(lParam));
		return 1;
	}
	switch(WSAGETSELECTEVENT(lParam))
	{
	case FD_READ:
		m_Comm.RecvMsg(msg_recv);
		m_Data.parseMsg(msg_recv);

		break;
	case FD_CONNECT:
		break;
	}
	return 0;
}


void CChatClient1Dlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	this->UnInit();
	CDialogEx::OnClose();
}

//���˺�����Ӧ�ͻ�������CMsg�ദ��õ���Ϣ, �������Ի��������������� û��...
afx_msg LRESULT CChatClient1Dlg::OnUmFeed(WPARAM wParam, LPARAM lParam)
{
	return 0;
}


void CChatClient1Dlg::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	int CurSel = m_tab.GetCurSel();
	m_Data.SwitchList(CurSel);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
}


void CChatClient1Dlg::OnCbnSelchangeCombo1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

}
