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

        public fixed sbyte m_Name[@MAX_ROOM_NAME_COUNT@];
        public byte m_started;
        public uint m_member;
    }

    public struct room_t {
        public @room_cs_t@ a;
    }
}