#define SERVER 1
#include "./Room.imp.h"
#include "./RoomPrivate.h"
#include "./SvrMain.h"

#include <Dbg.h>
#include <Req.h>
#include <Room.h>
#include <Sock.imp.h>

#if _WIN32

#define nonblock(sock, res)                                                    \
  {                                                                            \
    u_long mode = 1;                                                           \
    if (ioctlsocket(sock, FIONBIO, &mode) != 0) {                              \
      dbg_printf("ioctlsocket failed with error: %d\n", WSAGetLastError());    \
      *(res) = -1;                                                             \
    }                                                                          \
    *(res) = 0;                                                                \
  }

#define errno WSAGetLastError()
#define EWOULDBLOCK WSAEWOULDBLOCK
#define EAGAIN WSAEWOULDBLOCK

#else
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

#define nonblock(fd, res)                                                      \
  if (res) {                                                                   \
    int v_flags;                                                               \
    v_flags = fcntl(fd, F_GETFL, 0);                                           \
                                                                               \
    if (v_flags == -1) {                                                       \
      dbg_puts("fcntl F_GETFL");                                               \
      *(res) = -1;                                                             \
    } else {                                                                   \
                                                                               \
      /* Add non-blocking flag */                                              \
      v_flags |= O_NONBLOCK;                                                   \
      if (fcntl(fd, F_SETFL, v_flags) == -1) {                                 \
        perror("fcntl F_SETFL");                                               \
        *(res) = -1;                                                           \
      } else                                                                   \
        *(res) = 0;                                                            \
    }                                                                          \
  }

#endif

#undef dbg_prefix
#define dbg_prefix "[SvrUnit] "

ae2f_extern ae2f_SHAREDEXPORT void SvrUnit(room_t a) {
  dbg_printf("thread %p has started\n", a);
  Rooms[a].m_started = 0;
  RoomFlags[a] = eRoomFlags_PAUSED;
  Rooms[a].m_Name[0] = 0;
  Rooms[a].m_member = 0;

  socklen_t v_addrlen;
  ssize_t v_succeed;
  ReqBuff v_reqbuff;
  uSockAddr v_addr;

__START:
  __ae2f_Wait(&RoomFlags[a], eRoomFlags_PAUSED);

  switch (RoomFlags[a]) {
  case eRoomFlags_PAUSED:
    goto __START;
  case eRoomFlags_KILL:
    goto __QUIT;
  case eRoomFlags_RUNNING:
    break;
  }

  if (SvrUnits[a].ID.fd == INVALID_SOCKET) {
    dbg_puts("Socket is invalid.\n"
             "Back to suspend...");

    __RoomTerminate(a);
    RoomFlags[a] = eRoomFlags_PAUSED;
    goto __START;
  }

  v_addrlen = SockAddrLen;

  v_succeed = recvfrom(SvrUnits[a].ID.fd, (void *)&v_reqbuff,
                       sizeof(Svr.m_reqbuff), 0, &v_addr.m_addr, &v_addrlen);

  if (v_succeed < sizeof(req_t) || v_succeed > sizeof(Svr.m_reqbuff)) {
    if (errno == EWOULDBLOCK || errno == EAGAIN) {
      goto __START;
    } else {
      dbg_printf("recvfrom error: %d\n", errno);
    }
    goto __START;
  }

  goto __START;
__QUIT:
  __RoomTerminate(a);
  dbg_printf("thread %p is over\n", a);
}

#undef dbg_prefix
#define dbg_prefix "[SvrRes] "

ae2f_extern ae2f_SHAREDEXPORT _Svr Svr = {
    0,
};

ae2f_extern ae2f_SHAREDEXPORT void SvrRes(room_t a) {
  RoomFlags[a] = eRoomFlags_RUNNING;
  {
    int res;
    nonblock(SvrUnits[a].ID.fd, &res);
  }

  Svr.m_sock = SvrUnits[a].ID.fd;
  dbg_printf("Socket: %d\n", SvrUnits[a].ID.fd);

  while (SvrUnits[a].ID.fd != INVALID_SOCKET) {
    Svr.m_addrlen = SockAddrLen;

    Svr.m_succeed =
        recvfrom(Svr.m_sock, (void *)&Svr.m_reqbuff, sizeof(Svr.m_reqbuff), 0,
                 &Svr.m_addr->m_addr, &Svr.m_addrlen);

    if (!RoomFlags[MAX_ROOM_COUNT]) {
      RoomFlags[MAX_ROOM_COUNT] = 1;
    }

    if (Svr.m_succeed < sizeof(req_t) ||
        Svr.m_succeed > sizeof(Svr.m_reqbuff)) {
      if (errno == EWOULDBLOCK || errno == EAGAIN) {
        continue;
      } else if (SvrUnits[a].ID.fd == INVALID_SOCKET) {
        break;
      }
      continue;
    }

    dbg_printf("Addrlen: %d\n", Svr.m_addrlen);

    if (Svr.m_succeed >= sizeof(req_t)) {
      dbg_printf("Connect from somewhere. Req: %d\n", Svr.m_reqbuff.m_req);
    }
    switch (Svr.m_reqbuff.m_req) {
    case REQ_ROOMLOBBY:
      if (Svr.m_succeed != sizeof(Svr.m_reqbuff.m_ReqRoomLobby))
        continue;
      ;
      ResRoomLobby(Svr.m_sock, Svr.m_addr, &Svr.m_reqbuff.m_ReqRoomLobby);
      break;
    case REQ_ROOMSHOW:
      if (Svr.m_succeed != sizeof(Svr.m_reqbuff.m_ReqRoomShow))
        continue;
      ;
      ResRoomShow(Svr.m_sock, Svr.m_addr, &Svr.m_reqbuff.m_ReqRoomShow);
      break;
    }
  }

  dbg_printf("thread %p is over\n", a);
}
