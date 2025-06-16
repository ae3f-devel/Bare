#include <Sock.h>
#include <ae2f/Inet/Block.h>
#include <ae2f/Call.h>

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

ae2f_SHAREDEXPORT time_t timeout = 1;

ae2f_SHAREDEXPORT time_t SetTimeOut(time_t t) {
  if (t <= 0)
    return t;
  return (timeout = t);
}

ae2f_SHAREDEXPORT void SockClose(sock_t a) { close(a); }
