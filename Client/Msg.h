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
//消息类型
enum MSG_TYPE
{
	LOGIN,				//2 客户端请求昵称注册
	LOGOUT,
	ADDFRD,				//2 加好友
	P2P_CHAT,			//2 单对单文字通信
	P2P_TALK,			//2 单对单语音通信
	ENTER,				//1 进入房间
	LEAVE,				//1 离开房间
	P2A_CHAT,			//2 单对多文字聊天
	P2A_TALK			//2 单对多语音通信
};

//信息体结构体, 定义信息的传递方式
typedef struct MSG_INFO
{
	MSG_TYPE						type;
	int								sourceID;
	int								CltID;
	char							textBuffer[TEXT_BUFFER];
	//char							recordBuffer[RECORD_BUFFER];
}MsgInfo, *pMsgInfo;

//客户端表项, 记录每个连入的客户端的信息
typedef struct CLIENT_INFO
{
	int							CltID;
	CString						nickname;
	CString						location;
	bool						bFrd;

}CltInfo, *pCltInfo;


//客户打开的对话窗口表
typedef struct TALKDLG_LIST
{
	bool			bAct;
	MSG_TYPE		type;
	int				CltID;
	CString			dlgTitle;
	CTalkDlg*		pTalkDlg;
}tkDlgInfo, *ptkDlgInfo;





//客户端管理用户列表及消息的类 
class CMsg
{
	//构造函数
public:
	CMsg(void);
	~CMsg(void);

	//外部操作函数
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


	//未定义

	//属性
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

