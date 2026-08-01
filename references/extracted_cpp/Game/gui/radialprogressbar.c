// Source: F:/Games/Dayz/scripts/3_game/gui/effects/radialprogressbar.c Source File
// Extracted from DayZ Code Explorer by Zeroy

// -----------------------------------------------------------
class RadialProgressBar
{
    reference float speed;
    reference float start_rotation;
 
    protected string m_BarHider;
    protected string m_BarPart;
    protected Widget m_Root;    
    protected ref AnimatorTimer m_Anim;
    protected float x, y, z;
    protected float rotation = 0;
    protected int stage = 0;
 
    // -----------------------------------------------------------
    void RadialProgressBar()
    {
        m_Anim = new AnimatorTimer();
        g_Game.GetUpdateQueue(CALL_CATEGORY_GUI).Insert(this.Update);
