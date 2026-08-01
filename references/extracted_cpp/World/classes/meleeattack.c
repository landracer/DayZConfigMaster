// Source: F:/Games/Dayz/scripts/4_world/classes/soundevents/playersoundevents/events/meleeattack.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class MeleeAttackSoundEvents extends PlayerSoundEventBase
{
    void MeleeAttackSoundEvents()
    {
        m_Type = EPlayerSoundEventType.MELEE;
        m_HasPriorityOverTypes = -1;
