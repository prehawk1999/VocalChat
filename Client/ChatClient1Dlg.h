
// ChatClient1Dlg.h : ͷ�ļ�
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
// CChatClient1Dlg �Ի���
class CChatClient1Dlg : public CDialogEx
{
// ����
public:
	CChatClient1Dlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_CHATCLIENT1_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnUmSock(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUmFeed(WPARAM wParam, LPARAM lParam);
	afx_msg void OnClose();


	//�Զ���
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
