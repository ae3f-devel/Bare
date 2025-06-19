using globplayer_t = @globplayer_cs_t@;
using room_t = @room_cs_t@;
using player_t = @player_cs_t@;
using System.Runtime.InteropServices;


namespace Core
{
    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public unsafe struct Room
    {
        public const int MAX_ROOM_NAME_COUNT = @MAX_ROOM_NAME_COUNT@;
        public const int MAX_ROOM_PLAYER_COUNT = @MAX_ROOM_PLAYER_COUNT@;
        public const int MAX_ROOM_SPECTATOR_COUNT = @MAX_ROOM_SPECTATOR_COUNT@;
        public const int MAX_ROOM_COUNT = @MAX_ROOM_COUNT@;
        public const int MAX_ROOM_MEM_COUNT = @MAX_ROOM_PLAYER_COUNT@ + @MAX_ROOM_SPECTATOR_COUNT@;

        internal fixed sbyte m_Name[@MAX_ROOM_NAME_COUNT@];
        public byte m_started;
        public @room_cs_t@ m_member;

        public string name
        {
            get
            {
                fixed (sbyte* _name = m_Name)
                {
                    return Marshal.PtrToStringAnsi((nint)_name);   
                }
            }
        }
    }

    public struct RoomIndex {
        public @room_cs_t@ a;
        public const @room_cs_t@ INVALID = @room_cs_t@.MaxValue;
    }
}