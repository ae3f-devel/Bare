#include <Sock.imp.h>
#include <stddef.h>

ae2f_SHAREDEXPORT
void SockAddrMk(sockaddr_internal_t* addr, const char* ip, unsigned short port) {
    __SockAddrMk(addr, ip, port);
}