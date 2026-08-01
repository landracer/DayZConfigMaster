// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/basebuildingbase/watchtower.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class Watchtower extends BaseBuildingBase
{   
    typename ATTACHMENT_BARBED_WIRE         = BarbedWire;
    typename ATTACHMENT_CAMONET             = CamoNet;
    
    const float MAX_FLOOR_VERTICAL_DISTANCE         = 0.5;
    
    const float MIN_ACTION_DETECTION_ANGLE_RAD      = 0.35;     //0.35 RAD = 20 DEG
    const float MAX_ACTION_DETECTION_DISTANCE       = 2.0;      //meters
    
    static const string BASE_VIEW_NAME              = &quot;level_&quot;;
    static const string BASE_WALL_NAME              = &quot;_wall_&quot;;
    static const string BASE_ROOF_NAME              = &quot;_roof&quot;;
    static const int    MAX_WATCHTOWER_FLOORS       = 3;
    static const int    MAX_WATCHTOWER_WALLS        = 3;
    
    void Watchtower()
    {
