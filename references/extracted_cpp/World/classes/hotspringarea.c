// Source: F:/Games/Dayz/scripts/4_world/classes/contaminatedarea/hotspringarea.c Source File
// Extracted from DayZ Code Explorer by Zeroy


class HotSpringArea : EffectArea
{
    protected ref UniversalTemperatureSource m_UTSource;
    protected ref UniversalTemperatureSourceSettings m_UTSSettings;
    protected ref UniversalTemperatureSourceLambdaConstant m_UTSLConstant;
    
    override void DeferredInit()
    {
        super.DeferredInit();
        
        InitZone();     
