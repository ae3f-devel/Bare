#ifndef Timeout_t
#define Timeout_t

#include <ae2f/Call.h>
#include <ae2f/Cast.h>
#include <time.h>

ae2f_extern ae2f_SHAREDCALL time_t timeout;
#define ELAPSED(t) (time(0) - (t))
#define TIMEOUT(t) (ELAPSED(t) >= (timeout))

#endif
