// Source: F:/Games/Dayz/scripts/5_mission/gui/inventorynew/playerpreview.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class PlayerPreview: LayoutHolder
{
    protected ref PlayerPreviewWidget m_CharacterPanelWidget;
 
    protected int m_CharacterRotationX;
    protected int m_CharacterRotationY;
    protected int m_CharacterScaleDelta;
    protected vector m_CharacterOrientation;
    protected bool m_IsHolding;
    
    void PlayerPreview( LayoutHolder parent )
    {
        m_CharacterPanelWidget = PlayerPreviewWidget.Cast( m_Parent.GetMainWidget().FindAnyWidget( &quot;CharacterPanelWidget&quot; ) );
 
        WidgetEventHandler.GetInstance().RegisterOnMouseButtonDown( m_Parent.GetMainWidget().FindAnyWidget( &quot;CharacterPanel&quot; ),  this, &quot;MouseButtonDown&quot; );
        WidgetEventHandler.GetInstance().RegisterOnMouseWheel( m_Parent.GetMainWidget().FindAnyWidget( &quot;CharacterPanel&quot; ),  this, &quot;MouseWheel&quot; );
        
        m_CharacterScaleDelta = 1;
        m_CharacterPanelWidget.SetPlayer( g_Game.GetPlayer() );
        m_CharacterPanelWidget.SetModelPosition( &quot;0 0 0.605&quot; );
        m_CharacterPanelWidget.SetSize( 1.34, 1.34 );  // default scale
        UpdateScale();
