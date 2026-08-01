// Source: F:/Games/Dayz/scripts/4_world/classes/playerstomach.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class StomachItem
{
    ref NutritionalProfile m_Profile;
    float m_Amount;
    int m_FoodStage;
    //bool m_IsLiquid;
    string m_ClassName;
    int m_Agents;
    protected float m_Temperature;
    
    void StomachItem(string class_name, float amount, NutritionalProfile profile,int foodstage, int agents, float temperature)
    {
        m_Amount = amount;
        m_Profile = profile;
        //m_IsLiquid = is_liquid;
        m_FoodStage = foodstage;
        m_ClassName = class_name;
        m_Agents = agents;
        m_Temperature = temperature;
