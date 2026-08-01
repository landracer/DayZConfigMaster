// Source: F:/Games/Dayz/scripts/3_game/client/notifications/notificationui.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class NotificationUI
{
    protected ref Widget    m_Root;
    protected ref Widget    m_Spacer;
    protected ref Widget    m_VoiceContent;
    protected ref Widget    m_NotificationContent;
    
    protected ref map<NotificationRuntimeData, Widget>  m_Notifications;
    protected ref map<string, Widget>                   m_VoiceNotifications;
    
    protected float                                     m_Width;
    protected float                                     m_CurrentHeight;
    protected float                                     m_TargetHeight;
    protected float                                     m_BackupPosX;
    protected float                                     m_BackupPosY;
    protected ref map<string, Widget>                   m_WidgetTimers;
    
    protected bool                                      m_OffsetEnabled;;
    
    void NotificationUI()
    {
        m_Root                  = g_Game.GetWorkspace().CreateWidgets(&quot;gui/layouts/new_ui/notifications/notifications.layout&quot;);
        m_Spacer                = m_Root.FindAnyWidget( &quot;NotificationSpacer&quot; );
        m_VoiceContent          = m_Root.FindAnyWidget( &quot;VoiceContent&quot; );
        m_NotificationContent   = m_Root.FindAnyWidget( &quot;NotificationContent&quot; );
        m_Notifications         = new map<NotificationRuntimeData, Widget>;
        m_VoiceNotifications    = new map<string, Widget>;
        m_WidgetTimers          = new map<string, Widget>;
        
        NotificationSystem ntfSys = NotificationSystem.GetInstance();
        if (ntfSys)
        {
            ntfSys.m_OnNotificationAdded.Insert( AddNotification );
            ntfSys.m_OnNotificationRemoved.Insert( RemoveNotification );
        }
