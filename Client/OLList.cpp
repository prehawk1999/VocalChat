// OLList.cpp : 实现文件
//

#include "stdafx.h"
#include "ChatClient1.h"
#include "ChatClient1Dlg.h"
#include "OLList.h"
#include "TalkDlg.h"
#include "CreateRoom.h"
#include "resource.h"
// COLList

IMPLEMENT_DYNAMIC(COLList, CListCtrl)

COLList::COLList()
{
	m_page = 0;
}

COLList::~COLList()
{
}


BEGIN_MESSAGE_MAP(COLList, CListCtrl)
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(NM_DBLCLK, &COLList::OnNMDblclk)
END_MESSAGE_MAP()



// COLList 消息处理程序


void COLList::SetMainDlg(CChatClient1Dlg* pDlg)
{
	m_pDlg = pDlg;
}


int COLList::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CListCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	return 0;
}


void COLList::RemoveAll(void)
{
	for(int i = 0; i < GetItemCount(); i++)	DeleteItem(i);
}


void COLList::AddItem(int ID, CString name)
{
	CString			strTemp;
	LVITEM			lvi;

	strTemp.Format("%d", ID);
	lvi.mask = LVIF_TEXT;
	lvi.iItem = GetItemCount();
	lvi.iSubItem = 0;
	lvi.pszText = strTemp.GetBuffer(0);
	lvi.cchTextMax = 32;
	InsertItem(&lvi);

	lvi.mask = LVIF_TEXT;
	lvi.pszText = name.GetBuffer(0);
	lvi.cchTextMax = 64;
	lvi.iSubItem = 1;
	SetItem(&lvi);
}

void COLList::Remove(CString name)
{
	CString str;
	for(int i = 0; i < GetItemCount(); i++){
		str = GetItemText(i,0);
		if( name.Compare(str) == 0){
			DeleteItem(i);
			break;
		}
	}
}




void COLList::OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult)
{
	int									iSel;
	list<CTalkDlg>::iterator			itd;
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	MsgInfo								msg_feedback;
	ZeroMemory(&msg_feedback, sizeof(msg_feedback) );

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if( (iSel = pNMListView->iItem) >= 0)
	{
		int											iID			= _wtoi( (const wchar_t*)GetItemText(iSel, 0).GetBuffer(0) );
		CString										strListText = GetItemText(iSel, 1);
		CString										strTalkTitle;

		switch (m_page)
		{
		case 0:
			//双击在线列表项: 添加好友
			msg_feedback.type				=	ADDFRD;
			msg_feedback.sourceID			=	m_pDlg->m_Data.m_myID;
			msg_feedback.CltID				=	iID;
			m_pDlg->m_Comm.SendMsg(msg_feedback);

			//只能用这招了, 给自己发送添加好友信息,不过不用对话框
			msg_feedback.sourceID			=	iID;
			msg_feedback.CltID				=	0;
			m_pDlg->m_Data.parseMsg(msg_feedback);
			break;

		case 1: //双击好友列表项: 打开通信对话框
			m_pDlg->m_Data.ActivateTalk(P2P_CHAT, iID, strListText);

			break;

		case 2: //双击房间列表项: 打开房间对话框
			m_pDlg->m_Data.m_location = strListText;
			m_pDlg->m_Data.ActivateTalk(P2A_CHAT, iID, strListText);


			break;

		default:
			break;
		}
	}
	else if(m_page == 2) //在房间列表双击空白处
	{
		CCreateRoom					crDlg;
		crDlg.SetFeedbackDlg(this);
		if(IDOK == crDlg.DoModal())
		{
			AddItem(GetItemCount() + 1, m_Roomname);
			msg_feedback.type				=	ENTER;
			msg_feedback.sourceID			=	m_pDlg->m_Data.m_myID;
			memcpy(msg_feedback.textBuffer, m_Roomname.GetBuffer(0), m_Roomname.GetLength() );
			m_pDlg->m_Data.m_LstRm.push_back(msg_feedback.textBuffer);
			m_pDlg->m_Comm.SendMsg(msg_feedback);
		}
	}

	*pResult = 0;
}
