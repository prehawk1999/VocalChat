#pragma once


// CTalkDlg 对话框
class CChatClient1Dlg;
class CAudioCollect;
class CTalkDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTalkDlg)

public:
	CTalkDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTalkDlg();
	void SetMainDlg(CChatClient1Dlg* pDlg);
// 对话框数据
	enum { IDD = IDD_TALK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CChatClient1Dlg*							m_pDlg;
	CString										m_dlgTitle;
	MSG_TYPE									m_dlgType;
	int											m_dlgID;
	CAudioCollect*								m_Audio;

	void PrintText(CString name, CString context);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnDestroy();
};
