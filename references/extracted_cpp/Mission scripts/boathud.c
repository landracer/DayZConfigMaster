// Source: F:/Games/Dayz/scripts/5_mission/gui/vehicles/boathud.c Source File
// Extracted from DayZ Code Explorer by Zeroy

// Boat HUD UI
class BoatHud : VehicleHudBase
{   
    protected const float       ENGINE_BLINK_DURATION = 3;
    
    protected ImageWidget   m_SpeedPointer;
    protected ImageWidget   m_EngineLight;
    protected ImageWidget   m_FuelLight;
        
    protected bool          m_HasEngine;
    protected int           m_EngagedGear;
    protected int           m_EngineHealthLevel;
    protected float         m_FuelLevel;
    protected float         m_TimeSinceEngineHit;
    protected float         m_TimeSinceEngineBlink;
    protected BoatScript    m_CurrentVehicle;
    
    protected ref WidgetFadeTimer   m_FadeTimer = new WidgetFadeTimer();
    
    override void Init(Widget vehicleHudPanels)
    {   
        super.Init(vehicleHudPanels);
        
        m_VehiclePanel = g_Game.GetWorkspace().CreateWidgets(&quot;gui/layouts/day_z_hud_boats.layout&quot;, vehicleHudPanels);
        
        m_SpeedPointer              = ImageWidget.Cast( m_VehiclePanel.FindAnyWidget(&quot;SpeedPointer&quot;) );
        m_EngineLight               = ImageWidget.Cast( m_VehiclePanel.FindAnyWidget(&quot;EngineLight&quot;));
        m_FuelLight                 = ImageWidget.Cast( m_VehiclePanel.FindAnyWidget(&quot;FuelLight&quot;));
        
        m_VehicleCurrentGearValue   = TextWidget.Cast( m_VehiclePanel.FindAnyWidget(&quot;Current&quot;) );
        m_VehicleNextGearValue      = TextWidget.Cast( m_VehiclePanel.FindAnyWidget(&quot;Next&quot;) );
        m_VehiclePrevGearValue      = TextWidget.Cast( m_VehiclePanel.FindAnyWidget(&quot;Prev&quot;) );
