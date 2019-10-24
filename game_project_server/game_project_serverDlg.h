
// game_project_serverDlg.h : header file
//

#pragma once
#include "ServerSocket.h"
#include "afxwin.h"

// Cgame_project_serverDlg dialog
class Cgame_project_serverDlg : public CDialogEx
{
// Construction
public:
	Cgame_project_serverDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GAME_PROJECT_SERVER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	CClientSocket *m_clientSocket;
	CServerSocket *m_serverSocket;

public:
	CListBox m_list_client;
	CListBox m_list_msg;
	CPtrList m_ptrClientSocketList;
	CPtrList m_RoomList;
	Room m_room;
protected:
	afx_msg LRESULT OnAcceptSocket(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnClientMsgRecv(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnClientClose(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnClientMsgSend(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnClientReady(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnClientIsOnCreate(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnClientRecvRoomPosition(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnClientCardIsReady(WPARAM wParam, LPARAM lParam);
public:
	CListBox m_room_client_list;
protected:
	afx_msg LRESULT OnClientRoomCreate(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnClientRoom(WPARAM wParam, LPARAM lParam);
public:
	CListBox m_list_room;
protected:
	afx_msg LRESULT OnClientOthelloMsg(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnClientOthelloMsgSend(WPARAM wParam, LPARAM lParam);
	
	afx_msg LRESULT OnClientCardMsg(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnClientCardMsgSend(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnClientGameClose(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnClientRockChoice(WPARAM wParam, LPARAM lParam);
	int* CompareResult(int player1, int player2);
	afx_msg LRESULT OnClientFindIndex(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnClientSendMap(WPARAM wParam, LPARAM lParam);
};


