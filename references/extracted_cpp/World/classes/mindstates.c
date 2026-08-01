// Source: F:/Games/Dayz/scripts/4_world/classes/soundevents/infectedsoundevents/events/mindstates.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class MindStateSoundEventBase extends InfectedSoundEventBase
{
    override bool CanPlay()
    {
        if( !super.CanPlay() )
        {
            return false;
        }
        return true;
