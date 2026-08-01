// Source: F:/Games/Dayz/scripts/3_game/global/ammotypes.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class AmmoTypesAPI
{
    static protected ref map<string, ref map<string, int>> m_TypeToSurfaceParticleIDMap;
    
    private void AmmoTypesAPI() {}
    private void ~AmmoTypesAPI() {}
    
    static proto bool Exists(string ammoType);
    static proto bool GetAmmoType(string magazineType, out string ammoType);
    
    static bool MagazineTypeToAmmoType(string magazineType, out string ammoType)
    {
        if ( !AmmoTypesAPI.Exists(magazineType) )
        {
            if ( !AmmoTypesAPI.GetAmmoType(magazineType, ammoType) )
            {
                ErrorEx(string.Format(&quot;%1 is not a magazineType or ammoType or has no default &#39;ammo&#39; set up in config.&quot;, magazineType));
                return false;
            }
            return true;
        }
        else
        {
            // This is already an ammoType
            ammoType = magazineType;
            return true;
        }
