// Source: F:/Games/Dayz/scripts/4_world/entities/vehicles/inheritedcars/truck_02.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class Truck_02 extends CarScript
{
    protected ref UniversalTemperatureSource m_UTSource;
    protected ref UniversalTemperatureSourceSettings m_UTSSettings;
    protected ref UniversalTemperatureSourceLambdaEngine m_UTSLEngine;
 
    void Truck_02()
    {
        //m_dmgContactCoef = 0.018;
        m_enginePtcPos = &quot;0 1.346 2.205&quot;;
