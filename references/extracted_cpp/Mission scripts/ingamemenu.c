// Source: F:/Games/Dayz/scripts/5_mission/gui/ingamemenu.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class InGameMenu extends UIScriptedMenu
{
    string                      m_ServerInfoText;
    
    protected Widget            m_ContinueButton;
    protected Widget            m_SeparatorPanel;
    protected Widget            m_ExitButton;
    protected Widget            m_RestartButton;
    protected Widget            m_RespawnButton;
    protected Widget            m_RestartDeadRandomButton;
    protected Widget            m_RestartDeadCustomButton;
    protected Widget            m_OptionsButton;
    protected Widget            m_ServerInfoPanel;
    protected Widget            m_FavoriteButton;
    protected Widget            m_FavoriteImage;
    protected Widget            m_UnfavoriteImage;
    protected Widget            m_CopyInfoButton;
    protected Widget            m_FeedbackButton;
    
    protected ref TextWidget    m_ModdedWarning;
    protected ref TextWidget    m_ServerIP;
    protected ref TextWidget    m_ServerPort;
    protected ref TextWidget    m_ServerName;
    
    protected ref UiHintPanel m_HintPanel;
    
    void ~InGameMenu()
    {
        HudShow(true);
        
        Mission mission = g_Game.GetMission();
        if (mission)
            mission.Continue();
