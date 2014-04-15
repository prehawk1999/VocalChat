#pragma once

#include <list>
#include <algorithm>
using namespace std;
// COLList
class CTalkDlg;
class CChatClient1Dlg;


		
class COLList : public CListCtrl
{
	DECLARE_DYNAMIC(COLList)

public:
	COLList();
	virtual ~COLList();
	void SetMainDlg(CChatClient1Dlg* pDlg);


private:
	CChatClient1Dlg*									m_pDlg;
public:
	int													m_page; //判断是双击哪一页列表
	CString												m_Roomname;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult);
	void RemoveAll(void);
	void AddItem(int ID, CString name);
	void Remove(CString name);
	//bool FindRoom(CString roomname, list<tkDlgInfo>::iterator& iri);
};


