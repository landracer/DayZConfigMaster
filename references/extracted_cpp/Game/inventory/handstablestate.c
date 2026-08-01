// Source: F:/Games/Dayz/scripts/3_game/systems/inventory/handstablestate.c Source File
// Extracted from DayZ Code Explorer by Zeroy


class HandStableState extends HandStateBase
{
    int m_AnimState;
 
    void HandStableState (Man player = NULL, HandStateBase parent = NULL, int anim_state = -1) { m_AnimState = anim_state; }
 
    void SyncAnimState () { }
 
    override void OnEntry (HandEventBase e)
    {
        super.OnEntry(e);
        SyncAnimState();
 
        //m_weapon.OnStableStateEntry();
