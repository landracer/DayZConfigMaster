// Source: F:/Games/Dayz/scripts/5_mission/gui/vehicles/vehiclehudbase.c Source File
// Extracted from DayZ Code Explorer by Zeroy

// Vehicle HUD UI base class
class VehicleHudBase : Managed
{
    protected Widget        m_VehiclePanel;
    protected TextWidget    m_VehicleCurrentGearValue;
    protected TextWidget    m_VehicleNextGearValue;
    protected TextWidget    m_VehiclePrevGearValue;
    
    protected ref map<int,string>   m_VehicleGearTable;
    protected ref map<int,string>   m_VehicleGearTableAuto;
    
    void VehicleHudBase();
    
    void ~VehicleHudBase();
    
    // Spawn 
    void Init(Widget vehicleHudPanels)
    {
        m_VehicleGearTable              = new map<int, string>;
        m_VehicleGearTableAuto          = new map<int, string>;
