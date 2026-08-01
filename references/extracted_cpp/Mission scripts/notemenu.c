// Source: F:/Games/Dayz/scripts/5_mission/gui/notemenu.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class NoteMenu extends UIScriptedMenu
{
    protected MultilineEditBoxWidget m_edit;
    protected HtmlWidget m_html;
    protected ButtonWidget m_confirm_button;
    protected ItemBase m_Paper;
    protected EntityAI m_Pen;
    protected bool m_IsWriting;
    //protected int m_Handwriting;
    protected int m_SymbolCount;
    protected string m_PenColor; //color in hex-code format, transferred as string. Could be transferred as int or array<int, bool>?
 
    void NoteMenu()
    {
        MissionGameplay mission = MissionGameplay.Cast(g_Game.GetMission());
        if (mission)
        {
            IngameHud hud = IngameHud.Cast(mission.GetHud());
            if (hud)
            {
                hud.ShowHudUI(false);
            }
        }
