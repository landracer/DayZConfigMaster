// Source: F:/Games/Dayz/scripts/3_game/gui/effects/bouncer.c Source File
// Extracted from DayZ Code Explorer by Zeroy

// -----------------------------------------------------------
class Bouncer
{
    reference float amount;
    reference float speed;
 
    protected float m_orginal_width;
    protected float m_orginal_height;
    protected Widget m_root;
    protected ref AnimatorTimer m_anim;
 
    // -----------------------------------------------------------
    void Bouncer()
    {
        m_anim = new AnimatorTimer();
