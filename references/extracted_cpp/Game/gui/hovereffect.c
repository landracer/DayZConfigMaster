// Source: F:/Games/Dayz/scripts/3_game/gui/effects/hovereffect.c Source File
// Extracted from DayZ Code Explorer by Zeroy

// -----------------------------------------------------------
class HoverEffect : ScriptedWidgetEventHandler
{
    reference float speed;
    reference float amount;
    protected float m_orginal_width;
    protected float m_orginal_height;
    protected Widget m_root;
    protected ref AnimatorTimer m_anim;
    
    void HoverEffect()
    {
        m_anim = new AnimatorTimer();
