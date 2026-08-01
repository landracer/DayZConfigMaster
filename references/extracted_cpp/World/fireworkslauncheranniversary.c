// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/gear/tools/fireworkslauncheranniversary.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class AnniversaryFireworksLauncherClientEvent: FireworksLauncherClientEvent
{
    override protected int GetExplParticleFromSequence()
    {
        switch (m_Color)
        {
            case &quot;1&quot;:
                return ParticleList.FIREWORKS_EXPLOSION_THANKS1;
            case &quot;2&quot;:
                return ParticleList.FIREWORKS_EXPLOSION_THANKS2;
            case &quot;3&quot;:
                return ParticleList.FIREWORKS_EXPLOSION_THANKS3;
            case &quot;4&quot;:
                return ParticleList.FIREWORKS_EXPLOSION_THANKS4;
            case &quot;5&quot;:
                return ParticleList.FIREWORKS_EXPLOSION_THANKS5;
            default:
                ErrorEx(&quot;Incorrect explosion particle color in the sequence&quot;);
        }
        return ParticleList.FIREWORKS_EXPLOSION_RED;
