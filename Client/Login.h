#pragma once
#include "ChatClient1Dlg.h"
#include "afxcmn.h"
// CLogin �Ի���
class CLogin : public CDialogEx
{
	DECLARE_DYNAMIC(CLogin)

public:
	CLogin(CWnd* pParent = NULL);   // ��׼���캯��
	void SetMainDlg(CChatClient1Dlg* pDlg);
	virtual ~CLogin();

// �Ի�������
	enum { IDD = IDD_LOGIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedLogin();
	afx_msg void OnBnClickedCancel();

private:
	CChatClient1Dlg*					m_pDlg;
public:
	afx_msg void OnDestroy();
	CIPAddressCtrl m_InputIP;
	bool SetIP(DWORD dwIP);
};
