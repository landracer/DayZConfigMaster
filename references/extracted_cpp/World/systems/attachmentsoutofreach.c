// Source: F:/Games/Dayz/scripts/4_world/systems/inventory/attachmentsoutofreach.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class AttachmentsOutOfReach
{
    protected static ref map<string, ref map<int, vector>> m_AttData;
    
    static bool IsAttachmentReachable(EntityAI e, string att_slot_name = &quot;&quot;, int slot_id = -1, float range = 1.5)
    {
        if( !e.IgnoreOutOfReachCondition() )
        {
            PlayerBase player = PlayerBase.Cast( g_Game.GetPlayer() );
            if( player.IsInVehicle() )
            {
                return false;
            }
            else
            {
                
                vector pos_att;
                if ( slot_id != -1 )
                {
                    att_slot_name = InventorySlots.GetSlotName(slot_id);
                }
                if( att_slot_name != &quot;&quot; )
                {
                    if( e.MemoryPointExists(att_slot_name) )
                    {
                        vector mem_point = e.GetMemoryPointPos(att_slot_name);
                        pos_att = e.ModelToWorld(mem_point);
                    }
                    else
                    {
                        pos_att = e.ModelToWorld(GetAttachmentPosition(e, InventorySlots.GetSlotIdFromString( att_slot_name ) ));
                    }
                    
                }
                
                vector pos_player = player.GetPosition();
                
                float height_diff = Math.AbsFloat( pos_player[1] - pos_att[1] );
                if( height_diff < range )
                {
                    pos_player[1] = 0;
                    pos_att[1] = 0;
                    if ( vector.Distance(pos_player, pos_att) <= range )
                    {
                        return true;
                    }
                    return false;
                }
                else
                {
                    return false;
                }
            }
        }
        else
        {
            return true;
        }
