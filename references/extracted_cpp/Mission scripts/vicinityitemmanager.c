// Source: F:/Games/Dayz/scripts/5_mission/gui/inventorynew/vicinityitemmanager.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class VicinityItemManager
{
    private const float UPDATE_FREQUENCY                = 0.25;
    private const float VICINITY_DISTANCE               = 0.5;
    private const float VICINITY_ACTOR_DISTANCE         = 2.0;
    private const float VICINITY_LARGE_ACTOR_DISTANCE   = 3.0;
    private const float VICINITY_CONE_DISTANCE          = 2.0;
    private const float VICINITY_CONE_REACH_DISTANCE    = 2.0;
    private const float VICINITY_CONE_ANGLE             = 30;
    private const float VICINITY_CONE_RADIANS           = 0.5;
    private const string CE_CENTER                      = &quot;ce_center&quot;;
    private const float HEIGHT_OFFSET                   = 0.2;
    private const int OBJECT_OBSTRUCTION_WEIGHT         = 10000; //in grams
    private const float CONE_HEIGHT_MIN                 = -0.5;
    private const float CONE_HEIGHT_MAX                 = 3.0;
 
    private ref array<EntityAI> m_VicinityItems         = new array<EntityAI>();
    private ref array<CargoBase> m_VicinityCargos       = new array<CargoBase>();
    private float m_RefreshCounter;
    private static ref VicinityItemManager s_Instance;
    
    static VicinityItemManager GetInstance ()
    {
        if (!s_Instance)
            s_Instance = new VicinityItemManager();
 
        return s_Instance;
