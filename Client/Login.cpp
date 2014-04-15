// Login.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ChatClient1.h"
#include "Login.h"
#include "afxdialogex.h"
#include "Msg.h"

// CLogin �Ի���

IMPLEMENT_DYNAMIC(CLogin, CDialogEx)

CLogin::CLogin(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLogin::IDD, pParent)
{
	
}

void CLogin::SetMainDlg(CChatClient1Dlg* pDlg)
{
	m_pDlg = pDlg;

}

CLogin::~CLogin()
{
}

void CLogin::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IPADDRESS1, m_InputIP);
}


BEGIN_MESSAGE_MAP(CLogin, CDialogEx)
	ON_BN_CLICKED(IDC_LOGIN, &CLogin::OnBnClickedLogin)
	ON_BN_CLICKED(IDC_CANCEL, &CLogin::OnBnClickedCancel)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CLogin ��Ϣ�������


void CLogin::OnBnClickedLogin()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	//��������
	unsigned   char   *pIP;  
	CString   strIP;  
	DWORD   dwIP;  
	m_InputIP.GetAddress(dwIP);  
	pIP   =   (unsigned   char*)&dwIP;  
	strIP.Format("%u.%u.%u.%u",*(pIP+3),   *(pIP+2),   *(pIP+1),   *pIP);
	m_pDlg->m_Comm.Start(m_pDlg->m_Comm.GetLocalIP());  //
	GetDlgItem(IDC_LOGIN)->EnableWindow(false);
}


void CLogin::OnBnClickedCancel()
{
	OnCancel();
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void CLogin::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: �ڴ˴������Ϣ����������
}


bool CLogin::SetIP(DWORD dwIP)
{
	m_InputIP.SetAddress(dwIP);
	return false;
}
