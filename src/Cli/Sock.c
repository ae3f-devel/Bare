#include <Sock.h>
#include <ae2f/Call.h>
#include <ae2f/Inet/Block.h>

ae2f_SHAREDEXPORT sock_t SockOpen() {
  sock_t s = socket(AF_INET, SOCK_DGRAM, 0);
  int res;
  ae2f_InetNBlock(s, &res);

  if (res) {
    close(s);
    return -1;
  }

  return s;
}

#include <Timeout.h>
#undef timeout

ae2f_SHAREDEXPORT uint32_t SetTimeOut(uint32_t t) {
  static time_t timeout;
  if(timeout <= 0) timeout = 1; /* Default value */

  if (t > 0)
    timeout = t;

  return (t);
}

ae2f_SHAREDEXPORT void SockClose(sock_t a) { close(a); }
