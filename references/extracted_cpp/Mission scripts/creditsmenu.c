// Source: F:/Games/Dayz/scripts/5_mission/gui/newui/credits/creditsmenu.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class CreditsMenu extends UIScriptedMenu
{
    protected static float                      MENU_FADEIN_TIME    = 2.0;      //fade starts as soon as menu opens
    protected static float                      LOGO_FADEIN_TIME    = 1.0;      //fade starts halfway through menu fade in
    protected static float                      CREDIT_SCROLL_SPEED = 200.0;    //pixels per second (relative to 1080p res)
    
    protected float                             m_MenuFadeInIncrement;
    protected float                             m_MenuFadeInLevel;
    protected float                             m_MenuFadeInLevel2 = 1;
    protected float                             m_LogoFadeInIncrement;
    protected float                             m_LogoFadeInLevel;
    protected float                             m_ScrollIncrement;
    protected float                             m_ScrollLevel;
    protected float                             m_ScrollMax;
    protected float                             m_ScrollSize;
    
    protected ref JsonDataCredits               m_CreditsData;
    
    protected float                             m_CurrentTime = 0.0;
    protected ref array<ref CreditsElement>     m_CreditsEntries = new array<ref CreditsElement>;
    
    protected ImageWidget                       m_Logo;
    protected ScrollWidget                      m_Scroller;
    protected WrapSpacerWidget                  m_Content;
    protected RichTextWidget                    m_InfoPanelText;
    protected Widget                            m_InfoPanel;
    
    override Widget Init()
    {
        float x_f;
        int x, y;
        
        layoutRoot  = g_Game.GetWorkspace().CreateWidgets( &quot;gui/layouts/new_ui/credits/credits_menu.layout&quot;, null );
        m_Logo      = ImageWidget.Cast( layoutRoot.FindAnyWidget( &quot;Logo&quot; ) );
        m_Scroller  = ScrollWidget.Cast( layoutRoot.FindAnyWidget( &quot;CreditsPanel&quot; ) );
        m_Content   = WrapSpacerWidget.Cast( layoutRoot.FindAnyWidget( &quot;CreditsContent&quot; ) );
        m_InfoPanelText = RichTextWidget.Cast( layoutRoot.FindAnyWidget( &quot;InfoPanelText&quot; ) );
        m_InfoPanel = layoutRoot.FindAnyWidget( &quot;InfoPanel&quot; );
        
        GetScreenSize( x, y );
        
        m_MenuFadeInIncrement   = 1 / MENU_FADEIN_TIME;
        m_LogoFadeInIncrement   = 1 / LOGO_FADEIN_TIME;
        m_ScrollIncrement       = CREDIT_SCROLL_SPEED * ( y / 1080 );
        
        m_Scroller.VScrollToPos01( 0 );
        m_Scroller.GetScreenSize( x_f, m_ScrollSize );
        
        g_Game.GameScript.Call( this, &quot;LoadDataAsync&quot;, null );
        
        UpdateInfoPanelText(g_Game.GetInput().GetCurrentInputDevice());
        
        return layoutRoot;
