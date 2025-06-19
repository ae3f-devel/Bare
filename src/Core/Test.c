#include <Dbg.h>

#if DEBUG

static
void (*TestLogString) (const char*) = 0;

ae2f_SHAREDEXPORT
void SetTestLogger(void (*_TestLogString)(const char*)) {
    TestLogString = _TestLogString;
}

ae2f_SHAREDEXPORT
void TestLog(const char* c) {
    if(TestLogString) TestLogString(c);
}

#endif