// Source: F:/Games/Dayz/scripts/3_game/ammoeffects.c Source File
// Extracted from DayZ Code Explorer by Zeroy


class AmmoEffects
{   
    static ref map<string, int> m_AmmoParticles;
    
    static ref map<string, typename> m_AmmoEffects;
    
    
    
    
    static int GetAmmoParticleID(string ammoType)
    {
        int particleID;
        
        // Search for it in the static map
        if ( !m_AmmoParticles.Find(ammoType, particleID) )
        {
            // Load it in when we can&#39;t find it
            string particleFileName;
            g_Game.ConfigGetText(string.Format(&quot;cfgAmmo %1 particle&quot;, ammoType), particleFileName);
            
            // If we found a valid entry, try looking for it in ParticleList
            if ( particleFileName != &quot;&quot; )
            {
                particleID = ParticleList.GetParticleIDByName(particleFileName);
            }
            
            // Store it for next search
            m_AmmoParticles.Insert(ammoType, particleID);
        }
        
        return particleID;
