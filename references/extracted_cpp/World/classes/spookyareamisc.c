// Source: F:/Games/Dayz/scripts/4_world/classes/contaminatedarea/spookyareamisc.c Source File
// Extracted from DayZ Code Explorer by Zeroy

//-------------------------------------------------------------
class SpookyEventWind : SpookyEventBase
{
    override protected void Init()
    {
        SetCoolDown(65);
        m_SoundSet = &quot;SpookyArea_WhistlingWind_SoundSet&quot;;
