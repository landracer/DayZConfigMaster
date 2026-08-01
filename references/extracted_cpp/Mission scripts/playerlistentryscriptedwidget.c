// Source: F:/Games/Dayz/scripts/5_mission/gui/ingamemenu_xbox/playerlistentryscriptedwidget.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class PlayerListEntryScriptedWidget extends ScriptedWidgetEventHandler
{
    protected string                    m_Name;
    protected string                    m_UID;
    protected bool                      m_Mute;
    protected bool                      m_GlobalMute;
    
    protected Widget                    m_Root;
    protected TextWidget                m_PlayerName;
    protected ImageWidget               m_PlayerAvatar;
    protected ImageWidget               m_MicrophoneIcon;
    protected ImageWidget               m_MuteIcon;
    protected ButtonWidget              m_PlayerButton;
    
    protected PlayerListScriptedWidget  m_Tab;
    protected bool                      m_Selected;
    
    void PlayerListEntryScriptedWidget( Widget parent, string name, string uid, bool show_permissions, PlayerListScriptedWidget tab )
    {
        m_Name              = name;
        m_UID               = uid;
        m_Tab               = tab;
        
        m_Root              = g_Game.GetWorkspace().CreateWidgets( &quot;gui/layouts/xbox/ingamemenu_xbox/player_info_entry.layout&quot;, parent );
        m_PlayerName        = TextWidget.Cast( m_Root.FindAnyWidget( &quot;Name&quot; ) );
        m_PlayerAvatar      = ImageWidget.Cast( m_Root.FindAnyWidget( &quot;Avatar&quot; ) );
        m_MicrophoneIcon    = ImageWidget.Cast( m_Root.FindAnyWidget( &quot;Microphone&quot; ) );
        m_MuteIcon          = ImageWidget.Cast( m_Root.FindAnyWidget( &quot;Muted&quot; ) );
        m_PlayerButton      = ButtonWidget.Cast( m_Root.FindAnyWidget( &quot;Button&quot; ) );
        
        m_MicrophoneIcon.Show( show_permissions && !IsLocalPlayer() );
        
        m_PlayerName.SetText( name );
        m_Root.SetHandler( this );
