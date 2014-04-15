#include "stdafx.h"
#include "Msg.h"
#include "ChatClient1Dlg.h"
#include "Login.h"
#include "OLList.h"
#include "TalkDlg.h"
#include "AudioCollect.h"

CMsg::CMsg(void)
{
}


CMsg::~CMsg(void)
{
	m_LstClt.clear();
	m_LstRm.clear();
	m_LstTkDlg.clear();
}


bool CMsg::SetMainDlg(CChatClient1Dlg* pDlg)
{
	m_pDlg = pDlg;
	return false;
}


bool CMsg::SetLoginDlg(CLogin* pLog)
{
	m_pLog = pLog;
	return false;
}

bool CMsg::FindDialog(MSG_TYPE type, int CltID, list<tkDlgInfo>::iterator& iri)
{
	for(list<tkDlgInfo>::iterator iter = m_LstTkDlg.begin(); iter != m_LstTkDlg.end(); iter++)
	{
		if((*iter).type == type)
		{
			if( (*iter).CltID == CltID)
			{
				iri = iter;
				return true;
			}
		}
	}

	return false;
}

bool CMsg::FindDialog(CString dlgTitle, list<tkDlgInfo>::iterator& iri)
{
	for(list<tkDlgInfo>::iterator iter = m_LstTkDlg.begin(); iter != m_LstTkDlg.end(); iter++)
	{
		if((*iter).dlgTitle == dlgTitle)
		{
			iri = iter;
			return true;
		}
	}
	return false;
}

bool CMsg::FindUser(int CltID, list<CltInfo>::iterator& ici)
{
	for( list<CltInfo>::iterator iter = m_LstClt.begin(); iter != m_LstClt.end(); ++iter)
	{
		if( CltID == (*iter).CltID)
		{
			ici = iter;
			return true;
		}
	}
	return false;
}

bool CMsg::FindRoom(CString roomname, vector<CString>::iterator& iri)
{
	for( vector<CString>::iterator iter = m_LstRm.begin(); iter != m_LstRm.end(); ++iter)
	{
		if( roomname == (*iter))
		{
			iri = iter;
			return true;
		}
	}
	return false;
}


bool CMsg::AddUser(int CltID, CString nickname, CString location)
{
	CltInfo										ci;
	ci.CltID								=	CltID;
	ci.nickname								=	nickname;
	ci.location								=	location;
	ci.bFrd									=	false;
	m_LstClt.push_back(ci);
	m_pDlg->m_OLList->AddItem(CltID, nickname);
	TRACE("ID: %d Nick: %s Location: %s \n", ci.CltID, ci.nickname, ci.location);//test
	return false;
}

bool CMsg::ReleaseUser(int CltID)
{
	list<CltInfo>::iterator						ici;
	if(FindUser(CltID, ici))
	{
		TRACE("成功删除用户:%s, 现有用户数量%d \n", (*ici).nickname, m_LstClt.size() - 1);//test
		m_pDlg->m_OLList->Remove((*ici).nickname);
		m_LstClt.erase(ici);//这一句要放在最后
	}
	else
	{
		TRACE("删除用户操作失败! \n");
	}
	return false;
}

void CMsg::SwitchList(int num)
{
	int i = 1;
	m_pDlg->m_OLList->DeleteAllItems();
	m_pDlg->m_OLList->m_page = num;
	switch (num)
	{
	case 0: // 切换到在线列表
		for(list<CltInfo>::iterator iter = m_LstClt.begin();iter != m_LstClt.end();++iter)
		{
			m_pDlg->m_OLList->AddItem((*iter).CltID, (*iter).nickname);
		}

		break;
	case 1:  // 切换到在线好友
		for(list<CltInfo>::iterator iter = m_LstClt.begin();iter != m_LstClt.end();++iter)
		{
			if((*iter).bFrd)
				m_pDlg->m_OLList->AddItem((*iter).CltID, (*iter).nickname);
		}
		
		break;

	case 2:  // 切换到房间列表
		for(vector<CString>::iterator iter = m_LstRm.begin(); iter != m_LstRm.end(); ++iter)
		{
			m_pDlg->m_OLList->AddItem(i, (*iter) );
			i++;
		}


		break;
	default:
		break;
	}

}

void CMsg::ActivateTalk(MSG_TYPE type, int CltID, CString strListText)
{
	list<tkDlgInfo>::iterator					iri;
	if(FindDialog(type, CltID, iri))
	{
		(*iri).pTalkDlg->SetActiveWindow();
		(*iri).pTalkDlg->GetDlgItem(IDC_EDIT1)->SetFocus();
		(*iri).pTalkDlg->ShowWindow(SW_SHOW);
		(*iri).bAct							=	true;
	}
	else
	{
		//找不到现有通信对话框, 创建之
		CTalkDlg* tkDlg = new CTalkDlg;
		tkDlg->Create(IDD_TALK);
		tkDlg->ShowWindow(SW_SHOW);
		tkDlg->SetWindowTextA(strListText);
		tkDlg->GetDlgItem(IDC_EDIT1)->SetFocus();
		tkDlg->SetMainDlg(m_pDlg);
		//首先将对话通信目标相关的信息放进对话框对象里的成员变量里
		tkDlg->m_dlgTitle		= strListText;
		(type == P2P_CHAT)
			?tkDlg->m_dlgID			= CltID
			:tkDlg->m_dlgID			= 0;
		tkDlg->m_dlgType			= type;
		tkDlg->m_Audio->InitAudioCollect(type, m_pDlg->m_Data.m_myID, CltID);
		tkDlg->m_Audio->SetMainDlg(m_pDlg);

		//把对话框及相关信息压入表里
		tkDlgInfo*		tkInfo = new tkDlgInfo;
		tkInfo->bAct		= true;
		tkInfo->type		= type;
		(type == P2P_CHAT)
			?tkInfo->CltID	= CltID
			:tkInfo->CltID	= 0;
		tkInfo->pTalkDlg = tkDlg;
		tkInfo->dlgTitle = strListText;
		m_LstTkDlg.push_back(*tkInfo);
	}
}


