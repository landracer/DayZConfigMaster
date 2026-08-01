// Source: F:/Games/Dayz/scripts/4_world/classes/bloodyhands.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class BloodyHands
{
    string m_Materials[2];
    
    const int MATERIAL_TYPE_NORMAL = 0;
    const int MATERIAL_TYPE_BLOODY = 1;
 
    void BloodyHands( string normal_material, string bloody_material )
    {
        m_Materials[MATERIAL_TYPE_NORMAL] = normal_material;
        m_Materials[MATERIAL_TYPE_BLOODY] = bloody_material;
