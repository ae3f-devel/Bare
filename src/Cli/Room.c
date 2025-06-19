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

    ssize_t v_suc;
    {
      time_t t = time(0);
      while ((v_suc = sendto(svrsock, (void *)&v_showbuf, sizeof(v_showbuf), 0,
                             &svraddr->m_addr, SockAddrLen)) !=
             sizeof(__ReqRoomShowBuf)) {
        if ((errno == EWOULDBLOCK || errno == EAGAIN) && !TIMEOUT(t)) {
          continue;
        } else {
          break;
        }
      }
    }

    if (v_suc != sizeof(__ReqRoomShowBuf)) {
      dbg_puts("Sendto has failed.");
      *(retcount) = 0;
    } else {
      socklen_t v_addrlen = SockAddrLen;

      {
        time_t verifier = -1, t = time(0);

        while ((v_suc =
                    recvfrom(svrsock, (void *)retroom, sizeof(Room) * roomcount,
                             0, &v_svraddr.m_addr, &v_addrlen)) < 0) {

          if ((errno == EWOULDBLOCK || errno == EAGAIN)) {
            if(((time(&verifier) - (t)) > (timeout))) {
              if(verifier == -1) {
                dbg_puts("Error: time function is somehow failing.");
              } else {
                dbg_puts("time function is okay.");
              }
              dbg_puts("Cancelling due to the timeout.");
              dbg_printf("%d %d\n", ELAPSED(t), timeout);
              break;
            }
            continue;
          } else {
            dbg_puts("Got some error not normal.");
            break;
          }
        }
      }

#if 1
      if (v_suc < 0) {
        dbg_puts("recvfrom has failed.");
        *(retcount) = 0;
      } else if (v_suc > MAX_GLOBAL_PLAYER_COUNT * sizeof(Room)) {
        dbg_puts("We got some overflows.");
        *(retcount) = 0;
      } 
      else if (v_addrlen < SockAddrLen) {
        dbg_puts("Socket address length is somehow lower than expected.");
        *(retcount) = 0;
      }
      else if (!uSockAddrInCheck(&v_svraddr, svraddr)) {
        dbg_puts("Target address did not match.");
        *(retcount) = 0;
      } else if (v_suc > MAX_GLOBAL_PLAYER_COUNT * sizeof(Room) || v_suc < 0) {
        dbg_puts("Overflow for some reason.");
        *(retcount) = 0;
      }
      
      if (*(retcount) > 0) {
        *(retcount) = v_suc / sizeof(Room);
        dbg_puts("Test printing m_Name from low level");
        when_DEBUG(TestLog(retroom[0].m_Name));
      }
#endif
      dbg_puts("End");
    }
  }
}

#define dbg_prefix
