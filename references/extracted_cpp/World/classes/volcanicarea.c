// Source: F:/Games/Dayz/scripts/4_world/classes/contaminatedarea/volcanicarea.c Source File
// Extracted from DayZ Code Explorer by Zeroy


class VolcanicArea : EffectArea
{
    protected ref UniversalTemperatureSource m_UTSource;
    protected ref UniversalTemperatureSourceSettings m_UTSSettings;
    protected ref UniversalTemperatureSourceLambdaConstant m_UTSLConstant;
    
    override void DeferredInit()
    {
        super.DeferredInit();
        
        InitZone();     
