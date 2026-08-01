// Source: F:/Games/Dayz/scripts/5_mission/gui/newui/mainmenu/mainmenunewsfeed.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class MainMenuNewsfeed extends ScriptedWidgetEventHandler
{
    protected Widget        m_Root;
    
    protected Widget        m_DLDLC;
    protected Widget        m_Discord;
    protected Widget        m_Feedback;
    protected Widget        m_DayZForum;
    protected Widget        m_Twitter;
    protected Widget        m_Youtube;
    
    protected TextWidget    m_MainText1;
    protected TextWidget    m_MainText2;
    protected TextWidget    m_MainText3;
    protected TextWidget    m_SecText1;
    protected TextWidget    m_SecText2;
    protected TextWidget    m_SecText3;
    
    void MainMenuNewsfeed( Widget root )
    {
        m_Root          = root;
        
        m_DLDLC         = m_Root.FindAnyWidget( &quot;downloaddlc&quot; );
        m_Discord       = m_Root.FindAnyWidget( &quot;discord&quot; );
        m_Feedback      = m_Root.FindAnyWidget( &quot;feedback_tracker&quot; );
        m_DayZForum     = m_Root.FindAnyWidget( &quot;dayz_forums&quot; );
        m_Twitter       = m_Root.FindAnyWidget( &quot;twitter&quot; );
        m_Youtube       = m_Root.FindAnyWidget( &quot;youtube&quot; );
        
        m_MainText1     = TextWidget.Cast( m_Root.FindAnyWidget( &quot;SGInfoT1&quot; ) );
        m_MainText2     = TextWidget.Cast( m_Root.FindAnyWidget( &quot;SGInfoT2&quot; ) );
        m_MainText3     = TextWidget.Cast( m_Root.FindAnyWidget( &quot;SGInfoT3&quot; ) );
        m_SecText1      = TextWidget.Cast( m_Root.FindAnyWidget( &quot;SGInfoC1&quot; ) );
        m_SecText2      = TextWidget.Cast( m_Root.FindAnyWidget( &quot;SGInfoC2&quot; ) );
        m_SecText3      = TextWidget.Cast( m_Root.FindAnyWidget( &quot;SGInfoC3&quot; ) );
        
        ShowNewsfeed();
        
        m_Root.SetHandler( this );
