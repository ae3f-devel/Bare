#ifndef SvrMain_h
#define SvrMain_h

#include <Max.auto.h>
#include <Room.h>
#include <ae2f/Atom.h>
#include <ae2f/Call.h>
#include <ae2f/Cast.h>

ae2f_extern ae2f_SHAREDCALL union _SvrUnit {
  struct _SvrUnitID {
    ae2f_Atom(sock_t) fd;
#ifdef __cplusplus
    constexpr _SvrUnitID() : fd(INVALID_SOCKET) {}
    inline ~_SvrUnitID() {}
#endif
  } ID;

  struct _SvrUnitGame {
    struct _SvrUnitID _;
  } Game;
#ifdef __cplusplus
  constexpr _SvrUnit() : ID() {}
  inline ~_SvrUnit() {}
#endif
} SvrUnits[MAX_ROOM_COUNT + 1];

#include "./ReqBuff.h"

typedef struct _Svr {
  ae2f_InetMkData m_mkdata;
  sock_t m_sock;
  ReqBuff m_reqbuff;
  uSockAddr m_addr[1];
  socklen_t m_addrlen;
  ssize_t m_succeed;
} _Svr;

#include "./RoomFlags.h"

ae2f_extern ae2f_SHAREDCALL _Svr Svr;

#endif