bool CMsg::parseMsg(MsgInfo msg)
{
	CString										strSplit, strLeft, strRight;
	int											posSplit;
	char										szTemp[256];
	MsgInfo										msg_feedback;
	list<CltInfo>::iterator						ici;
	list<tkDlgInfo>::iterator					idi;
	vector<CString>::iterator					iri;

	ZeroMemory(&msg_feedback, sizeof(msg_feedback));
	switch (msg.type)
	{
	case LOGIN:

		///////////////////////////////////////////////////////
		//sourceID 跟CltID为0时, 服务端通知客户端其申请到的昵称, 
		//若只有sourceID为0, 则CltID为本客户端的用户ID,
		//若只有CltID为0, 则sourceID为其他客户端的用户ID(更新其他用户信息)
		//////////////////////////////////////////////////////////////
		if(msg.sourceID == 0) 
		{
			GetDlgItemText(m_pLog->m_hWnd, IDC_EDIT1, szTemp, 256);
			if(msg.CltID == 0) 
			{
				//若收到msg.textBuffer为空则为申请不到昵称, 若跟自己的昵称相同则申请成功.
				if(msg.textBuffer[0] == '\0')
				{
					m_pLog->MessageBox("昵称已存在! ", MB_OK);
				}
				else
				{
					m_pDlg->m_Data.m_nickname		=	msg.textBuffer;
					m_pDlg->m_Data.m_location		=	"hall";
					m_pLog->OnBnClickedCancel();
					m_pDlg->GetDlgItem(IDC_LOGINNAME)->SetWindowTextA(msg.textBuffer);
					m_pDlg->SetWindowTextA(msg.textBuffer);
				}
				m_pDlg->m_WndLogin->GetDlgItem(IDC_LOGIN)->EnableWindow(true);

			}
			else 
			{
				//若收到自己的客户ID则发送昵称过去
				m_myID = msg.CltID;
				msg_feedback.type		=	LOGIN;
				msg_feedback.sourceID	=	m_pDlg->m_Data.m_myID;
				memcpy(msg_feedback.textBuffer, szTemp, 256);
				m_pDlg->m_Comm.SendMsg(msg_feedback);
			}
		}
		else
		{
			//收到其他用户的更新信息, 收到的text同时包含昵称和所在房间信息
			strSplit = msg.textBuffer;
			posSplit = strSplit.Find('|');
			strLeft = strSplit.Left(posSplit);
			strRight = strSplit.Mid(posSplit + 1);
			m_pDlg->m_Data.AddUser(msg.sourceID, strLeft, strRight);
		}

		break;		
	case LOGOUT:
		m_pDlg->m_Data.ReleaseUser(msg.sourceID);

		break;
	case ADDFRD:

		if(FindUser(msg.sourceID, ici))
		{
			if(msg.CltID == 0)
				(*ici).bFrd							=	true;
			else
			{
				strSplit.Format("%s想加你为好友", (*ici).nickname);
				if(m_pDlg->MessageBox(strSplit, 0, MB_OKCANCEL) == IDOK )
				{
					(*ici).bFrd							=	true;
				}			
			}
		}
		else
		{
			TRACE("查无此人!");
		}
		


		break;
	case ENTER:
		if(FindUser(msg.sourceID, ici))
		{
			(*ici).location = msg.textBuffer;
		}
		if(!FindRoom(msg.textBuffer, iri))m_LstRm.push_back(msg.textBuffer); //test

		break;
	case LEAVE:
		if(FindUser(msg.sourceID, ici))
		{
			(*ici).location = "hall";
		}


		break;
	case P2P_CHAT:
		if(FindDialog(P2P_CHAT, msg.sourceID, idi))
		{
			if(FindUser(msg.sourceID, ici))
			{
				if((*idi).bAct) 
				{
					(*idi).pTalkDlg->SetActiveWindow();
					(*idi).pTalkDlg->GetDlgItem(IDC_EDIT1)->SetFocus();
					(*idi).pTalkDlg->PrintText( (*ici).nickname, msg.textBuffer);
				}
				else
				{
				}
			}
		}

		break;
	case P2A_CHAT:
		if(FindDialog(P2A_CHAT, 0, idi))
		{
			if((*idi).bAct) 
				(*idi).pTalkDlg->PrintText( (*idi).dlgTitle, msg.textBuffer);
		}

		break;

	case P2P_TALK:
		if(FindDialog(P2P_CHAT, msg.sourceID, idi))
		{
			(*idi).pTalkDlg->m_Audio->RecvWaveClips(msg);
		}

		break;
	case P2A_TALK:
		//FindUser(msg.sourceID, ici);
		if(FindDialog(P2A_CHAT, 0, idi))
		{
			(*idi).pTalkDlg->m_Audio->RecvWaveClips(msg);
		}
		break;
	default:
		break;
	}

	return false;
}
