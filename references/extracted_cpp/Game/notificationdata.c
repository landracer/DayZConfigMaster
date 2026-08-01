// Source: F:/Games/Dayz/scripts/3_game/client/notifications/notificationdata.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class NotificationData
{
    string m_Icon;
    string m_TitleText;
    string m_DescriptionText;
    
    void NotificationData(string icon, string title_text, string desc_text = &quot;&quot;)
    {
        m_Icon = icon;
        m_TitleText = title_text;
        if (desc_text != &quot;&quot;)
            m_DescriptionText = desc_text;
