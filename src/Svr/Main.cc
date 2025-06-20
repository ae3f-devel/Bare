#define SERVER 1

#include "./SvrMain.h"
#include <Sock.imp.h>

#include <cstdlib>
#include <new>

#include "./RoomPrivate.h"
#include <ae2f/Atom.h>

#include "./PlConn.h"
#include <Patternise.h>

#include "./Room.imp.h"

#include <thread>

union _SvrUnitIDHandle {
  std::thread td;
  char a;
  inline ~_SvrUnitIDHandle() {}
  constexpr _SvrUnitIDHandle() : a(0) {}
};

ae2f_extern ae2f_SHAREDEXPORT union _SvrUnit SvrUnits[MAX_ROOM_COUNT + 1] = {};
static union _SvrUnitIDHandle SvrTds[MAX_ROOM_COUNT + 1] = {};

ae2f_extern ae2f_SHAREDCALL void SvrUnit(room_t);
ae2f_extern ae2f_SHAREDCALL void SvrRes(room_t);

static unsigned SvrStarted = 0; /** this might have been fucked */

ae2f_extern ae2f_SHAREDCALL void SvrExit();
ae2f_extern ae2f_SHAREDEXPORT unsigned short PORTMAIN = 0;

/**
 * @brief
 * @param port
 * @returns 0 when succeed.
 */
ae2f_extern ae2f_SHAREDEXPORT int SvrMain(unsigned short port) {
  if (SvrStarted)
    return 0;
  else
    SvrStarted = 1;

  PORTMAIN = port;

  {
    ae2f_InetMkData mkdata;
    if (ae2f_InetMk(MAKEWORD(2, 2), &mkdata) != 0) {
      SvrStarted = 0;
      return 1;
    }
  }

  sock_t svrfd = 0;
  uSockAddr svraddr;

  if ((svrfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    dbg_puts("socket has failed");
    close(svrfd);
    SvrStarted = 0;
    return 1;
  }

  __SockAddrMkVerbose(&svraddr.m_in, INADDR_ANY, port);

  int a;
  if ((a = bind(svrfd, &svraddr.m_addr, SockAddrLen))) {
    dbg_puts("bind has failed");
    close(svrfd);
    SvrStarted = 0;
    return (a);
  }

  SvrUnits[MAX_ROOM_COUNT].ID.fd = svrfd;
  new (&SvrTds[MAX_ROOM_COUNT].td) std::thread(SvrRes, MAX_ROOM_COUNT);

  for (globplayer_t i = 0; i < MAX_GLOBAL_PLAYER_COUNT; i++) {
    if (!(i % MAX_ROOM_MEM_COUNT)) {
      room_t j = i / MAX_ROOM_MEM_COUNT;
      SvrUnits[j].ID.fd = INVALID_SOCKET;
      new (&SvrTds[j].td) std::thread(SvrUnit, j);
    }

    Players[i].m_connected = 0;
  }

  return 0;
}

#include "./SvrMain.h"

ae2f_extern ae2f_SHAREDEXPORT void SvrExit() {

  if (!SvrStarted) {
    dbg_puts("Svr has not been started.");
    return;
  }

  close(SvrUnits[MAX_ROOM_COUNT].ID.fd);

  size_t i = MAX_ROOM_COUNT;
  goto LOOPSTART;

  for (; i != -1; i--) {
    {
      RoomFlags[i] = eRoomFlags_KILL;
      ;                                 /* Changing roomflags to stop waiting */
      __ae2f_WakeSingle(RoomFlags + i); /* notify the memory */
      __RoomTerminate(i);
    }

  LOOPSTART:
    SvrUnits[i].ID.fd = INVALID_SOCKET;
    SvrTds[i].td.join();
    SvrTds[i].td.~thread();
  }

  ae2f_InetDel();
}
