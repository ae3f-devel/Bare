#include "./PlConn.h"
#include "./Room.imp.h"
#include "./Sock.imp.h"
#include <Dbg.h>

#if ae2f_IS_WIN
#include <../inc/Player.imp.h>
#else 
#include_next <Player.imp.h>
#endif

#include <ae2f/Cast.h>
#include <string.h>
#define __IsPlayerNull(p) (!Players[(p)].m_connected)
#define __PlayerInit(p) (Players[p] = ae2f_RecordMk(Player, 0, 0, 0, ))

#define __SetPlayerOffline(addr, retidx)                                       \
  for (*(retidx) = 0; *(retidx) < MAX_GLOBAL_PLAYER_COUNT; (*(retidx))++) {    \
    if (uSockAddrInCheckNPort((addr), (&PlConns[*(retidx)].m_addr))) {         \
      memset(&PlConns[*(retidx)].m_addr, 0,                                    \
             sizeof(PlConns[*(retidx)].m_addr));                               \
      __PlayerInit(*(retidx));                                                 \
      Players[*(retidx)].m_connected = 0;                                      \
      if (!(--Rooms[(*(retidx)) / MAX_ROOM_MEM_COUNT].m_member)) {             \
        __RoomTerminate((*(retidx)) / MAX_ROOM_MEM_COUNT);                     \
      }                                                                        \
      {                                                                        \
        dbg_printf("Got someone offline: %d\n", (*retidx));                    \
        break;                                                                 \
      }                                                                        \
    }                                                                          \
  }                                                                            \
  __uSockAddrPrint(addr);                                                      \
  __uSockAddrPrint(&PlConns[0].m_addr);                                        \
  __uSockAddrPrint(&PlConns[1].m_addr);

#define __SetPlayerOnline(addr, globidx)                                       \
  dbg_puts("__SetPlayerOnline");                                               \
  if ((addr) && __IsPlayerNull((globidx))) {                                   \
    dbg_puts("__SetPlayerOnline: ON");                                         \
    PlConns[globidx].m_addr = *(addr);                                         \
    Players[globidx].m_connected = 1;                                          \
    if (!(Rooms[(globidx) / MAX_ROOM_MEM_COUNT].m_member++)) {                 \
      __RoomActivate((globidx) / MAX_ROOM_MEM_COUNT);                          \
    }                                                                          \
  }
