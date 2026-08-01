// Source: F:/Games/Dayz/scripts/3_game/systems/universaltemperaturesource/universaltemperaturesource.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class UniversalTemperatureSourceSettings
{
    float m_UpdateInterval      = 1.0;          
    float m_TemperatureItemCap  = GameConstants.ITEM_TEMPERATURE_NEUTRAL_ZONE_MIDDLE; 
    float m_TemperatureItemCoef = GameConstants.TEMP_COEF_UTS; 
    float m_TemperatureCap      = float.MAX;    
    float m_RangeFull           = 1;            
    float m_RangeMax            = 2;            
    float m_ItemDryModifier     = 1.5;
    
    bool m_EnableOnTemperatureControl       = false;    
    float m_ActiveTemperatureThreshold      = 0.0;      
    float m_InactiveTemperatureThreshold    = 0.0;      
    
    bool m_Updateable           = false;        
    bool m_ManualUpdate         = false;        
    bool m_IsWorldOverriden     = true;         
 
    vector m_Position           = vector.Zero;
    EntityAI m_Parent           = null;         

    bool m_AffectStat           = false;
    float m_TemperatureMin      = 0;
    float m_TemperatureMax      = 100;
