
// game_project_serverDlg.cpp : implementation file
//

#include "stdafx.h"
#include "game_project_server.h"
#include "game_project_serverDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Cgame_project_serverDlg dialog
#define PORT 9999


Cgame_project_serverDlg::Cgame_project_serverDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_GAME_PROJECT_SERVER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void Cgame_project_serverDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_CLIENT, m_list_client);
	DDX_Control(pDX, IDC_LIST_MSG, m_list_msg);
	DDX_Control(pDX, IDC_LIST4, m_room_client_list);
	DDX_Control(pDX, IDC_LIST_ROOM, m_list_room);
}

BEGIN_MESSAGE_MAP(Cgame_project_serverDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_ACCEPT_SOCKET, &Cgame_project_serverDlg::OnAcceptSocket)
	ON_MESSAGE(WM_CLIENT_MSG_RECV, &Cgame_project_serverDlg::OnClientMsgRecv)
	ON_MESSAGE(WM_CLIENT_CLOSE, &Cgame_project_serverDlg::OnClientClose)
	ON_MESSAGE(WM_CLIENT_MSG_SEND, &Cgame_project_serverDlg::OnClientMsgSend)
	ON_MESSAGE(WM_CLIENT_READY, &Cgame_project_serverDlg::OnClientReady)
	ON_MESSAGE(WM_CLIENT_ROOM_CREATE, &Cgame_project_serverDlg::OnClientRoomCreate)
	ON_MESSAGE(WM_CLIENT_ROOM, &Cgame_project_serverDlg::OnClientRoom)
	ON_MESSAGE(WM_CLIENT_IS_ON_CREATE, &Cgame_project_serverDlg::OnClientIsOnCreate)
	ON_MESSAGE(WM_CLIENT_RECV_ROOM_POSITION, &Cgame_project_serverDlg::OnClientRecvRoomPosition)
	ON_MESSAGE(WM_CLIENT_OTHELLO_MSG, &Cgame_project_serverDlg::OnClientOthelloMsg)
	ON_MESSAGE(WM_CLIENT_OTHELLO_MSG_SEND, &Cgame_project_serverDlg::OnClientOthelloMsgSend)
	ON_MESSAGE(WM_CLIENT_CARD_MSG, &Cgame_project_serverDlg::OnClientCardMsg)
	ON_MESSAGE(WM_CLIENT_CARD_MSG_SEND, &Cgame_project_serverDlg::OnClientCardMsgSend)
END_MESSAGE_MAP()


// Cgame_project_serverDlg message handlers

BOOL Cgame_project_serverDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	m_serverSocket = new CServerSocket;
	m_serverSocket->SetWnd(this->m_hWnd);

	m_serverSocket->Create(PORT);
	m_serverSocket->Listen();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void Cgame_project_serverDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR Cgame_project_serverDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



afx_msg LRESULT Cgame_project_serverDlg::OnAcceptSocket(WPARAM wParam, LPARAM lParam)
{
	CString str;
	m_clientSocket = (CClientSocket*)lParam;
	m_clientSocket->m_ready = false;
	m_ptrClientSocketList.AddTail(m_clientSocket);

	str.Format(_T("Client (%d)"), int(m_clientSocket));
	m_list_client.InsertString(-1, str);

	m_clientSocket = NULL;
	delete m_clientSocket;
	return 0;
}


afx_msg LRESULT Cgame_project_serverDlg::OnClientMsgRecv(WPARAM wParam, LPARAM lParam)
{
	LPCTSTR lpData = (LPCTSTR)lParam;

	m_list_msg.InsertString(-1, lpData);
	m_list_msg.SetCurSel(m_list_msg.GetCount() - 1);
	return 0;
}


afx_msg LRESULT Cgame_project_serverDlg::OnClientClose(WPARAM wParam, LPARAM lParam)
{
	CClientSocket *pClient = (CClientSocket*)lParam;
	CString str,strTmp;
	UINT idx = 0;
	POSITION pos = m_ptrClientSocketList.Find(pClient);

	strTmp.Format(_T("[%d]:close"), int(pClient));
	m_list_msg.InsertString(-1, strTmp);
	m_list_msg.SetCurSel(m_list_msg.GetCount() - 1);
	if (pos != NULL) {
		str.Format(_T("Client (%d)"), (int)pClient);
		idx = m_list_client.SelectString(-1, (LPCTSTR)str);
		m_list_client.DeleteString(idx);

		m_ptrClientSocketList.RemoveAt(pos);
	}

	pos = m_room.clientList.Find(pClient);
	if (pos != NULL){
		m_room.clientList.RemoveAt(pos);
	}
	delete pClient;
	return 0;
}


afx_msg LRESULT Cgame_project_serverDlg::OnClientMsgSend(WPARAM wParam, LPARAM lParam)
{
	msgRecvMessage *lpData = (msgRecvMessage*)lParam;
	POSITION pos = m_ptrClientSocketList.GetHeadPosition();

	while (pos != NULL) {
		CClientSocket *pClient = (CClientSocket*)m_ptrClientSocketList.GetNext(pos);
		if (pClient != NULL) {
			msgMessage *msg = new msgMessage;
			msg->id = 2;
			msg->size = sizeof(msgRecvMessage);
			_tcscpy_s(msg->data.str,lpData->str);
			_tcscpy_s(msg->data.name, lpData->name);
			pClient->Send((char*)msg, sizeof(msgMessage));
		}
	}

	return 0;
}


