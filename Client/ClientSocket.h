#pragma once

#include "Msg.h"
#define UM_SOCK  WM_USER + 1001
#define MAX_BUFFER							8192
#define DEFAULT_PORT						5310
#define DEFAULT_IP							_T("127.0.0.1")
#define MAX_BUFFER							8192
#define TEXT_BUFFER							2048


class CClientSocket
{
public:
	SOCKET					m_socket;

public:
	CClientSocket(void);
	~CClientSocket(void);
	void SetMainDlg(CDialog* pDlg);

public:
	CString GetLocalIP(void);
	bool Start(CString strIP = DEFAULT_IP, int port = DEFAULT_PORT);
	void Stop(void);
	bool SendMsg(MsgInfo msg);
	bool RecvMsg(MsgInfo& msgOut);
	bool InitSocket(void);
	bool UnInitSocket(void);

private:
	CDialog*				m_pDlg;//用来指定网络消息发送的窗口
};

