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
	int roomID;
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
	TCHAR ClientName[50];

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
	int count;
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
	TCHAR ClientName[50];

};

struct attendRoom {
	int id;
	int size;
	attendRoomStruct data;
};
/*************************************************************************************/

//방참가할 때 게임 종류 받기 
/********************** 방 생성 완료후 클라이언트가 방 만들수 있게 해줌 ******************/
struct createRoomStruct {
	int kind;
	int roomID;
	int roomKind;
	TCHAR name[50];
};

struct createRoom {
	int id;
	int size;
	createRoomStruct data;
};
/*****************************************************************************************/
// 방 리스트 삭제 후 게임방에 들어가있는 클라이언트한테	보내는 구조체
struct sendRoomIDStruct {
	int roomID;
	int roomKind;
};

struct sendRoomID {
	int id;
	int size;
	sendRoomIDStruct data;
};

// 룸 정보


// 오델로 채팅 메세지
struct othelloMsgStruct {
	TCHAR msg[2000];
	int roomID;
	TCHAR name[50];
};

struct othelloMsg {
	int id;
	int size;
	othelloMsgStruct data;
};



/************************** 짝맞추기에 필요한 struct ***********************/
// 카드게임 채팅 메세지
struct cardMsgStruct {
	TCHAR msg[2000];
	int roomID;
	TCHAR name[50];
};

struct cardMsg {
	int id;
	int size;
	cardMsgStruct data;
};

// 게임시작 버튼에 대한 요청
struct cardReadyStruct {
	int roomID;
	bool isReady;
};

struct cardReady {
	int id;
	int size;
	cardReadyStruct data;
};

// 게임시작하라는 요청
struct cardStartStruct {
	bool start;
};

struct cardStart {
	int id;
	int size;
	cardStartStruct data;
};
/**********************************************************************/

struct playerStruct {
	TCHAR name[50];
};

struct playerMessage {
	int id;
	int size;
	playerStruct data;
};

struct choiceStruct {
	int choice;
	int roomID;
};
struct choiceMessage {
	int id;
	int size;
	choiceStruct data;
};

//오델로 맵 받고 주기
struct mapStruct {
	int clientID;
	int roomID;
	int map[8][8];
};
struct mapmessage {
	int id;
	int size;
	mapStruct data;
};