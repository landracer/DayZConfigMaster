// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/cablereel.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class CableReel extends ItemBase
{
    bool m_ForceIntoHands;
 
    static const string SEL_CORD_FOLDED     = &quot;cord_folded&quot;;
    static const string SEL_CORD_PLUGGED    = &quot;cord_plugged&quot;;
    
    void CableReel()
    {
        m_ForceIntoHands = false;
