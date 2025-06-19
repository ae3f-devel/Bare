#ifndef Sock_h
#define Sock_h

#include <ae2f/Inet.h>

#if _WIN32
#include <ws2tcpip.h>
#endif

typedef ae2f_InetSock sock_t;
typedef struct sockaddr_in sockaddr_internal_t;
typedef struct sockaddr sockaddr_t;
typedef socklen_t socklen_t;

typedef union uSockAddr {
  sockaddr_t m_addr;
  sockaddr_internal_t m_in;
} uSockAddr;

#define uSockAddrInCheckNPort(a, b)                                            \
  ((a)->m_in.sin_addr.s_addr == (b)->m_in.sin_addr.s_addr)

#define uSockAddrInCheck(a, b)                                                 \
  (uSockAddrInCheckNPort(a, b) &&                   \
   ((a)->m_in.sin_port == (b)->m_in.sin_port))



#define SockAddrLen (sizeof(sockaddr_internal_t))

ae2f_extern ae2f_SHAREDCALL sock_t SockOpen();

#endif
