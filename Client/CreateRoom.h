#pragma once
#include "afxwin.h"


// CCreateRoom �Ի���
class COLList;
class CCreateRoom : public CDialogEx
{
	DECLARE_DYNAMIC(CCreateRoom)

public:
	CCreateRoom(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCreateRoom();

// �Ի�������
	enum { IDD = IDD_CREATEROOM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnEnChangeEdit1();
	CEdit						Roomname;
	void SetFeedbackDlg(COLList* v);
	COLList*					m_v;
};
