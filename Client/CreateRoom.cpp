// CreateRoom.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ChatClient1.h"
#include "CreateRoom.h"
#include "afxdialogex.h"
#include "OLList.h"

// CCreateRoom �Ի���

IMPLEMENT_DYNAMIC(CCreateRoom, CDialogEx)

CCreateRoom::CCreateRoom(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCreateRoom::IDD, pParent)
{

}

CCreateRoom::~CCreateRoom()
{
}

void CCreateRoom::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, Roomname);
}


BEGIN_MESSAGE_MAP(CCreateRoom, CDialogEx)
	ON_BN_CLICKED(IDOK, &CCreateRoom::OnBnClickedOk)
	ON_EN_CHANGE(IDC_EDIT1, &CCreateRoom::OnEnChangeEdit1)
END_MESSAGE_MAP()


// CCreateRoom ��Ϣ�������

void CCreateRoom::SetFeedbackDlg(COLList* v)
{
	m_v = v;
}
void CCreateRoom::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strTemp;
	GetDlgItemText(IDC_EDIT1, (CString &)strTemp);
	m_v->m_Roomname = strTemp.GetBuffer(0);
	CDialogEx::OnOK();
}


void CCreateRoom::OnEnChangeEdit1()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�



	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}
