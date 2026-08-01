// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/chemlight.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class Chemlight_ColorBase : ItemBase
{
    string                  m_DefaultMaterial;
    string                  m_GlowMaterial;
    
    ChemlightLight          m_Light;
    
    private int             m_Efficiency0To10; // Synchronized variable
    static private float    m_EfficiencyDecayStart = 0.05; // At this % of maximum energy the output of the light starts to weaken.
    
    float GetEfficiency0To1()
    {
        return m_Efficiency0To10 / 10;
