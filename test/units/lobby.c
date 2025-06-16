#include <Dbg.h>
#include <Room.imp.auto.h>
#include <Sock.h>
#include <Sock.imp.h>
#include <stdio.h>

#include "../port.auto.h"

int main() {
  ae2f_InetMkData data;
  ae2f_InetMk(MAKEWORD(2, 2), &data);

  sock_t sock = SockOpen();
  int a;
  dbg_printf("%d\n", a);

  uSockAddr in[1];
  __uSockAddrMk(in, "127.0.0.1", TEST_PORT);
  room_t r;

  ReqRoomLobby(sock, in, -1, &r, "FEARNOT", NULL, "Playername");

  printf("%d\n", r);

  close(sock);
  ae2f_InetDel();
  return 0;
}
