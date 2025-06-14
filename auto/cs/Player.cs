
namespace Core
{
    public static unsafe class Player {
        public const int MAX_GLOBAL_PLAYER_COUNT = ((@MAX_ROOM_PLAYER_COUNT@ + @MAX_ROOM_SPECTATOR_COUNT@) * @MAX_ROOM_COUNT@);
    }

    public struct player_t {
        public @player_cs_t@ a;
    }

    public struct globplayer_t {
        public @globplayer_cs_t@ a;

        public bool isvalid { get { return this.a != @globplayer_cs_t@.MaxValue; } }

        public room_t room
        {
            get
            {
                room_t r;
                r.a = (@room_cs_t@)(a / Room.MAX_ROOM_MEM_COUNT);
                return r;
            }
        }

        public player_t player
        {
            get
            {
                player_t player;
                player.a = (@player_cs_t@)(a % Room.MAX_ROOM_MEM_COUNT);
                return player;
            }
        }
    }
}