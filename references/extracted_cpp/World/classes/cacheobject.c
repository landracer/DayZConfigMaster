// Source: F:/Games/Dayz/scripts/4_world/classes/recipes/cacheobject.c Source File
// Extracted from DayZ Code Explorer by Zeroy

 
class RecipeCacheData
{
    int m_Mask;
    int m_BitCount;
    
    void RecipeCacheData(int mask)
    {
        SetMask(mask);
