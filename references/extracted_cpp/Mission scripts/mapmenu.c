// Source: F:/Games/Dayz/scripts/5_mission/gui/mapmenu.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class MapMenu extends UIScriptedMenu
{
    protected const string COORD_FORMAT             = &quot;%1.%2%3&quot;;
    protected const int SCALE_RULER_LINE_WIDTH      = 8;
    protected const int SCALE_RULER_NUM_SEGMENTS    = 10;
 
    protected bool                          m_WasChanged;
    protected bool                          m_HasCompass
    protected bool                          m_HasGPS
    protected bool                          m_IsOpenning;
    
    protected float                         m_ToolScaleCellSizeCanvasWidth;
    protected float                         m_ToolsContainerPos0[2];
 
    protected ref IngameHud                 m_Hud ;
    protected ref MapHandler                m_MapMenuHandler;
    protected ref MapWidget                 m_MapWidgetInstance;
    protected ref SizeToChild               m_LegendResizer;
 
    protected Widget                        m_MapToolsContainer;
    protected ImageWidget                   m_Images;
    protected Widget                        m_GPSMarker;
    protected ImageWidget                   m_GPSMarkerArrow;
    protected Widget                        m_UpperLegendContainer;
    protected Widget                        m_ToolsCompassBase;
    protected ImageWidget                   m_ToolsCompassArrow;
    protected TextWidget                    m_ToolsCompassAzimuth;
    protected TextWidget                    m_ToolsScaleContourText;
    protected TextWidget                    m_ToolsGPSElevationText;
    protected TextWidget                    m_ToolsGPSXText;
    protected TextWidget                    m_ToolsGPSYText;
    protected TextWidget                    m_ToolsScaleCellSizeText;
    protected CanvasWidget                  m_ToolsScaleCellSizeCanvas;
    protected ItemMap                       m_Map;
    protected Widget                        m_ToolbarPanel;
    //int                                   m_MarkerCount;
    
    protected ref MapNavigationBehaviour    m_MapNavigationBehaviour;
    
    void ~MapMenu()
    {
        if (g_Game && g_Game.GetMission())
        {
            g_Game.GetMission().GetOnInputPresetChanged().Remove(OnInputPresetChanged);
            g_Game.GetMission().GetOnInputDeviceChanged().Remove(OnInputDeviceChanged);
        }
