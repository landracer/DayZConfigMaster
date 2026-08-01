// Source: F:/Games/Dayz/scripts/4_world/plugins/pluginbase/plugindiagmenu/plugindiagmenu.c Source File
// Extracted from DayZ Code Explorer by Zeroy

// For modding, see PluginDiagMenuModding.c
// !!! MODDING DISCLAIMER: These are debug functionality files, if you are thinking about modding the vanilla ones, do so at your own risk
//                         These files will not be maintained with the thought of &quot;what if a modder modded this&quot; (Excluding the modding functionality of course)
//                         Which is why the modding functionality was developed with the thought of the modded ones having their own isolated safe space
 
enum DebugActionType
{
    GENERIC_ACTIONS = 1,
    PLAYER_AGENTS = 2,
    UNLIMITED_AMMO = 4,
