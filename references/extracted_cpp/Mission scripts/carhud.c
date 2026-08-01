// Source: F:/Games/Dayz/scripts/5_mission/gui/vehicles/carhud.c Source File
// Extracted from DayZ Code Explorer by Zeroy

// Car HUD 
class CarHud : VehicleHudBase
{
    protected ImageWidget           m_VehicleRPMPointer;
    protected ImageWidget           m_VehicleRPMDial;
    protected ImageWidget           m_VehicleRPMRedline;
    protected ImageWidget           m_VehicleSpeedPointer;
    protected Widget                m_VehicleTemperatureIndicator;
    protected ImageWidget           m_VehicleTemperaturePointer;
    protected ImageWidget           m_VehicleTemperatureLight;
    protected ImageWidget           m_VehicleFuelPointer;
    protected ImageWidget           m_VehicleFuelLight;
    
    protected TextWidget            m_VehicleSpeedValue;
        
    protected ImageWidget           m_VehicleBatteryLight;
    protected ImageWidget           m_VehicleEngineLight;
    protected ImageWidget           m_VehicleOilLight;
    protected ImageWidget           m_VehicleHandBrakeLight;
    protected ImageWidget           m_VehicleWheelLight;
    
    protected bool                  m_VehicleDamageZoneHitEngineState;
    protected float                 m_VehicleDamageZonesHitTimer;
    
    protected CarScript             m_CurrentVehicle;
    
    protected int                   m_VehicleGearCount = -1;
    protected float                 m_TimeSinceLastEngineLightChange;
    protected bool                  m_VehicleHasCoolant;
        
    override void Init(Widget vehicleHudPanels)
    {       
        super.Init(vehicleHudPanels);
        
        m_VehiclePanel = g_Game.GetWorkspace().CreateWidgets(&quot;gui/layouts/day_z_hud_cars.layout&quot;, vehicleHudPanels);
        
        m_VehicleRPMPointer             = ImageWidget.Cast( m_VehiclePanel.FindAnyWidget(&quot;RPMPointer&quot;) );
        m_VehicleRPMDial                = ImageWidget.Cast( m_VehiclePanel.FindAnyWidget(&quot;RPMDial&quot;) );
        m_VehicleRPMRedline             = ImageWidget.Cast( m_VehiclePanel.FindAnyWidget(&quot;RPMDialRedline&quot;) );
        m_VehicleSpeedPointer           = ImageWidget.Cast( m_VehiclePanel.FindAnyWidget(&quot;SpeedPointer&quot;) );
        m_VehicleSpeedValue             = TextWidget.Cast( m_VehiclePanel.FindAnyWidget(&quot;SpeedValue&quot;) );
        
        m_VehicleCurrentGearValue       = TextWidget.Cast( m_VehiclePanel.FindAnyWidget(&quot;Current&quot;) );
        m_VehicleNextGearValue          = TextWidget.Cast( m_VehiclePanel.FindAnyWidget(&quot;Next&quot;) );
        m_VehiclePrevGearValue          = TextWidget.Cast( m_VehiclePanel.FindAnyWidget(&quot;Prev&quot;) );
        
        m_VehicleBatteryLight           = ImageWidget.Cast( m_VehiclePanel.FindAnyWidget(&quot;BatteryLight&quot;));
        m_VehicleEngineLight            = ImageWidget.Cast( m_VehiclePanel.FindAnyWidget(&quot;EngineLight&quot;));
        m_VehicleOilLight               = ImageWidget.Cast( m_VehiclePanel.FindAnyWidget(&quot;OilLight&quot;));
        m_VehicleHandBrakeLight         = ImageWidget.Cast( m_VehiclePanel.FindAnyWidget(&quot;HandBrakeLight&quot;));
        m_VehicleWheelLight             = ImageWidget.Cast( m_VehiclePanel.FindAnyWidget(&quot;WheelLight&quot;));
        
        m_VehicleTemperatureIndicator   = m_VehiclePanel.FindAnyWidget(&quot;TemperatureIndicator&quot;);
        m_VehicleTemperaturePointer     = ImageWidget.Cast(m_VehiclePanel.FindAnyWidget(&quot;TemperaturePointer&quot;));
        m_VehicleTemperatureLight       = ImageWidget.Cast(m_VehiclePanel.FindAnyWidget(&quot;TemperatureLight&quot;));
        m_VehicleFuelPointer            = ImageWidget.Cast(m_VehiclePanel.FindAnyWidget(&quot;FuelPointer&quot;));
        m_VehicleFuelLight              = ImageWidget.Cast(m_VehiclePanel.FindAnyWidget(&quot;FuelLight&quot;));
        
        m_VehicleGearTable.Set(-1, &quot;&quot;);
        m_VehicleGearTable.Set(CarGear.REVERSE, &quot;R&quot;);
        m_VehicleGearTable.Set(CarGear.NEUTRAL, &quot;N&quot;);
        m_VehicleGearTable.Set(CarGear.FIRST, &quot;1&quot;);
        m_VehicleGearTable.Set(CarGear.SECOND, &quot;2&quot;);
        m_VehicleGearTable.Set(CarGear.THIRD, &quot;3&quot;);
        m_VehicleGearTable.Set(CarGear.FOURTH, &quot;4&quot;);
        m_VehicleGearTable.Set(CarGear.FIFTH, &quot;5&quot;);
        m_VehicleGearTable.Set(CarGear.SIXTH, &quot;6&quot;);
        m_VehicleGearTable.Set(CarGear.SEVENTH, &quot;7&quot;);
        m_VehicleGearTable.Set(CarGear.EIGTH, &quot;8&quot;);
        
        m_VehicleGearTableAuto.Set(-1, &quot;&quot;);
        m_VehicleGearTableAuto.Set(CarAutomaticGearboxMode.R, &quot;R&quot;);
        m_VehicleGearTableAuto.Set(CarAutomaticGearboxMode.N, &quot;N&quot;);
        m_VehicleGearTableAuto.Set(CarAutomaticGearboxMode.D, &quot;D&quot;);
