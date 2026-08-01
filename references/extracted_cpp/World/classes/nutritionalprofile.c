// Source: F:/Games/Dayz/scripts/4_world/classes/nutritionalprofile.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class NutritionalProfile
{
    float m_Energy;
    float m_WaterContent;
    float m_NutritionalIndex;
    float m_FullnessIndex;
    float m_Toxicity;
    int m_LiquidType;
    int m_Agents;
    string m_LiquidClassname;
    float m_Digestibility;
    float m_AgentsPerDigest;
    
    void NutritionalProfile(float energy = 0.0, float water_content = 0.0, float nutritional_index = 0.0, float fullness_index = 0.0, float toxicity = 0.0, int agents = 0.0, float digestibility = 0.0)
    {
        m_Energy = energy;
        m_WaterContent = water_content;
        m_NutritionalIndex = nutritional_index;
        m_FullnessIndex = fullness_index;
        m_Toxicity = toxicity;
        m_Agents = agents;
        m_Digestibility = digestibility;
