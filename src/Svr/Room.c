#include "./Room.imp.h"
#include "./RoomPrivate.h"
#include <Dbg.h>

#define dbg_prefix "[RoomLobby] "

ae2f_SHAREDEXPORT void RoomLobby(room_t room, room_t *retroom,
                                 const char *name_room, const char *pw,
                                 sock_t sock, const uSockAddr *addr,
                                 const char *name_member) {
  __RoomLobby(room, retroom, name_room, pw, sock, addr, name_member);
}

#include <errno.h>

#define dbg_prefix "[ResRoomLobby] "
ae2f_SHAREDEXPORT void ResRoomLobby(sock_t sock, const uSockAddr *addr,
                                    __ReqRoomLobbyBuf *req) {
  if (req && req->m_req == REQ_ROOMLOBBY && addr && sock != INVALID_SOCKET) {
    globplayer_t v_retroom = -1;
    __RoomLobby(req->m_room, &v_retroom, req->m_rname, req->m_rpwd, sock, addr,
                req->m_plname);
    ssize_t d;
    if (sizeof(globplayer_t) ==
        (d = sendto(sock, (const void *)&v_retroom, sizeof(v_retroom), 0,
                    &addr->m_addr, SockAddrLen))) {
      dbg_puts("Good");
    } else {
      dbg_printf("Bad %d\n", d);
      dbg_printf("Err: %d\n", errno);
    }
  }
}

#define dbg_prefix "[ResRoomShow] "

ae2f_SHAREDCALL void ResRoomShow(sock_t clisock, const uSockAddr *cliaddr,
                                 __ReqRoomShowBuf *req) {
  if ((req) && (cliaddr) && clisock != INVALID_SOCKET) {
    dbg_printf("pad %d + count %d = %d\n", req->pad, req->count,
               req->count + req->pad);
    if (req->pad + req->count <= MAX_ROOM_COUNT) {
      dbg_puts("Got. sending room informations.");
      ssize_t a =
          sendto(clisock, ae2f_reinterpret_cast(const void *, Rooms + req->pad),
                 sizeof(Room) * req->count, 0, &cliaddr->m_addr, SockAddrLen);
      dbg_printf("sendto: %d\n", a);
    }
  }
}
