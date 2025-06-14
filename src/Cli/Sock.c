#include <Sock.h>
#include <ae2f/Call.h>

ae2f_SHAREDEXPORT
sock_t SockOpen() {
    return socket(AF_INET, SOCK_DGRAM, 0);
}

ae2f_SHAREDEXPORT
void SockClose(sock_t a) {
    close(a);
}