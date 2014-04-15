#include "stdafx.h"
#include "ClientSocket.h"
#include "Msg.h"
#include <ws2tcpip.h>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

// 释放指针宏
#define RELEASE(x)                      {if(x != NULL ){delete x;x=NULL;}}
// 释放句柄宏
#define RELEASE_HANDLE(x)               {if(x != NULL && x!=INVALID_HANDLE_VALUE){ CloseHandle(x);x = NULL;}}
// 释放Socket宏
#define RELEASE_SOCKET(x)               {if(x !=INVALID_SOCKET) { closesocket(x);x=INVALID_SOCKET;}}

CClientSocket::CClientSocket(void)
{
}

CClientSocket::~CClientSocket(void)
{
	UnInitSocket();
}

void CClientSocket::SetMainDlg(CDialog* pDlg)
{
	m_pDlg = pDlg;
	InitSocket();
}

bool CClientSocket::InitSocket(void)
{
	WSADATA wsaData;
	if ( 0 != WSAStartup( MAKEWORD( 2, 2 ), &wsaData ) ) 
	{
		AfxMessageBox(_T("WSAStartup() error！"));
		return false;
	}
	if ( LOBYTE( wsaData.wVersion ) != 2 || HIBYTE( wsaData.wVersion ) != 2 ) 
	{
		AfxMessageBox(_T("Not winsock2.2！"));
		WSACleanup( );
		return false; 
	}
	return true;
}

bool CClientSocket::UnInitSocket(void)
{
	WSACleanup();
	return false;
}

bool CClientSocket::Start(CString strIP, int port)
{

	m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(INVALID_SOCKET == m_socket)
	{
		AfxMessageBox("socket() error！");
		return false;
	}
	
	struct sockaddr_in ServerAddress;
	ZeroMemory(&ServerAddress, sizeof(ServerAddress));

	ServerAddress.sin_family = AF_INET;
	ServerAddress.sin_addr.S_un.S_addr = inet_addr((const char*)strIP.GetString());
	ServerAddress.sin_port = htons(port);

	//使用WSAAsyncSelect模型
	if(SOCKET_ERROR==WSAAsyncSelect(m_socket, m_pDlg->m_hWnd, UM_SOCK, FD_READ | FD_CONNECT | FD_CLOSE))
	{
		AfxMessageBox(_T("WSAAsyncSelect() error！"));
		return false;
	}
	connect(m_socket, reinterpret_cast<const struct sockaddr *>(&ServerAddress),sizeof(ServerAddress));

	return true;
}


CString CClientSocket::GetLocalIP(void)
{

	char hostname[MAX_PATH];
	gethostname(hostname,MAX_PATH);                // 获得本机主机名
	struct hostent FAR* lpHostEnt = gethostbyname(hostname);

	if(lpHostEnt == NULL)
	{
		AfxMessageBox(_T("gethostbyname() error!"));
		return _T("");
	}

	LPSTR lpAddr = lpHostEnt->h_addr_list[0];      // 取得IP地址列表中的第一个为返回的IP

	struct in_addr inAddr;
	memmove(&inAddr,lpAddr,4);
	 // 转化成标准的IP地址形式
	return CString( inet_ntoa(inAddr) );
}


void CClientSocket::Stop(void)
{
	closesocket(m_socket);
}


bool CClientSocket::SendMsg(MsgInfo msg)
{
	WSABUF												DataBuf[4];
	DWORD												bytesSend = 0, Flags = 0;
	
	DataBuf[0].len									=	sizeof(msg.type);
	DataBuf[0].buf									=	(char*)&msg.type;
	
	DataBuf[1].len									=	sizeof(msg.sourceID);
	DataBuf[1].buf									=	(CHAR*)&msg.sourceID;

	DataBuf[2].len									=	sizeof(msg.CltID);
	DataBuf[2].buf									=	(CHAR*)&msg.CltID;

	DataBuf[3].len									=	sizeof(msg.textBuffer);
	DataBuf[3].buf									=	(CHAR*)&msg.textBuffer;
	
	//DataBuf[4].len									=	sizeof(msg.recordBuffer);
	//DataBuf[4].buf									=	(CHAR*)&msg.recordBuffer;

	if(SOCKET_ERROR == WSASend(m_socket, DataBuf, 4, &bytesSend, Flags, NULL, NULL) )
	{
		TRACE("WSASend error ErrorCode: %ld \n", WSAGetLastError() );
		return false;
	}
	return true;
}

bool CClientSocket::RecvMsg(MsgInfo& msgOut)
{
	MsgInfo												msg;
	ZeroMemory(&msg, sizeof(msg));

	WSABUF												DataBuf[4];
	DWORD												bytesRecv = 0, Flags = 0;

	DataBuf[0].len									=	sizeof(msg.type);
	DataBuf[0].buf									=	(CHAR*)&msg.type;

	DataBuf[1].len									=	sizeof(msg.sourceID);
	DataBuf[1].buf									=	(CHAR*)&msg.sourceID;

	DataBuf[2].len									=	sizeof(msg.CltID);
	DataBuf[2].buf									=	(CHAR*)&msg.CltID;

	DataBuf[3].len									=	sizeof(msg.textBuffer);
	DataBuf[3].buf									=	(CHAR*)&msg.textBuffer;

	//DataBuf[4].len									=	sizeof(msg.recordBuffer);
	//DataBuf[4].buf									=	(CHAR*)&msg.recordBuffer;

	WSARecv(m_socket, DataBuf, 4, &bytesRecv, &Flags, NULL, NULL);
	if(bytesRecv == sizeof(msg) )  // 首先投递一次Recv检验信息的类型
	{
		msgOut = msg;
		return true;
	}
	else
	{
		TRACE("接收信息长度不符!错误长度为:%d, ErrorCode: %ld", bytesRecv, WSAGetLastError() );
	}
	return false;
}
