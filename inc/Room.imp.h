#include "./Req.h"
#include "./Room.h"
#include "./Util.h"

#include <ae2f/Cast.h>

#include <stddef.h>
#include <string.h>

#define __GetPlayerIndex(roomid, playerid)                                     \
  ((((roomid)) * MAX_ROOM_PLAYER_COUNT) + (playerid))

#define __GetRoomIndex(globplayerid) ((globplayerid) / MAX_ROOM_PLAYER_COUNT)

/**
 * @details
 * v_req:
 *  global:
 *    [req_t]                      : as request  \n
 *    [room_t]                     : room number \n
 *    [char[MAX_PLAYER_NAME_COUNT]]: player name \n
 *  additional when making new one [room number is -1]: \n
 *    [char[MAX_ROOM_NAME_COUNT]]  : room name   \n
 *    [char[MAX_ROOM_PW]]          : room passwd \n
 *
 * v_res:
 *  [room_t]
 */
#define __ReqRoomLobby(svrsock, svraddr, room, retgplidx, name /*opt*/,        \
                       pw /*opt*/, clientname /*opt*/)                         \
  {                                                                            \
    if (!((svraddr) && (retgplidx)))                                           \
      ;                                                                        \
    if ((!(svraddr) || (svrsock) == INVALID_SOCKET)) {                         \
      when_SERVER(                                                             \
          RoomLobby(room, retgplidx, name, pw, svrsock, svraddr, clientname)); \
      when_CLIENT(*(retgplidx) = -1);                                          \
    } else {                                                                   \
      __ReqRoomLobbyBuf v_req = {                                              \
          0,                                                                   \
      };                                                                       \
      size_t v_bytes;                                                          \
                                                                               \
      v_req.m_req = REQ_ROOMLOBBY;                                             \
      v_req.m_room = room;                                                     \
      if (clientname)                                                          \
        strncpy(v_req.m_plname, clientname, MAX_PLAYER_NAME_COUNT);            \
      if ((room) == -1) {                                                      \
        if (name)                                                              \
          strncpy(v_req.m_rname, (name), MAX_ROOM_NAME_COUNT);                 \
        if (pw)                                                                \
          strncpy(v_req.m_rpwd, (pw), MAX_ROOM_PW);                            \
      }                                                                        \
      v_bytes = sendto((svrsock), (const void *)&v_req, sizeof(v_req), 0,      \
                       &(svraddr)->m_addr, SockAddrLen);                       \
                                                                               \
      if (v_bytes != sizeof(v_req)) {                                          \
        (*(retgplidx) = -1);                                                   \
      }                                                                        \
                                                                               \
      else {                                                                   \
        uSockAddr v_from[1];                                                   \
        socklen_t v_fromlen[1] = {SockAddrLen};                                \
        if (recvfrom((svrsock), (void *)(retgplidx), sizeof(room_t), 0,        \
                     &v_from->m_addr, v_fromlen) != sizeof(room_t)) {          \
          (*(retgplidx) = -1);                                                 \
        }                                                                      \
        if (!uSockAddrInCheck(v_from, (svraddr))) {                            \
          (*(retgplidx) = -1); /* invalid, socket address does not match. */   \
        }                                                                      \
      }                                                                        \
    }                                                                          \
  }

#define __IsRoomCustom(r) !((r)->m_Name[0])
#define __IsRoomPrivate(r) ((r)->m_Pw[0])
#define __IsRoomOnGame(r) ((r)->m_started)
