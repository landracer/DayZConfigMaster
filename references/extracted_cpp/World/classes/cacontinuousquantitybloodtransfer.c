// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actioncomponents/cacontinuousquantitybloodtransfer.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class CAContinuousQuantityBloodTransfer : CAContinuousQuantityRepeat
{
    override void CalcAndSetQuantity( ActionData action_data )
    {
        if ( g_Game.IsServer() )
        {
            if ( m_SpentUnits )
            {
                m_SpentUnits.param1 = m_SpentQuantity;
                SetACData(m_SpentUnits);
            }
 
            action_data.m_MainItem.AddQuantity( -m_SpentQuantity, false, false );
 
            ActionGiveBloodData action_data_b = ActionGiveBloodData.Cast( action_data );
 
            PlayerBase bloodtarget = PlayerBase.Cast( action_data.m_Target.GetObject() );
            if ( !bloodtarget )
                bloodtarget = action_data.m_Player;
 
            
            if( BloodTypes.MatchBloodCompatibility(action_data_b.m_ItemBloodType, bloodtarget.GetStatBloodType().Get()) )
            {
                bloodtarget.AddHealth(&quot;&quot;, &quot;Blood&quot;, m_SpentQuantity);
            }
 
        }
