// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/torch.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class FlammableBase : ItemBase
{
    const float PARAM_FULL_HEAT_RADIUS              = 0.5;
    const float PARAM_HEAT_RADIUS                   = 1.0;
    const int   TIMER_HEATING_UPDATE_INTERVAL       = 1;        
    const float PARAM_MAX_TRANSFERED_TEMPERATURE    = 5;        
    const float PARAM_DRY_MODIFIER                  = 1.125;
    
    void FlammableBase()
    {
        Init();
