// Source: F:/Games/Dayz/scripts/4_world/classes/playernotifiers/notifiers/bleedingnotfr.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class BleedingNotfr: NotifierBase
{
    void BleedingNotfr(NotifiersManager manager)
    {
        m_TendencyBufferSize = 8;
        m_Active = false; 
