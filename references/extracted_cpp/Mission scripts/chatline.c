// Source: F:/Games/Dayz/scripts/5_mission/gui/chat/chatline.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ChatLine
{
    // Consts
    const float FADE_TIMEOUT = 30;
    const float FADE_OUT_DURATION = 3;
    const float FADE_IN_DURATION = 0.5;
    
    private const string RADIO_PREFIX = &quot;(&quot; + &quot;#str_radio&quot; + &quot;) &quot;;
    private const string GAME_PREFIX = &quot;(&quot; + &quot;#layout_chat_game&quot; + &quot;) &quot;;
    private const string ADMIN_PREFIX = &quot;(&quot; + &quot;#STR_MP_MASTER&quot; + &quot;) &quot;;
    private const int    DEFAULT_COLOUR = ARGB(255, 255, 255, 255);
    private const int    GAME_TEXT_COLOUR = ARGB(255, 255, 0, 0);
    private const int    ADMIN_TEXT_COLOUR = ARGB(255, 255, 255, 0);
    
    // Widgets
    Widget                          m_RootWidget;
    TextWidget                      m_NameWidget;
    TextWidget                      m_TextWidget;
    
    private ref WidgetFadeTimer     m_FadeTimer;
    private ref Timer               m_TimeoutTimer;
 
    void ChatLine(Widget root_widget)
    {
        m_RootWidget    = g_Game.GetWorkspace().CreateWidgets(&quot;gui/layouts/day_z_chat_item.layout&quot;, root_widget);
    
        m_NameWidget    = TextWidget.Cast( m_RootWidget.FindAnyWidget( &quot;ChatItemSenderWidget&quot; ) );
        m_TextWidget    = TextWidget.Cast( m_RootWidget.FindAnyWidget( &quot;ChatItemTextWidget&quot; ) );
        
        m_FadeTimer     = new WidgetFadeTimer;
        m_TimeoutTimer  = new Timer(CALL_CATEGORY_GUI);
