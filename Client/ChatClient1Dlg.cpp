
// ChatClient1Dlg.cpp : 实现文件
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


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	// 实现
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


// CChatClient1Dlg 对话框



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


// CChatClient1Dlg 消息处理程序

BOOL CChatClient1Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	//创建ClientSocket对象和msg对象并进行对象的初始化
	this->Init();
	//CLogin	LogDlg;
	//m_WndLogin = &LogDlg; //不知行不行
	m_WndLogin->DoModal();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CChatClient1Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
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



	//设置默认IP
	
	DWORD   dwIP;  
	CString csIP = m_Comm.GetLocalIP();
	dwIP   =   inet_addr(csIP); 
	//m_WndLogin->SetIP(dwIP);

	ZeroMemory(&msg_recv, sizeof(msg_recv));

	((CTabCtrl*)GetDlgItem(IDC_TAB1))->InsertItem(0, "在线用户");
	((CTabCtrl*)GetDlgItem(IDC_TAB1))->InsertItem(1, "好友列表");
	((CTabCtrl*)GetDlgItem(IDC_TAB1))->InsertItem(2, "房间列表");

	((CComboBox*)GetDlgItem(IDC_COMBO1))->AddString("在线");
	((CComboBox*)GetDlgItem(IDC_COMBO1))->AddString("离开");


	//创建列表窗口

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
	m_OLList->InsertColumn(1,"操作列表项",LVCFMT_LEFT, 180);
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


//此函数响应服务端传来的信息
afx_msg LRESULT CChatClient1Dlg::OnUmSock(WPARAM wParam, LPARAM lParam)
{
	if(WSAGETSELECTERROR(lParam))
	{
		TRACE("网络消息接收异常! 错误代码: %d \n", WSAGETSELECTERROR(lParam));
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
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	this->UnInit();
	CDialogEx::OnClose();
}

//而此函数响应客户端自身CMsg类处理好的消息, 并由主对话框来处理界面呈现 没用...
afx_msg LRESULT CChatClient1Dlg::OnUmFeed(WPARAM wParam, LPARAM lParam)
{
	return 0;
}


void CChatClient1Dlg::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	int CurSel = m_tab.GetCurSel();
	m_Data.SwitchList(CurSel);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}


void CChatClient1Dlg::OnCbnSelchangeCombo1()
{
	// TODO: 在此添加控件通知处理程序代码

}
