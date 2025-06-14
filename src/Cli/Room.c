#include "./Room.imp.h"
#include <Dbg.h>

#define dbg_prefix "[ReqRoomLobby] "

ae2f_SHAREDEXPORT void ReqRoomLobby(sock_t svrsock, const uSockAddr *svraddr,
                                    room_t room, globplayer_t *retgplidx,
                                    const char *name, const char *pw,
                                    const char *clientname) {
  dbg_puts("Starting.");
  assertmsg((svrsock == -1) == SERVER);
  assertmsg(!svraddr == SERVER); /*?*/
  __ReqRoomLobby(svrsock, svraddr, room, retgplidx, name, pw, clientname);
  dbg_puts("done gracully.");
}

#define dbg_prefix "[ReqRoomShow] "

ae2f_SHAREDEXPORT void ReqRoomShow(sock_t svrsock, const uSockAddr *svraddr,
                                   room_t roompad, room_t roomcount,
                                   Room *retroom, room_t *retcount) {
  if (!((svraddr) && (retroom) && (retcount)))
    ;
  else if (roompad + roomcount > MAX_ROOM_COUNT) {
    dbg_puts("Too many: it exceeed the max");
    *(retcount) = 0;
  } else {
    uSockAddr v_svraddr;
    __ReqRoomShowBuf v_showbuf;

    v_showbuf.m_req = REQ_ROOMSHOW;
    v_showbuf.count = roomcount;
    v_showbuf.pad = roompad;

    if (sendto(svrsock, (void *)&v_showbuf, sizeof(v_showbuf), 0,
               &svraddr->m_addr, SockAddrLen) != sizeof(__ReqRoomShowBuf)) {
      dbg_puts("Sendto has failed.");
      *(retcount) = 0;
    } else {
      socklen_t v_addrlen = SockAddrLen;
      *(retcount) = recvfrom(svrsock, (void *)retroom, sizeof(Room) * roomcount,
                             0, &v_svraddr.m_addr, &v_addrlen);

#if 1
      if (!uSockAddrInCheck(&v_svraddr, svraddr)) {
        dbg_puts("Target address did not match.");
        *(retcount) = 0;
      } else if (*(retcount) > MAX_GLOBAL_PLAYER_COUNT * sizeof(Room)) {
        dbg_puts("Overflow for some reason.");
        *(retcount) = 0;
      }
      if (*(retcount) > 0) {
        *(retcount) /= sizeof(Room);
      }
#endif
    }
  }
}

#define dbg_prefix
