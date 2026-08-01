// Source: F:/Games/Dayz/scripts/4_world/classes/lifespanlevel.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class LifespanLevel
{
    protected int m_Level;
    protected float m_Threshold;
    protected string m_TextureName;
    protected string m_MaterialName;
 
 
    void LifespanLevel( int level, float threshold, string texture_name, string material_name )
    {
        m_Level = level;
        m_Threshold = threshold;
        m_TextureName = texture_name;
        m_MaterialName = material_name;
