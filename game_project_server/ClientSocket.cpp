// ClikentSocket.cpp : implementation file
//

#include "stdafx.h"
#include "game_project_server.h"
#include "ClientSocket.h"


// CClikentSocket

CClientSocket::CClientSocket()
{
	roomKind = 0;
	rsp_choice = 0;
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
	//AfxMessageBox(_T("방 잘만들어짐"));

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

		delete msg;
	}
	//오델로 레디 버튼
	if (header[0] == 3) {
		readyRecvMessage *msg = new readyRecvMessage;
		ZeroMemory(msg, sizeof(readyRecvMessage));
		Receive((char*)msg, header[1]);
		this->roomID;
		SendMessage(m_hWnd, WM_CLIENT_READY, 0, (LPARAM)this);
		delete msg;
	}
	//오델로 맵 받기
	if (header[0] == 8) {
		mapStruct *map = new mapStruct;
		ZeroMemory(map, sizeof(mapStruct));
		Receive((char*)map, header[1]);
		this->roomID = map->roomID;
		SendMessage(m_hWnd, WM_CLIENT_SEND_MAP, 0, (LPARAM)map);
	}
	if (header[0] == 50) {
		othelloMsgStruct *msg = new othelloMsgStruct;
		ZeroMemory(msg,sizeof(othelloMsg));
		Receive((char*)msg, header[1]);
		CString str;

		str.Format(_T("[Room:%d client:%d]:%s"), msg->roomID, int(this), msg->msg);
		SendMessage(m_hWnd, WM_CLIENT_OTHELLO_MSG, 0, (LPARAM)((LPCTSTR)str));
		SendMessage(m_hWnd, WM_CLIENT_OTHELLO_MSG_SEND, 0, (LPARAM)msg);
	}
	//가위바위보
	if (header[0] == 401) {
		choiceStruct *msg = new choiceStruct;
		ZeroMemory(msg, sizeof(choiceStruct));
		Receive((char*)msg, header[1]);
		this->rsp_choice = msg->choice;
		this->roomID = msg->roomID;
		SendMessage(m_hWnd, WM_CLIENT_ROCK_CHOICE, 0, (LPARAM)this);
		delete msg;
	}
	if (header[0] == 3000) {
		roomInfoRecvMessage *msg = new roomInfoRecvMessage;
		Receive((char *)msg, header[1]);
		Room *room = new Room;
		room->name = msg->name;
		room->kind = msg->kind;
		this->nickName = msg->ClientName;
		room->clientList.AddTail(this);
		SendMessage(m_hWnd, WM_CLIENT_ROOM_CREATE, 0, (LPARAM)room);
		delete msg;
		//SendMessage(m_hWnd, WM_CLIENT_ROOM, 0, (LPARAM)room);
	}
	/***************** 대기방 처음 켜졌을때 방리스트 보내달라는 요청 *****************/
	if (header[0] == 3002) {
		firstConnectStruct* fcs = new firstConnectStruct;
		Receive((char*)fcs, header[1]);
		if (fcs->isOnCreate == true) { // 내가 이 조건문을 왜 넣었을까
			SendMessage(m_hWnd, WM_CLIENT_IS_ON_CREATE, 0, (LPARAM)this);
		}
		delete fcs;
	}
	/*********************************************************************************/
	/****************** 클라이언트에서 어떤 방 선택했는지 받는 곳 *******************/
	if (header[0] == 4000) {
		attendRoomStruct* ars = new attendRoomStruct;
		Receive((char*)ars, header[1]);
		this->roomID = ars->roomPosition;
		this->nickName = ars->ClientName;
		SendMessage(m_hWnd, WM_CLIENT_RECV_ROOM_POSITION, 0, (LPARAM)this);
		delete ars;
	}

	//게임방 나갈때 
	if (header[0] == 5005) {
		//AfxMessageBox(_T("왔따왔다"));
		createRoomStruct *msg = new createRoomStruct;
		ZeroMemory(msg, sizeof(createRoomStruct));
		Receive((char*)msg, header[1]);
		//CString str;
		//str.Format(_T("%d | %d"), msg->roomID, msg->roomKind);
		//AfxMessageBox(str);
		this->roomID = msg->roomID;
		this->roomKind = msg->roomKind;
		SendMessage(m_hWnd, WM_CLIENT_GAME_CLOSE, 0, (LPARAM)this);
		delete msg;
	}
	/********************************************************************************/

	/****************** 짝맞추기에서 오는 메세지 ***********************/
	if (header[0] == 5000) {
		cardMsgStruct *msg = new cardMsgStruct;
		ZeroMemory(msg, sizeof(cardMsg));
		Receive((char*)msg, header[1]);
		CString str;
		str.Format(_T("[Room:%d client:%d]:%s"), msg->roomID, int(this), msg->msg);
		SendMessage(m_hWnd, WM_CLIENT_CARD_MSG, 0, (LPARAM)((LPCTSTR)str));
		SendMessage(m_hWnd, WM_CLIENT_CARD_MSG_SEND, 0, (LPARAM)msg);
	}
	if (header[0] == 5400) {
		cardReadyStruct* crs = new cardReadyStruct;
		ZeroMemory(crs, sizeof(cardReady));
		Receive((char*)crs, header[1]);
		SendMessage(m_hWnd, WM_CLIENT_CARD_IS_READY, 0, (LPARAM)crs);
	}
	/*******************************************************************/
	CSocket::OnReceive(nErrorCode);
}
