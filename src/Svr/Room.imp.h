#ifndef Room_imp_h
#define Room_imp_h

#include "./RoomFlags.h"
#include "./RoomPrivate.h"

#if ae2f_IS_WIN
#include <../inc/Player.h>
#else
#include_next <Player.h>
#endif

#include "./PlConn.h"
#include "./RoomPrivate.h"
#include <Dbg.h>
#include <Sock.imp.h>

#define __IsPlayerNull(p) (!Players[(p)].m_connected)
#define __PlayerInit(p) (Players[p] = ae2f_RecordMk(Player, 0, 0, 0, ))

inline static void _SetPlayerOffline(const uSockAddr *const addr,
                                     globplayer_t *const retidx);

inline static void _SetPlayerOnline(const uSockAddr *const addr,
                                    const globplayer_t globidx);

#define __SetPlayerOnline _SetPlayerOnline
#define __SetPlayerOffline _SetPlayerOffline

#if ae2f_IS_WIN
#include <../inc/Room.imp.h>
#else
#include_next <Room.imp.h>
#endif

ae2f_extern ae2f_SHAREDCALL unsigned short PORTMAIN;

#define __RoomInit(room)                                                       \
  {                                                                            \
    Rooms[(room)].m_Name[0] = 0;                                               \
    Rooms[(room)].m_started = 0;                                               \
  }

/** When ret_i is MAX, it is blank */
ae2f_MAC() _IsRoomNOccupied(const room_t r, player_t *const ret_i) {
  if (ret_i)
    *(ret_i) = (MAX_ROOM_MEM_COUNT - Rooms[r].m_member);
}

#define __IsRoomNOccupied _IsRoomNOccupied

ae2f_MAC() _IsRoomNFull(const room_t r, player_t *const ret_i,
                        const uSockAddr *const addr) {
  if (ret_i)
    for (*(ret_i) = 0; *(ret_i) < MAX_ROOM_MEM_COUNT; (*(ret_i))++) {
      if (__IsPlayerNull((((r)) * MAX_ROOM_MEM_COUNT) + *(ret_i)))
        break;
      if (uSockAddrInCheck(
              addr,
              (&(PlConns + (((r)) * MAX_ROOM_MEM_COUNT) + *(ret_i))->m_addr)))
        break;
    }
}

#define __IsRoomNFull _IsRoomNFull

ae2f_MAC() _FindRoomNOccupied(room_t *const reti) {
  if (reti) {
    for (*(reti) = 0; *(reti) < MAX_ROOM_COUNT; (*(reti))++) {
      player_t v_i;
      v_i = 0;
      __IsRoomNOccupied(*(reti), &v_i);
      dbg_printf("MEMCOUNT from idroomnoccupied: %d\n",
                 v_i); /* world is blank. this is what we wanted. */
      if (v_i == MAX_ROOM_MEM_COUNT)
        break;
    }
  }
}

#define __FindRoomNOccupied _FindRoomNOccupied

ae2f_MAC() _FindRoomMatchAvailable(room_t *const reti, player_t *pl) {
  for (*(reti) = 0; *(reti) < MAX_ROOM_COUNT; (*(reti))++) {
    if (Rooms[*(reti)].m_Name[0])
      continue;
    for (*(pl) = 0; *(pl) < MAX_ROOM_PLAYER_COUNT; (*(pl))++) {
      if (__IsPlayerNull((*(reti)) * MAX_ROOM_MEM_COUNT + (*(pl))))
        break;
    }
  }
}

#define __FindRoomMatchAvailable _FindRoomMatchAvailable

