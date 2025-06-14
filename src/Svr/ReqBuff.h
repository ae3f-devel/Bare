#ifndef ReqBuff_h
#define ReqBuff_h

#include <Req.h>
#include <Room.h>

typedef struct __ReqGameStartBuf {
    req_t m_req;
    room_t m_room;
} __ReqGameStartBuf;

typedef union ReqBuff {
    req_t m_req;
    __ReqRoomLobbyBuf m_ReqRoomLobby;
    __ReqRoomShowBuf m_ReqRoomShow;
    __ReqGameStartBuf m_ReqGameStart;
} ReqBuff;

#endif