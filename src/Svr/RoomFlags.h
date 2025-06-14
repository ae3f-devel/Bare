#ifndef RoomFlags_h
#define RoomFlags_h

#include <Max.auto.h>
#include <ae2f/Atom.h>
#include <ae2f/Call.h>
#include <ae2f/Cast.h>
#include <WaitWake.h>

ae2f_extern
    ae2f_SHAREDEXPORT ae2f_Atom(ae2f_addrel_t) RoomFlags[MAX_ROOM_COUNT + 1];

enum eRoomFlags { eRoomFlags_PAUSED, eRoomFlags_RUNNING, eRoomFlags_KILL };

#endif
