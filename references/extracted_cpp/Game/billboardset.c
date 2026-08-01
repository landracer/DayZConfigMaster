// Source: F:/Games/Dayz/scripts/3_game/billboardset.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class BillboardSetHandler
{
    protected bool                          m_BillboardSetIndex     = -1;
    protected ref array<ref BillboardSet>   m_BillboardSets;
    protected static const string           ROOT_CLASS = &quot;BillboardPresets&quot;;
    protected static int                    m_SetIndexCached = -1;//once we resolve the name into an index, we cache it(this disallows dynamic index swapping during mission&#39;s runtime)
    
    string GetTextureByType(string type)
    {
        if (m_BillboardSetIndex == -1)
            return &quot;&quot;;
        BillboardSet bbset = m_BillboardSets.Get(m_BillboardSetIndex);
        return bbset.GetTextureByType(type);
