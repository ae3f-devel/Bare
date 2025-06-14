#ifndef RoomPrivate_h
#define RoomPrivate_h

#include <Room.h>
#include <ae2f/Atom.h>

typedef struct RoomPrivate {
  char m_Pw[MAX_ROOM_PW];
} RoomPrivate;

ae2f_extern ae2f_SHAREDCALL Room Rooms[MAX_ROOM_COUNT];
ae2f_extern ae2f_SHAREDCALL RoomPrivate RoomPrivates[MAX_ROOM_COUNT];

#endif
