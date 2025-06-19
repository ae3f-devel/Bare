using globplayer_t = @globplayer_cs_t@;
using room_t = @room_cs_t@;
using player_t = @player_cs_t@;
using System;

namespace Core
{
    public static unsafe class Player {
        public const int MAX_GLOBAL_PLAYER_COUNT = ((@MAX_ROOM_PLAYER_COUNT@ + @MAX_ROOM_SPECTATOR_COUNT@) * @MAX_ROOM_COUNT@);
    }

    public struct PlayerIndex {
        public @player_cs_t@ a;
        public const @player_cs_t@ INVALID = @player_cs_t@.MaxValue;
    }

    public struct GlobPlayerIndex {
        public @globplayer_cs_t@ a;

        public const @globplayer_cs_t@ INVALID =  @globplayer_cs_t@.MaxValue;

        public bool isvalid { get { return this.a != INVALID; } }

        public void ThrowOnNValid()
        {
            if (a == INVALID)
                throw new Exception("room must not be invalid");
        }

        public room_t room
        {
            get
            {
                ThrowOnNValid();
                room_t r;
                r = (@room_cs_t@)(a / Room.MAX_ROOM_MEM_COUNT);
                return r;
            }
        }

        public player_t player
        {
            get
            {
                ThrowOnNValid();
                player_t player;
                player = (@player_cs_t@)(a % Room.MAX_ROOM_MEM_COUNT);
                return player;
            }
        }
    }
}