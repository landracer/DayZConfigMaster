// Source: F:/Games/Dayz/scripts/4_world/classes/playermodifiers/modifiersmanager.c Source File
// Extracted from DayZ Code Explorer by Zeroy

//max 32 synced modifiers supported
enum eModifierSyncIDs 
{
    MODIFIER_SYNC_WOUND_INFECT_1    = 0x00000001,
    MODIFIER_SYNC_WOUND_INFECT_2    = 0x00000002,
    MODIFIER_SYNC_CONTAMINATION     = 0x00000004,//stage1 
    MODIFIER_SYNC_CONTAMINATION2    = 0x00000008,//stage2 and stage3 share the same sync id
    MODIFIER_SYNC_ZONE_EXPOSURE     = 0x00000010,
    MODIFIER_SYNC_DROWNING          = 0x00000020,
    MODIFIER_SYNC_FEVER             = 0x00000040,
    //0x00000080,
    //0x00000100,
    //0x00000200,
    //0x00000400,
    //0x00000800,
    //0x00001000,
    LAST_INDEX,
