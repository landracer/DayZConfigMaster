// Source: F:/Games/Dayz/scripts/4_world/entities/woodbase/christmastree.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ChristmasTree extends House
{
    XmasTreeLight       m_TreeLight;
    EffectSound         m_AmbientSoundLoop;
    const string        LOOP_SOUND = &quot;ChristmasMusic_SoundSet&quot;;
    
    void ChristmasTree()
    {
        if ( !g_Game.IsDedicatedServer() )
        {
            m_TreeLight = XmasTreeLight.Cast( ScriptedLightBase.CreateLight( XmasTreeLight, &quot;0 0 0&quot; ) );
            m_TreeLight.AttachOnMemoryPoint( this, &quot;action&quot; );
        }
