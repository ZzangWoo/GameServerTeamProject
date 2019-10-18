#pragma once
#include "Message.h"

// CClikentSocket command target
#define WM_CLIENT_MSG_RECV WM_USER+2
#define WM_CLIENT_CLOSE WM_USER+3
#define WM_CLIENT_MSG_SEND WM_USER+4
#define WM_CLIENT_READY WM_USER+5
#define WM_CLIENT_ROOM_CREATE WM_USER+6
#define WM_CLIENT_ROOM WM_USER+7

#define WM_CLIENT_IS_ON_CREATE WM_USER+3002

#define WM_CLIENT_RECV_ROOM_POSITION WM_USER+4000
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
};

class Room {
public:
	int kind;
	CString name;
	CObList clientList;
	Room() {

	}
	~Room() {

	}
};