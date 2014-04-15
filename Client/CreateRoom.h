#pragma once
#include "afxwin.h"


// CCreateRoom 对话框
class COLList;
class CCreateRoom : public CDialogEx
{
	DECLARE_DYNAMIC(CCreateRoom)

public:
	CCreateRoom(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCreateRoom();

// 对话框数据
	enum { IDD = IDD_CREATEROOM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnEnChangeEdit1();
	CEdit						Roomname;
	void SetFeedbackDlg(COLList* v);
	COLList*					m_v;
};
