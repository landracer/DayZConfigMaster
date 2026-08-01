// Source: F:/Games/Dayz/scripts/3_game/gui/effects/mainmenubuttoneffect.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class MainMenuButtonEffect : ScriptedWidgetEventHandler
{
    reference float speed;
    reference float amount;
    protected float m_textProportion;
    protected float m_textProportion2;
    protected ButtonWidget m_root;
    protected ref AnimatorTimer m_anim;
    
    // -----------------------------------------------------------
    void MainMenuButtonEffect()
    {
        if ( g_Game )
        {
            g_Game.GetUpdateQueue(CALL_CATEGORY_GUI).Insert(this.Update);
        }
        m_anim = new AnimatorTimer();
