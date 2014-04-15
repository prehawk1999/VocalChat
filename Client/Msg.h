#pragma once
#include <vector>
#include <list>


#define MAX_BUFFER							8192
#define TEXT_BUFFER							2048

using namespace std;
class CClientSocket;
class CChatClient1Dlg;
class CLogin;
class CTalkDlg;
//��Ϣ����
enum MSG_TYPE
{
	LOGIN,				//2 �ͻ��������ǳ�ע��
	LOGOUT,
	ADDFRD,				//2 �Ӻ���
	P2P_CHAT,			//2 ���Ե�����ͨ��
	P2P_TALK,			//2 ���Ե�����ͨ��
	ENTER,				//1 ���뷿��
	LEAVE,				//1 �뿪����
	P2A_CHAT,			//2 ���Զ���������
	P2A_TALK			//2 ���Զ�����ͨ��
};

//��Ϣ��ṹ��, ������Ϣ�Ĵ��ݷ�ʽ
typedef struct MSG_INFO
{
	MSG_TYPE						type;
	int								sourceID;
	int								CltID;
	char							textBuffer[TEXT_BUFFER];
	//char							recordBuffer[RECORD_BUFFER];
}MsgInfo, *pMsgInfo;

//�ͻ��˱���, ��¼ÿ������Ŀͻ��˵���Ϣ
typedef struct CLIENT_INFO
{
	int							CltID;
	CString						nickname;
	CString						location;
	bool						bFrd;

}CltInfo, *pCltInfo;


//�ͻ��򿪵ĶԻ����ڱ�
typedef struct TALKDLG_LIST
{
	bool			bAct;
	MSG_TYPE		type;
	int				CltID;
	CString			dlgTitle;
	CTalkDlg*		pTalkDlg;
}tkDlgInfo, *ptkDlgInfo;





//�ͻ��˹����û��б���Ϣ���� 
class CMsg
{
	//���캯��
public:
	CMsg(void);
	~CMsg(void);

	//�ⲿ��������
public:
	bool SetMainDlg(CChatClient1Dlg* pDlg);
	bool SetLoginDlg(CLogin* pLog);
	bool AddUser(int CltID, CString nickname, CString location);
	bool ReleaseUser(int CltID);
	bool parseMsg(MsgInfo msg);
	void SwitchList(int num);
	void ActivateTalk(MSG_TYPE type, int CltID, CString strListText);

	int														m_myID;
	CString													m_nickname;
	CString													m_location;


	//δ����

	//����
public:
	CClientSocket*											m_CltSock;		//useless	

	CChatClient1Dlg*										m_pDlg;
	CLogin*													m_pLog;
	list<CltInfo>											m_LstClt;
	list<tkDlgInfo>											m_LstTkDlg;
	vector<CString>											m_LstRm;

	bool FindRoom(CString roomname, vector<CString>::iterator& iri);
	bool FindUser(int CltID, list<CltInfo>::iterator& ici);
	bool FindDialog(MSG_TYPE type, int CltID, list<tkDlgInfo>::iterator& iri);
	bool FindDialog(CString dlgTitle, list<tkDlgInfo>::iterator& iri);
};

