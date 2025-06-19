#ifndef Timeout_t
#define Timeout_t

#include <ae2f/Call.h>
#include <ae2f/Cast.h>
#include <time.h>
#include <stdint.h>

ae2f_extern ae2f_SHAREDCALL uint32_t SetTimeOut(uint32_t t);
#define timeout             ae2f_static_cast(time_t, SetTimeOut(0))

#define ELAPSED(t) (time(0) - (t))
#define TIMEOUT(t) (ELAPSED(t) > (timeout))

#endif
