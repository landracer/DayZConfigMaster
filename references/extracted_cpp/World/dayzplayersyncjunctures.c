// Source: F:/Games/Dayz/scripts/4_world/entities/dayzplayersyncjunctures.c Source File
// Extracted from DayZ Code Explorer by Zeroy

// *************************************************************************************
// ! DayZPlayerSyncJunctures - sync junctures for dayz player - static functions
// *************************************************************************************
class DayZPlayerSyncJunctures
{
    static const int SJ_DAMAGE_HIT                      = 0;
    static const int SJ_INJURY                          = 1;
    static const int SJ_ACTION_INTERRUPT                = 2;
    static const int SJ_PLAYER_STATES                   = 3;
    static const int SJ_QUICKBAR_SET_SHORTCUT           = 4;
    static const int SJ_INVENTORY                       = 5;
    static const int SJ_ACTION_ACK_ACCEPT               = 6;
    static const int SJ_ACTION_ACK_REJECT               = 7;
    static const int SJ_WEAPON_ACTION_ACK_ACCEPT        = 8;
    static const int SJ_WEAPON_ACTION_ACK_REJECT        = 9;
    static const int SJ_WEAPON_SET_JAMMING_CHANCE       = 10;
    static const int SJ_UNCONSCIOUSNESS                 = 11;
    static const int SJ_DEATH                           = 12;
    static const int SJ_PLAYER_FB_MODIFIER              = 13;
    static const int SJ_PLAYER_ADD_MODIFIER             = 14;
    static const int SJ_KURU_REQUEST                    = 15;
    static const int SJ_GESTURE_REQUEST                 = 16;
    static const int SJ_INVENTORY_REPAIR                = 17;
    static const int SJ_WEAPON_LIFT                     = 18;
    static const int SJ_WEAPON_RAISE_COMPLETED          = 19;
    static const int SJ_DELETE_ITEM                     = 20;
    static const int SJ_BROKEN_LEGS                     = 21;
    static const int SJ_SHOCK                           = 22;
    static const int SJ_STAMINA                         = 23;
    static const int SJ_STAMINA_MISC                    = 24;
    static const int SJ_ADS_RESET                       = 25;
    static const int SJ_INVENTORY_FAILURE               = 26;
    #ifdef DEVELOPER
    static const int SJ_DEBUG_GET_IN_VEHICLE            = 200;
    #endif
    
    
    #ifdef DEVELOPER
    //-------------------------------------------------------------
 
    static void SendGetInVehicle(DayZPlayer pPlayer, EntityAI vehicle)
    {
        ScriptJunctureData ctx = new ScriptJunctureData();
        ctx.Write(vehicle);
        pPlayer.SendSyncJuncture(SJ_DEBUG_GET_IN_VEHICLE, ctx);
    }
    
    static bool ReadGetInVehicleParams(ParamsReadContext pCtx, out EntityAI vehicle)
    {
        if (!pCtx.Read(vehicle))
            return false;
 
        return true;
    }
    #endif
    
    //-------------------------------------------------------------
 
    static void SendDeath(DayZPlayer pPlayer, int pType, float pHitDir)
    {
        ScriptJunctureData ctx = new ScriptJunctureData;
        
        ctx.Write(pType);
        ctx.Write(pHitDir);
        pPlayer.SendSyncJuncture(SJ_DEATH, ctx);
