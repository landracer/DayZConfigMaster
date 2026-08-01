// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/magazine/ammunitionpiles.c Source File
// Extracted from DayZ Code Explorer by Zeroy


class Ammunition_Base: Magazine_Base
{
    static ref map<string, float> m_AmmoWeightByBulletType = new map<string, float>();
    
    static float GetAmmoWeightByBulletType(string bulletType)
    {
        if (m_AmmoWeightByBulletType.Contains(bulletType))
        {
            return m_AmmoWeightByBulletType.Get(bulletType);
        }
        else
        {
            float ammoWeight;
            string ammoTypeName;
            g_Game.ConfigGetText( string.Format(&quot;CfgAmmo %1 spawnPileType&quot;, bulletType) , ammoTypeName);
            if (ammoTypeName)
                ammoWeight = g_Game.ConfigGetFloat(string.Format(&quot;CfgMagazines %1 weight&quot;, ammoTypeName));
            else
                ErrorEx(&quot;empty &#39;spawnPileType&#39; for bullet type:&quot; + bulletType);
            if (ammoWeight)
                m_AmmoWeightByBulletType.Insert(bulletType, ammoWeight);
            return ammoWeight;
        }
