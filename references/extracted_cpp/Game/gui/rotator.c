// Source: F:/Games/Dayz/scripts/3_game/gui/effects/rotator.c Source File
// Extracted from DayZ Code Explorer by Zeroy

// -----------------------------------------------------------
class Rotator
{
    reference float speed;
 
    protected Widget m_root;    
    protected ref AnimatorTimer m_anim;
 
    // -----------------------------------------------------------
    void Rotator()
    {
        m_anim = new AnimatorTimer();
