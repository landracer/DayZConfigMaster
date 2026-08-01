// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/flag_base.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class Flag_Base extends ItemBase
{
    void Flag_Base()
    {
        //synchronized variables
        //RegisterNetSyncVariableBool( &quot;m_IsMounted&quot; );
        ShowSelection(&quot;folded&quot;);
        HideSelection(&quot;unfolded&quot;);
