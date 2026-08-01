// Source: F:/Games/Dayz/scripts/4_world/plugins/pluginbase/pluginlifespan.c Source File
// Extracted from DayZ Code Explorer by Zeroy

//-----------------------------
// LIFESPAN plugin
//-----------------------------
/*
Lifespan plugin handles player facial hair, bloody hands, blood type in HUD
*/
 
enum eBloodyHandsTypes
{
    CLEAN = 0,//clean needs to be 0
    SALMONELA,
    JUST_BLOOD,
    //--- ONLY LAST_INDEX BELLOW !!!
    LAST_INDEX,
