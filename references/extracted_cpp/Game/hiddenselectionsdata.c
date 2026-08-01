// Source: F:/Games/Dayz/scripts/3_game/hiddenselectionsdata.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class HiddenSelectionsData
{
    ref array<string> m_HiddenSelections = new array<string>;
    ref array<string> m_HiddenSelectionsTextures = new array<string>;
    ref array<string> m_HiddenSelectionsMaterials = new array<string>;
    
    ref map<string, int> m_HiddenSelectionNameToIndex = new map<string, int>;
    
    void HiddenSelectionsData(string type)
    {
        g_Game.ConfigGetTextArray( string.Format(&quot;CfgVehicles %1 hiddenSelections&quot;,             type), m_HiddenSelections );
        g_Game.ConfigGetTextArray( string.Format(&quot;CfgVehicles %1 hiddenSelectionsTextures&quot;,     type), m_HiddenSelectionsTextures );
        g_Game.ConfigGetTextArray( string.Format(&quot;CfgVehicles %1 hiddenSelectionsMaterials&quot;, type), m_HiddenSelectionsMaterials );
        
        for (int i = 0; i < m_HiddenSelections.Count(); ++i)
            m_HiddenSelectionNameToIndex.Insert(m_HiddenSelections[i], i);
