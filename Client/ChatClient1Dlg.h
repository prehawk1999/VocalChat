
// ChatClient1Dlg.h : 头文件
//

#pragma once
#include "resource.h"
#include "Msg.h"
#include "ClientSocket.h"
#include "afxcmn.h"
#define UM_SOCK		WM_USER + 1001
#define UM_FEED		WM_USER + 1002

class COLList;
class CTalkDlg;
// CChatClient1Dlg 对话框
class CChatClient1Dlg : public CDialogEx
{
// 构造
public:
	CChatClient1Dlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_CHATCLIENT1_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnUmSock(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUmFeed(WPARAM wParam, LPARAM lParam);
	afx_msg void OnClose();


	//自定义
public:
	void Init();
	void UnInit();
	CClientSocket										m_Comm;
	CMsg												m_Data;
	CLogin*												m_WndLogin;
	COLList*											m_OLList;
												
	void ResizeComboBox();
private:
	MsgInfo												msg_recv;
public:
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnCbnSelchangeCombo1();
	CTabCtrl											m_tab;
};
