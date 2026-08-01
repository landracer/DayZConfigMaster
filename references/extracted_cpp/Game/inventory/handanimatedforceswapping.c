// Source: F:/Games/Dayz/scripts/3_game/systems/inventory/handanimatedforceswapping.c Source File
// Extracted from DayZ Code Explorer by Zeroy

// When editing this, take a look at HandAnimatedMoveToDst_W4T_Basic as well
// They can not be inherited from each other because of different inheritance
class HandAnimatedMoveToDst_W4T_Basic extends HandStateBase
{
    ref InventoryLocation m_Dst;
 
    override void OnEntry(HandEventBase e)
    {
        Man player = e.m_Player;
        if (m_Dst && m_Dst.IsValid())
        {
            EntityAI item = m_Dst.GetItem();
            InventoryLocation src = new InventoryLocation;
            if (item.GetInventory().GetCurrentInventoryLocation(src))
            {
                if (g_Game.IsDedicatedServer())
                {
                    g_Game.ClearJunctureEx(m_Player, m_Dst.GetItem());
                }
                else
                {
                    m_Player.GetHumanInventory().ClearInventoryReservationEx(m_Dst.GetItem(), m_Dst);
                }
                #ifdef DIAG_DEVELOPER
                if (g_Game.IsMultiplayer() && InventoryDebug.IsHandAckEnable())
                {
                    if (!g_Game.IsDedicatedServer())
                    {
                        m_Player.GetHumanInventory().PostDeferredEventTakeToDst(InventoryMode.JUNCTURE, src, m_Dst);
                    }
                }
                else
                {
                #endif
                    if (GameInventory.LocationSyncMoveEntity(src, m_Dst))
                    {
                        player.OnItemInHandsChanged();
                    }
                    else
                    {
                        #ifdef ENABLE_LOGGING
                        if ( LogManager.IsInventoryHFSMLogEnable() )
                        {   
                            Debug.InventoryHFSMLog(&quot;[hndfsm] HandAnimatedMoveToDst_W4T_Basic - not allowed&quot;);
                        }
                        #endif
                    }
                #ifdef DIAG_DEVELOPER
                }
                #endif
            }
            else
                Error(&quot;[hndfsm] &quot; + Object.GetDebugName(e.m_Player) + &quot; STS = &quot; + e.m_Player.GetSimulationTimeStamp() + &quot; HandAnimatedMoveToDst_W4T_Basic - item &quot; + item + &quot; has no Inventory or Location, inv=&quot; + item.GetInventory());
        }
        else
            Error(&quot;[hndfsm] HandAnimatedMoveToDst_W4T_Basic - event has no valid m_Dst&quot;);
 
        super.OnEntry(e);
