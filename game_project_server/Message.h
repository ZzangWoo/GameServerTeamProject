#pragma once
struct msgRecvMessage {
	TCHAR name[50];
	TCHAR str[2000];
};

struct msgMessage {
	int id;
	int size;
	msgRecvMessage data;
};

struct readyRecvMessage {
	int ready;
};

struct readyMessage {
	int id;
	int size;
	readyRecvMessage data;
};

///////////////////////////////////
struct roomInfoRecvMessage {
	TCHAR name[50];
	int kind;
	bool ok;
};

struct roomInfoMessage {
	int id;
	int size;
	roomInfoRecvMessage data;
};
///////////////////////////////////

/*********** 클라이언트로 방 리스트 보낼 구조체 **********/
struct sendRoomInfoStruct {
	//CPtrList roomNameList;
	//CStringList roomNameList;
	TCHAR roomName[100];
};

struct sendRoomInfo {
	int id;
	int size;
	sendRoomInfoStruct data;
};
/********************************************************/

/********************** 처음 접속시 방리스트 불러오기 **************************/
struct firstConnectStruct {
	bool isOnCreate;
};

struct firstConnect {
	int id;
	int size;
	firstConnectStruct data;
};
/*******************************************************************************/

/******************** 대기창 들어온 클라이언트한테만 방리스트 보내기 *******************/
struct sendAllRoomListStruct {
	TCHAR roomNameList[100];
	//TCHAR* roomNameList[100];
	int roomListSize;
};

struct sendAllRoomList {
	int id;
	int size;
	sendAllRoomListStruct data;
};
/***************************************************************************************/

/*********************** 방 목록 클릭하고 참가하기 눌렀을 때 *************************/
struct attendRoomStruct {
	int roomPosition;
};

struct attendRoom {
	int id;
	int size;
	attendRoomStruct data;
};
/*************************************************************************************/

/********************** 방 생성 완료후 클라이언트가 방 만들수 있게 해줌 ******************/
struct createRoomStruct {
	int kind;
	int roomID;
};

struct createRoom {
	int id;
	int size;
	createRoomStruct data;
};
/*****************************************************************************************/