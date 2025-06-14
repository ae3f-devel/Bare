#define SERVER 1

#include <Max.auto.h>
#include <Player.h>
#include <Room.h>

#include "./PlConn.h"
#include "./RoomPrivate.h"

ae2f_SHAREDEXPORT Player
    Players[MAX_GLOBAL_PLAYER_COUNT] = {0, };
    
ae2f_SHAREDEXPORT Room Rooms[MAX_ROOM_COUNT] = {0, };
ae2f_SHAREDEXPORT RoomPrivate RoomPrivates[MAX_ROOM_COUNT] = {0,};

/** @brief Alignment matters. VERY MUCH. */
ae2f_SHAREDEXPORT ae2f_addrel_t RoomFlags[MAX_ROOM_COUNT + 1] = {0, };
ae2f_SHAREDEXPORT PlConn PlConns[MAX_GLOBAL_PLAYER_COUNT] = {0, };