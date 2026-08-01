// Source: F:/Games/Dayz/scripts/3_game/ammocamparams.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class AmmoCamParams
{
    void Load(string ammoType)
    {
        string cfgPath = &quot;CfgAmmo &quot; + ammoType + &quot; Effects&quot; + &quot; CameraShake &quot;;
        
        m_Radius                = g_Game.ConfigGetFloat(cfgPath + &quot;radius&quot;);
        m_Strength              = g_Game.ConfigGetFloat(cfgPath + &quot;strength&quot;);
        m_ModifierClose         = g_Game.ConfigGetFloat(cfgPath + &quot;modifierClose&quot;);
        m_ModifierFar           = g_Game.ConfigGetFloat(cfgPath + &quot;modifierFar&quot;);