afx_msg LRESULT Cgame_project_serverDlg::OnClientReady(WPARAM wParam, LPARAM lParam)
{
	CClientSocket *pClient = (CClientSocket*)lParam;
	CClientSocket *tmp;
	

	POSITION pos = m_ptrClientSocketList.Find(pClient);
	tmp=(CClientSocket*)m_ptrClientSocketList.GetAt(pos);
	if (m_room.clientList.GetCount() < 2) {
		tmp->m_ready = true;
		m_room.clientList.AddTail(tmp);
		CString str;
		str.Format(_T("Client (%d)"), int(tmp));
		m_room_client_list.InsertString(-1, str);
		if (m_room.clientList.GetCount() == 2) {
			pos = m_room.clientList.GetHeadPosition();
			int count = 0;
			while (pos != NULL) {
				CClientSocket *ptmp = (CClientSocket*)m_room.clientList.GetNext(pos);
				count++;
				if (ptmp != NULL) {
					readyMessage *msg = new readyMessage;
					msg->id = 4;
					msg->size = sizeof(readyRecvMessage);
					if (count == 1)
						msg->data.ready = 1;
					else
						msg->data.ready = 0;
					ptmp->Send((char*)msg, sizeof(readyMessage) * 2);
					CString strTmp;
					strTmp.Format(_T("[%d]:%d"), int(tmp), tmp->m_ready);
					m_list_msg.InsertString(-1, strTmp);
					m_list_msg.SetCurSel(m_list_msg.GetCount() - 1);
				}
			}
		}
	}
	else{
		readyMessage *msg = new readyMessage;
		msg->id = 5;
		msg->size = sizeof(readyRecvMessage);
		msg->data.ready = -1;
		tmp->Send((char *)msg, sizeof(readyMessage) * 2);
	}

	return 0;
}


/******* 추가적으로 어떤 방을 생성하면 될지 알려주기도 함 ***********/
afx_msg LRESULT Cgame_project_serverDlg::OnClientRoomCreate(WPARAM wParam, LPARAM lParam)
{
	CString str;
	Room *room = (Room*)lParam;
	CString kind;
	if (room->kind == 1004) {
		kind.Format(_T("오델로"));
	}
	else if (room->kind == 1005) {
		kind.Format(_T("오목"));
	}
	else if (room->kind == 1006) {
		kind.Format(_T("짝맞추기"));
	}
	str.Format(_T("[%s] %s (%d/2)"), kind, room->name, room->clientList.GetSize());
	room->name = str;

	m_RoomList.AddTail(room);

	m_list_room.InsertString(-1, str);
	m_list_room.SetCurSel(m_list_room.GetCount() - 1);

	/**************** 만들어진 방리스트 클라이언트로 보내기 *************/

	/*********** 보낼 방리스트 만들기 ***********/
	sendRoomInfo* sri = new sendRoomInfo;
	sri->id = 3001;
	sri->size = sizeof(sendRoomInfoStruct);
	_tcscpy(sri->data.roomName, str);
	/********************************************/

	POSITION pos = m_ptrClientSocketList.GetHeadPosition();
	while (pos != NULL) {
		CClientSocket *pClient = 
			(CClientSocket*)m_ptrClientSocketList.GetNext(pos);
		if (pClient != NULL) {
			pClient->Send((char *)sri, sizeof(sendRoomInfo));
		}
	}
	/********************************************************************/

	/*********** 방만든애한테 어떤 방 만들라고 보내주는 곳 **************/
	createRoom* cr = new createRoom;
	cr->id = 5000;
	cr->size = sizeof(createRoomStruct);
	cr->data.kind = room->kind;
	cr->data.roomID = m_RoomList.GetSize()-1;
	_tcscpy_s( cr->data.name , str);

	POSITION createClientPosition = room->clientList.GetHeadPosition();
	while (createClientPosition != NULL) {
		CClientSocket* ccs = (CClientSocket*)room->clientList.GetNext(createClientPosition);
		ccs->Send((char*)cr, sizeof(createRoom));
	}
	delete cr;
	delete sri;
	/********************************************************************/
	return 0;
}
/************************************************************************************/


afx_msg LRESULT Cgame_project_serverDlg::OnClientRoom(WPARAM wParam, LPARAM lParam)
{
	return 0;
}

