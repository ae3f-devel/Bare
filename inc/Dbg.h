#include "./Dbg.auto.h"

#if DEBUG
#include <stdio.h>
#include <assert.h>
#include <ae2f/Call.h>
#include <ae2f/Cast.h>

#define when_DEBUG(...) __VA_ARGS__


ae2f_extern ae2f_SHAREDCALL
void TestLog(const char* c);

#else
#define when_DEBUG(...)
#define TestLog(str)

#endif

#define dbg_prefix  "[DEBUG] "
#define err_prefix  "[ERR] "

#define dbg_puts(s)   when_DEBUG(puts(dbg_prefix      s); TestLog(dbg_prefix s))
#define dbg_printf(...) when_DEBUG(printf(dbg_prefix    __VA_ARGS__))

#define err_puts(s)   when_DEBUG(puts(err_prefix dbg_prefix s))
#define err_printf(...) when_DEBUG(printf(err_prefix dbg_prefix __VA_ARGS__))

#define assert_prefix "[ASSERT] "
#define assertmsg(...)  when_DEBUG(if(!(__VA_ARGS__)) { puts(dbg_prefix assert_prefix #__VA_ARGS__ "has failed!"); assert(0); })