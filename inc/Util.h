#ifndef Util_h
#define Util_h

#if SERVER
#define when_SERVER(...) __VA_ARGS__
#define when_CLIENT(...) 
#else
#define when_SERVER(...) 
#define when_CLIENT(...) __VA_ARGS__
#endif

#endif