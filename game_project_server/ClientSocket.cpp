// ClikentSocket.cpp : implementation file
//

#include "stdafx.h"
#include "game_project_server.h"
#include "ClientSocket.h"


// CClikentSocket

CClientSocket::CClientSocket()
{
}

CClientSocket::~CClientSocket()
{
}


// CClikentSocket member functions

void CClientSocket::SetWnd(HWND hWnd) {
	m_hWnd = hWnd;
}


void CClientSocket::OnClose(int nErrorCode)
{
	// TODO: Add your specialized code here and/or call the base class
	SendMessage(m_hWnd, WM_CLIENT_CLOSE, 0, (LPARAM)this);
	CSocket::OnClose(nErrorCode);
}


void CClientSocket::OnReceive(int nErrorCode)
{
	// TODO: Add your specialized code here and/or call the base class
	int header[2];
	
	Receive((char*)header, sizeof(int) * 2);
	//AfxMessageBox(_T("�� �߸������"));

	if (header[0] == 2) {
		CString strTmp = _T(""),strIPAddress=_T("");
		UINT uPortNum = 0;
		msgRecvMessage *msg = new msgRecvMessage;
		ZeroMemory(msg, sizeof(msgRecvMessage));
		Receive((char*)msg, header[1]);
		GetPeerName(strIPAddress, uPortNum);
		strTmp.Format(_T("[%s:%d:%d:%s]:%s"), strIPAddress, uPortNum,int(this), msg->name, msg->str);
		SendMessage(m_hWnd, WM_CLIENT_MSG_RECV, 0, (LPARAM)((LPCTSTR)strTmp));
		SendMessage(m_hWnd, WM_CLIENT_MSG_SEND, 0, (LPARAM)msg);
	}
	if (header[0] == 3) {
		CString strIPAddress;
		UINT uPortNum;
		GetPeerName(strIPAddress, uPortNum);
		this->m_IPAddress = strIPAddress;
		this->m_uPortNum = uPortNum;
		SendMessage(m_hWnd, WM_CLIENT_READY, 0, (LPARAM)this);
	}
	if (header[0] == 3000) {
		roomInfoRecvMessage *msg = new roomInfoRecvMessage;
		Receive((char *)msg, header[1]);
		Room *room = new Room;
		room->name = msg->name;
		room->kind = msg->kind;
		room->clientList.AddTail(this);
		SendMessage(m_hWnd, WM_CLIENT_ROOM_CREATE, 0, (LPARAM)room);
		//SendMessage(m_hWnd, WM_CLIENT_ROOM, 0, (LPARAM)room);
	}
	/***************** ���� ó�� �������� �渮��Ʈ �����޶�� ��û *****************/
	if (header[0] == 3002) {
		firstConnectStruct* fcs = new firstConnectStruct;
		Receive((char*)fcs, header[1]);
		if (fcs->isOnCreate == true) { // ���� �� ���ǹ��� �� �־�����
			SendMessage(m_hWnd, WM_CLIENT_IS_ON_CREATE, 0, (LPARAM)this);
		}
		delete fcs;
	}
	/*********************************************************************************/
	/****************** Ŭ���̾�Ʈ���� � �� �����ߴ��� �޴� �� *******************/
	if (header[0] == 4000) {
		attendRoomStruct* ars = new attendRoomStruct;
		Receive((char*)ars, header[1]);
		SendMessage(m_hWnd, WM_CLIENT_RECV_ROOM_POSITION, 0, (LPARAM)ars);
		delete ars;
	}
	/********************************************************************************/
	CSocket::OnReceive(nErrorCode);
}
