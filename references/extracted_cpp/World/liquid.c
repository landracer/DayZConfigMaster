// Source: F:/Games/Dayz/scripts/4_world/static/liquid.c Source File
// Extracted from DayZ Code Explorer by Zeroy

//extendable!
class LiquidInfo
{
    string m_LiquidClassName;
    string m_LiquidDisplayName;
    int m_LiquidType;
    float m_TemperatureLiquidFreezeThreshold = float.LOWEST;
    float m_TemperatureLiquidThawThreshold = float.LOWEST;
    float m_TemperatureLiquidBoilThreshold = Cooking.LIQUID_BOILING_POINT;
    float m_Flammability;
    
    ref NutritionalProfile m_NutriProfile;
    
    void LiquidInfo(string className, NutritionalProfile profile)
    {
        m_NutriProfile = profile;
        Init(className);
