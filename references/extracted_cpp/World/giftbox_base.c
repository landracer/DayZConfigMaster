// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/giftbox_base.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class GiftBox_Base extends Container_Base
{
    protected vector m_HalfExtents; // The Y value contains a heightoffset and not the halfextent !!!
    protected ref OpenableBehaviour m_Openable;
    
    void GiftBox_Base()
    {
        m_HalfExtents   = vector.Zero;
        m_Openable      = new OpenableBehaviour(false);
 
        RegisterNetSyncVariableBool(&quot;m_Openable.m_IsOpened&quot;);
