// Source: F:/Games/Dayz/scripts/3_game/systems/inventory/junctures.c Source File
// Extracted from DayZ Code Explorer by Zeroy

 
bool TryAcquireInventoryJunctureFromServer (notnull Man player, notnull InventoryLocation src, notnull InventoryLocation dst)
{
    if (player.NeedInventoryJunctureFromServer(src.GetItem(), src.GetParent(), dst.GetParent()))
    {
        if ( ( src.GetItem() && src.GetItem().IsSetForDeletion() ) || ( src.GetParent() && src.GetParent().IsSetForDeletion() ) || ( dst.GetParent() && dst.GetParent().IsSetForDeletion() ) )
        {
            return JunctureRequestResult.JUNCTURE_DENIED;
        }
        
        if (src.GetItem() && !src.GetItem().CanPutIntoHands(player))
        {
            return JunctureRequestResult.JUNCTURE_DENIED;
        }
        
        bool test_dst_occupancy = true;
        if (g_Game.AddInventoryJunctureEx(player, src.GetItem(), dst, test_dst_occupancy, GameInventory.c_InventoryReservationTimeoutMS))
        {
            if (LogManager.IsSyncLogEnable()) syncDebugPrint(&quot;[syncinv] juncture needed and acquired, player=&quot; + Object.GetDebugName(player) + &quot; STS = &quot; + player.GetSimulationTimeStamp() + &quot; src=&quot; + InventoryLocation.DumpToStringNullSafe(src) + &quot; dst=&quot; + InventoryLocation.DumpToStringNullSafe(dst));
            return JunctureRequestResult.JUNCTURE_ACQUIRED; // ok
        }
        else
        {
            if (LogManager.IsSyncLogEnable()) syncDebugPrint(&quot;[syncinv] juncture request DENIED, player=&quot; + Object.GetDebugName(player) + &quot; STS = &quot; + player.GetSimulationTimeStamp() + &quot; src=&quot; + InventoryLocation.DumpToStringNullSafe(src) + &quot; dst=&quot; + InventoryLocation.DumpToStringNullSafe(dst));
            return JunctureRequestResult.JUNCTURE_DENIED; // permission to perform juncture denied
        }
    }
    else
    {
        if (LogManager.IsSyncLogEnable()) syncDebugPrint(&quot;[syncinv] juncture not required, player=&quot; + Object.GetDebugName(player) + &quot; STS = &quot; + player.GetSimulationTimeStamp() + &quot; src=&quot; + InventoryLocation.DumpToStringNullSafe(src) + &quot; dst=&quot; + InventoryLocation.DumpToStringNullSafe(dst));
        return JunctureRequestResult.JUNCTURE_NOT_REQUIRED; // juncture not necessary
    }
