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
		TRACE("�ɹ�ɾ���û�:%s, �����û�����%d \n", (*ici).nickname, m_LstClt.size() - 1);//test
		m_pDlg->m_OLList->Remove((*ici).nickname);
		m_LstClt.erase(ici);//��һ��Ҫ�������
	}
	else
	{
		TRACE("ɾ���û�����ʧ��! \n");
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
	case 0: // �л��������б�
		for(list<CltInfo>::iterator iter = m_LstClt.begin();iter != m_LstClt.end();++iter)
		{
			m_pDlg->m_OLList->AddItem((*iter).CltID, (*iter).nickname);
		}

		break;
	case 1:  // �л������ߺ���
		for(list<CltInfo>::iterator iter = m_LstClt.begin();iter != m_LstClt.end();++iter)
		{
			if((*iter).bFrd)
				m_pDlg->m_OLList->AddItem((*iter).CltID, (*iter).nickname);
		}
		
		break;

	case 2:  // �л��������б�
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
		//�Ҳ�������ͨ�ŶԻ���, ����֮
		CTalkDlg* tkDlg = new CTalkDlg;
		tkDlg->Create(IDD_TALK);
		tkDlg->ShowWindow(SW_SHOW);
		tkDlg->SetWindowTextA(strListText);
		tkDlg->GetDlgItem(IDC_EDIT1)->SetFocus();
		tkDlg->SetMainDlg(m_pDlg);
		//���Ƚ��Ի�ͨ��Ŀ����ص���Ϣ�Ž��Ի��������ĳ�Ա������
		tkDlg->m_dlgTitle		= strListText;
		(type == P2P_CHAT)
			?tkDlg->m_dlgID			= CltID
			:tkDlg->m_dlgID			= 0;
		tkDlg->m_dlgType			= type;
		tkDlg->m_Audio->InitAudioCollect(type, m_pDlg->m_Data.m_myID, CltID);
		tkDlg->m_Audio->SetMainDlg(m_pDlg);

		//�ѶԻ��������Ϣѹ�����
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
		//sourceID ��CltIDΪ0ʱ, �����֪ͨ�ͻ��������뵽���ǳ�, 
		//��ֻ��sourceIDΪ0, ��CltIDΪ���ͻ��˵��û�ID,
		//��ֻ��CltIDΪ0, ��sourceIDΪ�����ͻ��˵��û�ID(���������û���Ϣ)
		//////////////////////////////////////////////////////////////
		if(msg.sourceID == 0) 
		{
			GetDlgItemText(m_pLog->m_hWnd, IDC_EDIT1, szTemp, 256);
			if(msg.CltID == 0) 
			{
				//���յ�msg.textBufferΪ����Ϊ���벻���ǳ�, �����Լ����ǳ���ͬ������ɹ�.
				if(msg.textBuffer[0] == '\0')
				{
					m_pLog->MessageBox("�ǳ��Ѵ���! ", MB_OK);
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
				//���յ��Լ��Ŀͻ�ID�����ǳƹ�ȥ
				m_myID = msg.CltID;
				msg_feedback.type		=	LOGIN;
				msg_feedback.sourceID	=	m_pDlg->m_Data.m_myID;
				memcpy(msg_feedback.textBuffer, szTemp, 256);
				m_pDlg->m_Comm.SendMsg(msg_feedback);
			}
		}
		else
		{
			//�յ������û��ĸ�����Ϣ, �յ���textͬʱ�����ǳƺ����ڷ�����Ϣ
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
				strSplit.Format("%s�����Ϊ����", (*ici).nickname);
				if(m_pDlg->MessageBox(strSplit, 0, MB_OKCANCEL) == IDOK )
				{
					(*ici).bFrd							=	true;
				}			
			}
		}
		else
		{
			TRACE("���޴���!");
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
