#pragma once
#include "Message.h"

// CClikentSocket command target
#define WM_CLIENT_MSG_RECV WM_USER+2
#define WM_CLIENT_CLOSE WM_USER+3
#define WM_CLIENT_MSG_SEND WM_USER+4
#define WM_CLIENT_READY WM_USER+5
#define WM_CLIENT_ROOM_CREATE WM_USER+6
#define WM_CLIENT_ROOM WM_USER+7

#define WM_CLIENT_FIND_INDEX WM_USER+11
#define WM_CLIENT_SEND_MAP WM_USER+12
#define WM_CLIENT_ROCK_CHOICE WM_USER+402

#define WM_CLIENT_OTHELLO_MSG WM_USER+50
#define WM_CLIENT_OTHELLO_MSG_SEND WM_USER+51

#define WM_CLIENT_IS_ON_CREATE WM_USER+3002

#define WM_CLIENT_RECV_ROOM_POSITION WM_USER+4000

#define WM_CLIENT_CARD_MSG WM_USER+5000
#define WM_CLIENT_CARD_MSG_SEND WM_USER+5001

#define WM_CLIENT_GAME_CLOSE WM_USER+5005

#define WM_CLIENT_CARD_IS_READY WM_USER+5400

class CClientSocket : public CSocket
{
public:
	CClientSocket();
	virtual ~CClientSocket();
	void SetWnd(HWND hWnd);
	HWND m_hWnd;
	virtual void OnClose(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	CString m_IPAddress;
	UINT m_uPortNum;
	bool m_ready;
	int roomID;
	int roomKind;
	CString nickName;
	int rsp_choice;
};

class Room {
public:
	int kind;
	CString name;
	CObList clientList;

	/********** 짝맞추기에 필요한 변수 ***********/
	int card_isReady = 0; // 2 : 모두 준비 (가위바위보 출력)
	/*********************************************/

	Room() {

	}
	~Room() {

	}
};
