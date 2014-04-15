// TalkDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ChatClient1.h"
#include "ChatClient1Dlg.h"
#include "TalkDlg.h"
#include "afxdialogex.h"
#include "Msg.h"
#include "AudioCollect.h"

// CTalkDlg �Ի���

IMPLEMENT_DYNAMIC(CTalkDlg, CDialogEx)

CTalkDlg::CTalkDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTalkDlg::IDD, pParent)
{
	m_Audio = new CAudioCollect();
}

CTalkDlg::~CTalkDlg()
{
	m_Audio->Stop();
	delete m_Audio;
}


void CTalkDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTalkDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CTalkDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON2, &CTalkDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON1, &CTalkDlg::OnBnClickedButton1)
	ON_WM_DESTROY()
END_MESSAGE_MAP()

void CTalkDlg::SetMainDlg(CChatClient1Dlg* pDlg)
{
	m_pDlg = pDlg;
}


// CTalkDlg ��Ϣ�������


void CTalkDlg::OnBnClickedOk()
{
	// TODO: ����
	CString		strSend;
	MsgInfo		msg_send;
	ZeroMemory(&msg_send, sizeof(msg_send));
	GetDlgItemText(IDC_EDIT1, strSend);
	msg_send.type					=	m_dlgType;
	msg_send.sourceID				=	m_pDlg->m_Data.m_myID;
	msg_send.CltID					=	m_dlgID;
	memcpy(msg_send.textBuffer, strSend.GetBuffer(0), strSend.GetLength());
	if(strSend != "")
	{
		if(m_pDlg->m_Comm.SendMsg(msg_send))
		{
			PrintText(m_pDlg->m_Data.m_nickname, strSend);
			SetDlgItemText(IDC_EDIT1, "");
		}
		else
		{
			PrintText("���ݷ��ʹ���!", "");
		}
	}
	GetDlgItem(IDC_EDIT1)->SetFocus();


}


void CTalkDlg::OnBnClickedButton2()
{
	// TODO: �����Ļ
	CString str = "";
	SetDlgItemText(IDC_EDIT2, str);
	GetDlgItem(IDC_EDIT1)->SetFocus();
}


void CTalkDlg::OnBnClickedButton1()
{
	// TODO: ����ͨ��
	CString					buttonname;
	GetDlgItemText(IDC_BUTTON1, buttonname);
	if(buttonname == "����ͨ��")
	{
		m_Audio->Start();
		SetDlgItemText(IDC_BUTTON1, "�ر�����");
	}
	else
	{
		m_Audio->Stop();
		SetDlgItemText(IDC_BUTTON1, "����ͨ��");
	}



	GetDlgItem(IDC_EDIT1)->SetFocus();
}

void CTalkDlg::PrintText(CString name, CString context)
{
	CString strTime, strResult, strExist;
	GetDlgItemText(IDC_EDIT2, strExist);
	SYSTEMTIME st;	//����ϵͳʱ��
	GetLocalTime(&st);
	strTime.Format("%2d/%2d/%2d - %2d:%2d:%2d",st.wYear, st.wMonth, st.wDay, st.wHour,st.wMinute,st.wSecond);
	strResult.Format("%s [%s]\r\n%s\r\n", name, strTime, context);
	strResult += "\r\n";
	strResult += strExist;
	SetDlgItemText(IDC_EDIT2, strResult);
}


void CTalkDlg::OnDestroy()
{
	this->ShowWindow(SW_HIDE);
	m_Audio->Stop();
	// TODO: �ڴ˴������Ϣ����������
	list<tkDlgInfo>::iterator			idi;
	CString								str;
	GetWindowText(str);
	if(m_pDlg->m_Data.FindDialog(str, idi))
	{
		(*idi).bAct = false;
	}

}
