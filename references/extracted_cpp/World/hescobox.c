// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/hescobox.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class HescoBox extends Inventory_Base
{
    static const int FOLDED            = 0;
    static const int UNFOLDED          = 1;
    static const int FILLED            = 2;
    static const int PERCENTUAL_DAMAGE = 1;
    
    ref Timer                       m_Timer;
 
    protected int m_State;
    
    void HescoBox()
    {
        m_State = FOLDED;
 
        //synchronized variables
        RegisterNetSyncVariableInt( &quot;m_State&quot;, FOLDED, FILLED );