/****************************** 대기창 OnCreate되면 모든 방목록 보내주는 함수 ****************************/
afx_msg LRESULT Cgame_project_serverDlg::OnClientIsOnCreate(WPARAM wParam, LPARAM lParam) {
	CClientSocket* ccs = (CClientSocket*)lParam;

	POSITION roomListPosition = m_RoomList.GetHeadPosition();

	int roomListSize = m_RoomList.GetSize();
	int countIndex = 0;

	sendAllRoomList* sarl = new sendAllRoomList;
	sarl->id = 3002;
	sarl->size = sizeof(sendAllRoomListStruct);
	sarl->data.roomListSize = roomListSize;
	
	if (ccs != NULL) {
		while (roomListPosition != NULL) {
			Room* sendRoom = (Room*)m_RoomList.GetNext(roomListPosition);
			if (sendRoom != NULL) {
				_tcscpy_s(sarl->data.roomNameList, sendRoom->name);
				ccs->Send((char *)sarl, sizeof(sendAllRoomList));
			}
		}
	}
	delete sarl;

	return 0;
}
/****************************************************************************************************/

afx_msg LRESULT Cgame_project_serverDlg::OnClientRecvRoomPosition(WPARAM wParam, LPARAM lParam) {
	CClientSocket* cs = (CClientSocket*)lParam;

	POSITION roomListPosition = m_RoomList.FindIndex(cs->roomID);
	Room* r = (Room*)m_RoomList.GetAt(roomListPosition);
	if (r != NULL) {
		r->clientList.AddTail(cs);
		createRoom* msg = new createRoom;
		msg->id = 5004;
		msg->size = sizeof(createRoomStruct);
		msg->data.kind = r->kind;
		_tcscpy_s( msg->data.name , r->name);
		msg->data.roomID = cs->roomID;
		cs->Send((char*)msg, sizeof(createRoom));
	}
	/*int countIndex = 0;

	while (roomListPosition != NULL) {
		Room* sendRoom = (Room*)m_RoomList.GetNext(roomListPosition);
		if (sendRoom != NULL) {
			if (countIndex == ars->roomPosition) {
				CString str;
				str.Format(_T("%s"), sendRoom->name);
				AfxMessageBox(str);
			}
		}
		countIndex++;
	}*/

	return 0;
}

//오델로 메세지 서버에 출력하는 함수
afx_msg LRESULT Cgame_project_serverDlg::OnClientOthelloMsg(WPARAM wParam, LPARAM lParam)
{
	LPCTSTR lpData = (LPCTSTR)lParam;

	m_list_msg.InsertString(-1, lpData);
	m_list_msg.SetCurSel(m_list_msg.GetCount() - 1);
	return 0;
}

//오델로 메세지 해당 방 클라이언트에 추가하는 함수
afx_msg LRESULT Cgame_project_serverDlg::OnClientOthelloMsgSend(WPARAM wParam, LPARAM lParam)
{
	othelloMsgStruct *msgStruct = (othelloMsgStruct*)lParam;

	CString str,tmp;
	str.Format(_T("[%s]:%s"), msgStruct->name, msgStruct->msg); // 클라이언트에 메세지 보내기

	POSITION pos = m_RoomList.FindIndex(msgStruct->roomID);

	Room* r = (Room*)m_RoomList.GetAt(pos);
	if (r != NULL) {
		pos = r->clientList.GetHeadPosition();
		while (pos != NULL) {
			CClientSocket* cs = (CClientSocket*)r->clientList.GetNext(pos);
			if (cs != NULL) {
				othelloMsg* msg = new othelloMsg;
				msg->id = 51;
				msg->size = sizeof(othelloMsgStruct);
				_tcscpy_s(msg->data.msg, str);
				cs->Send((char*)msg, sizeof(othelloMsg));
			}
		}
	}

	return 0;
}

/********************************************** 카드게임 관련 함수 *************************************************/
// 카드게임 메세지 서버에 출력하는 함수
afx_msg LRESULT Cgame_project_serverDlg::OnClientCardMsg(WPARAM wParam, LPARAM lParam) {
	LPCTSTR lpData = (LPCTSTR)lParam;

	m_list_msg.InsertString(-1, lpData);
	m_list_msg.SetCurSel(m_list_msg.GetCount() - 1);
	return 0;
}

// 카드게임 메세지 해당 방 클라이언트에 추가하는 함수
afx_msg LRESULT Cgame_project_serverDlg::OnClientCardMsgSend(WPARAM wParam, LPARAM lParam) {
	cardMsgStruct *msgStruct = (cardMsgStruct*)lParam;

	CString str, tmp;
	str.Format(_T("[%s]:%s"), msgStruct->name, msgStruct->msg); // 클라이언트에 메세지 보내기

	POSITION pos = m_RoomList.FindIndex(msgStruct->roomID);

	Room* r = (Room*)m_RoomList.GetAt(pos);
	if (r != NULL) {
		pos = r->clientList.GetHeadPosition();
		while (pos != NULL) {
			CClientSocket* cs = (CClientSocket*)r->clientList.GetNext(pos);
			if (cs != NULL) {
				cardMsg* msg = new cardMsg;
				msg->id = 5001;
				msg->size = sizeof(cardMsgStruct);
				_tcscpy_s(msg->data.msg, str);
				cs->Send((char*)msg, sizeof(cardMsg));
			}
		}
	}

	return 0;
}
/*******************************************************************************************************************/