ae2f_MAC() _RoomLobby(const room_t room, globplayer_t *const retgplidx,
                      const char *const name_room, const char *const pw,
                      const sock_t sock, const uSockAddr *const addr,
                      const char *const name_member) {
  assertmsg(retgplidx);
  assertmsg(addr);
  dbg_puts("Starting.");

  if (!((retgplidx) && (addr)))
    ;
  else {
    globplayer_t v_globplayer_i = 0;
    __SetPlayerOffline(addr, &v_globplayer_i);
    if (v_globplayer_i != MAX_GLOBAL_PLAYER_COUNT) {
      dbg_puts("you are online.");
      dbg_printf("You are online here -> %d\n", v_globplayer_i);
      dbg_puts("Now you are being disconnected.");
      *(retgplidx) = -1;
    }
    dbg_printf("off %d\n", v_globplayer_i);
    if ((room) == -1 &&
        v_globplayer_i == MAX_GLOBAL_PLAYER_COUNT) { /** Making new room... */
      room_t reti;
      globplayer_t pl = 0;
      if ((name_room) && *(name_room)) {
        __FindRoomNOccupied((&reti));
      } else {
        __FindRoomMatchAvailable(&reti, &pl);
      }
      dbg_printf("reti: %d pl: %d\n", reti, pl);

      if (reti == MAX_ROOM_COUNT) {
        dbg_puts("No room is available.");
        *(retgplidx) = -1;
      } else {
        dbg_printf("Room %d will be allocated.\n", reti);
        *(retgplidx) = (reti)*MAX_ROOM_MEM_COUNT + pl;
        if (!pl) {
          if (name_room)
            strncpy(Rooms[reti].m_Name, name_room, MAX_ROOM_NAME_COUNT);
          else
            Rooms[reti].m_Name[0] = 0;

          if (pw)
            strncpy(RoomPrivates[reti].m_Pw, pw, MAX_ROOM_PW);
          else
            RoomPrivates[reti].m_Pw[0] = 0;
        }

        if (name_member)
          strncpy(Players[reti * MAX_ROOM_MEM_COUNT + (pl)].m_Name, name_member,
                  MAX_PLAYER_NAME_COUNT);
        else
          Players[reti * MAX_ROOM_MEM_COUNT + pl].m_Name[0] = 0;

        __SetPlayerOnline(addr, reti * MAX_ROOM_MEM_COUNT + pl);
      }
    }
    if ((room) != -1) { /** Connected to existing... */
      if (Rooms[room].m_started) {
        dbg_printf("This room[%d], is already started.\n", room);
        *(retgplidx) = -1;
      } else {
        player_t reti;
        __IsRoomNOccupied((room), &reti);

        if (reti == MAX_ROOM_MEM_COUNT) {
          dbg_printf("Unexpected room number %d\n", (room));
          *(retgplidx) = -1;
        } else {
          dbg_printf("The room %d is occupied by someone.\n", (room));
          __IsRoomNFull((room), &reti, addr);
          if (reti != MAX_ROOM_MEM_COUNT) {
            dbg_printf("The room %d is valid.\n", (room));
            if (((pw) && !strncmp(RoomPrivates[(room)].m_Pw, (pw),
                                  MAX_ROOM_PW)) /* pw matches. */
                || !RoomPrivates[(room)].m_Pw[0] /*Pwd of room is null.*/) {
              dbg_puts("Password matches. You may come in.");
              *(retgplidx) = (room)*MAX_ROOM_MEM_COUNT + (reti);
              __SetPlayerOnline(addr, reti + (room)*MAX_ROOM_MEM_COUNT);
              if (name_member)
                strncpy(Players[reti + (room)*MAX_ROOM_MEM_COUNT].m_Name,
                        (name_member), MAX_PLAYER_NAME_COUNT);
              else
                Players[reti + (room)*MAX_ROOM_MEM_COUNT].m_Name[0] = 0;
            } else {
              dbg_puts("Password does not matches.");
              *(retgplidx) = -1;
            }
          } else {
            dbg_printf("The room %d is full. \n", (room));
            *(retgplidx) = -1;
          }
        }
      }
    }
  }
  dbg_puts("Done gracully.");
}

#define __RoomLobby _RoomLobby

#include "./SvrMain.h"
#include <ae2f/Block.h>

ae2f_MAC() _RoomTerminate(room_t room) {
  __RoomInit(room);

  sock_t v_sock = SvrUnits[(room)].ID.fd;
  SvrUnits[(room)].ID.fd = INVALID_SOCKET;
  if (v_sock != INVALID_SOCKET)
    close(v_sock);
}

#define __RoomTerminate _RoomTerminate

ae2f_MAC() _RoomActivate(const room_t room) {
  sock_t v_sock = socket(AF_INET, SOCK_DGRAM, 0);
  uSockAddr v_addr;
  v_addr = Svr.m_addr[0];
  v_addr.m_in.sin_port = PORTMAIN + room + 1;
  dbg_printf("New Port: %d\n", v_addr.m_in.sin_port);
  if (v_sock < 0) {
    dbg_puts("socket failed.");
    close(v_sock);
  }

  int v_res;
  ae2f_InetNBlock(v_sock, &v_res);
  if (v_res) {
    dbg_printf("v_res is not zero. see %d\n", v_res);
  }

  if ((v_res = bind(v_sock, &v_addr.m_addr, SockAddrLen))) {
    dbg_printf("bind has failed. see %d\n", v_res);
    close(v_sock);
  } else if (v_sock != INVALID_SOCKET) {
    SvrUnits[room].ID.fd = v_sock;
  }
}

#define __RoomActivate _RoomActivate

ae2f_MAC()
    _SetPlayerOffline(const uSockAddr *const addr, globplayer_t *const retidx) {

  for (*(retidx) = 0; *(retidx) < MAX_GLOBAL_PLAYER_COUNT; (*(retidx))++) {
    if (uSockAddrInCheckNPort((addr), (&PlConns[*(retidx)].m_addr))) {
      memset(&PlConns[*(retidx)].m_addr, 0, sizeof(PlConns[*(retidx)].m_addr));
      __PlayerInit(*(retidx));
      Players[*(retidx)].m_connected = 0;
      if (!(--Rooms[(*(retidx)) / MAX_ROOM_MEM_COUNT].m_member)) {
        __RoomTerminate((*(retidx)) / MAX_ROOM_MEM_COUNT);
      }
      {
        dbg_printf("Got someone offline: %d\n", (*retidx));
        break;
      }
    }
  }
  __uSockAddrPrint(addr);
  __uSockAddrPrint(&PlConns[0].m_addr);
  __uSockAddrPrint(&PlConns[1].m_addr);
}

ae2f_MAC()
    _SetPlayerOnline(const uSockAddr *const addr, const globplayer_t globidx) {
  dbg_puts("__SetPlayerOnline");
  if ((addr) && __IsPlayerNull((globidx))) {
    dbg_puts("__SetPlayerOnline: ON");
    PlConns[globidx].m_addr = *(addr);
    Players[globidx].m_connected = 1;
    if (!(Rooms[(globidx) / MAX_ROOM_MEM_COUNT].m_member++)) {
      __RoomActivate((globidx) / MAX_ROOM_MEM_COUNT);
    }
  }
}

#define __SetPlayerOffline _SetPlayerOffline
#define __SetPlayerOnline _SetPlayerOnline

#endif
