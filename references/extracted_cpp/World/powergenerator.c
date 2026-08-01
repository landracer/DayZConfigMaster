// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/powergenerator.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class PowerGeneratorBase extends ItemBase
{
    float                       m_Fuel;
    private static float        m_FuelTankCapacity; // Capacity in ml.
    private static float        m_FuelToEnergyRatio; // Conversion ratio of 1 ml of fuel to X Energy
    private int                 m_FuelPercentage;
    
    protected const float       LOW_ENERGY_FUEL_PERCENTAGE = 20; // how much % of fuel has to remain to trigger low fuel state 
    static const string         START_SOUND = &quot;powerGeneratorTurnOn_SoundSet&quot;;
    static const string         LOOP_SOUND = &quot;powerGeneratorLoop_SoundSet&quot;;
    protected const string      LOOP_LOW_FUEL_SOUND = &quot;powerGenerator_low_Fuel_Loop_SoundSet&quot;;
    static const string         STOP_SOUND = &quot;powerGeneratorTurnOff_SoundSet&quot;;
    
    protected bool              m_IsLowEnergy; 
    protected EffectSound       m_EngineLoop;
    protected EffectSound       m_EngineStart;
    protected EffectSound       m_EngineStop;
    ref Timer                   m_SoundLoopStartTimer;
    ref protected Effect        m_Smoke;
    
    ItemBase    m_SparkPlug;    
    
    protected ref UniversalTemperatureSource m_UTSource;
    protected ref UniversalTemperatureSourceSettings m_UTSSettings;
    protected ref UniversalTemperatureSourceLambdaConstant m_UTSLEngine;
    
    // Constructor
    void PowerGeneratorBase()   
    {
        SetEventMask(EntityEvent.INIT); // Enable EOnInit event
        
        m_FuelPercentage = 50;
        RegisterNetSyncVariableInt(&quot;m_FuelPercentage&quot;);
