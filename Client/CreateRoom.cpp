// CreateRoom.cpp : 实现文件
//

#include "stdafx.h"
#include "ChatClient1.h"
#include "CreateRoom.h"
#include "afxdialogex.h"
#include "OLList.h"

// CCreateRoom 对话框

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


// CCreateRoom 消息处理程序

void CCreateRoom::SetFeedbackDlg(COLList* v)
{
	m_v = v;
}
void CCreateRoom::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strTemp;
	GetDlgItemText(IDC_EDIT1, (CString &)strTemp);
	m_v->m_Roomname = strTemp.GetBuffer(0);
	CDialogEx::OnOK();
}


void CCreateRoom::OnEnChangeEdit1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。



	// TODO:  在此添加控件通知处理程序代码
}
