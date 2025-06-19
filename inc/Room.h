#ifndef Room_h
#define Room_h

#include <ae2f/Call.h>
#include <ae2f/Cast.h>

#include "./Max.auto.h"
#include "./Sock.h"

#include <ae2f/Pack/Beg.h>
#include <time.h>
#include "./Room.auto.h"

#include <ae2f/Lock.h>
#include "./Player.h"

#pragma pack(push, 1)

/**
 * @brief
 * Global settings or something.
 */
typedef struct Room {
  char m_Name[MAX_ROOM_NAME_COUNT];
  unsigned m_started : 8;
  player_t m_member;
} Room;

#pragma pack(pop)

#include "./Req.h"

#include <ae2f/Pack/End.h>

typedef struct __ReqRoomLobbyBuf __ReqRoomLobbyBuf; 
typedef struct __ReqRoomShowBuf __ReqRoomShowBuf;

#if SERVER
ae2f_extern ae2f_SHAREDCALL void RoomLobby(room_t room, globplayer_t* retgplidx,
                                           const char *roomname, const char *pw,
                                           sock_t clientsocket,
                                           const uSockAddr *clientaddr,
                                           const char *clientname);


ae2f_extern ae2f_SHAREDCALL void ResRoomLobby(sock_t clientsocket,
                                           const uSockAddr *clientaddr, __ReqRoomLobbyBuf* req);

ae2f_extern ae2f_SHAREDCALL void ResRoomShow(sock_t clisock, const uSockAddr* cliaddr, __ReqRoomShowBuf* req);
#else
#endif

ae2f_extern ae2f_SHAREDCALL void ReqRoomLobby(sock_t svrsock,
                                              const uSockAddr *svraddr,
                                              room_t room, room_t *retroom,
                                              const char *roomname, const char *pw, const char* playername);

ae2f_extern ae2f_SHAREDCALL void ReqRoomShow(
    sock_t svrsock, const uSockAddr* svraddr
    , room_t roompad, room_t roomcount, Room* retroom, room_t* retcount
);


#pragma pack(push, 1)

/** Request type utility for networking */
struct __ReqRoomLobbyBuf {
  req_t m_req; /** Request type for server */
  room_t m_room; /** Room ID  */
  char m_plname[MAX_PLAYER_NAME_COUNT]; /** Player name */
  char m_rname[MAX_ROOM_NAME_COUNT];    /** Room Name (when you are making new) */
  char m_rpwd[MAX_ROOM_PW];             /** Password */
};

struct __ReqRoomShowBuf {
  req_t m_req;
  room_t pad;
  room_t count;
};

#pragma pack(pop)

#endif
