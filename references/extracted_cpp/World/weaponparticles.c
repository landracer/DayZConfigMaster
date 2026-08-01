// Source: F:/Games/Dayz/scripts/4_world/entities/weaponparticles.c Source File
// Extracted from DayZ Code Explorer by Zeroy

/*
    Author: Boris Vacula
    For documentation go to: DayZ Confluence -> How-to articles -> Weapon muzzle flash particle system configuration
    This system plays effect(s) on any weapon that is fired/jammed/ruined/...
*/
 
class WeaponParticlesBase // This class represents every particle effect you see in config within OnFire or OnOverheating events
{
    bool            m_IlluminateWorld;
    bool            m_IgnoreIfSuppressed;
    bool            m_OnlyIfBoltIsOpen;
    int             m_MuzzleIndex;
    int             m_OverrideParticle;
    int             m_OnlyWithinHealthLabelMin;
    int             m_OnlyWithinHealthLabelMax;
    float           m_OnlyWithinOverheatLimitsMin;
    float           m_OnlyWithinOverheatLimitsMax;
    float           m_OnlyWithinRainLimitsMin;
    float           m_OnlyWithinRainLimitsMax;
    string          m_OverrideDirectionPoint;
    string          m_OnlyIfBulletIs;
    string          m_OnlyIfWeaponIs;
    string          m_OverridePoint;
    vector          m_OverrideDirectionVector;
    vector          m_PositionOffset;
    
    string          m_Name;
    
    //======================================
    //          PRELOAD EVERYTHING
    //======================================
    
    void WeaponParticlesBase(ItemBase muzzle_owner, string config_OnFire_entry)
    {       
        m_Name = config_OnFire_entry;
        
        // ignoreIfSuppressed
        m_IgnoreIfSuppressed = g_Game.ConfigGetFloat(string.Format(&quot;%1 ignoreIfSuppressed&quot;, m_Name));
        
        // onlyIfBoltIsOpen
        m_OnlyIfBoltIsOpen = g_Game.ConfigGetFloat(string.Format(&quot;%1 onlyIfBoltIsOpen&quot;, m_Name));
        
        // illuminateWorld
        m_IlluminateWorld = g_Game.ConfigGetFloat(string.Format(&quot;%1 illuminateWorld&quot;, m_Name));
        
        m_MuzzleIndex = -1;
        if (g_Game.ConfigIsExisting(string.Format(&quot;%1 muzzleIndex&quot;, m_Name)))
        {
            m_MuzzleIndex = g_Game.ConfigGetInt(string.Format(&quot;%1 muzzleIndex&quot;, m_Name));
        }
        
        // onlyIfWeaponIs
        m_OnlyIfWeaponIs = &quot;&quot;;
        g_Game.ConfigGetText(string.Format(&quot;%1 onlyIfWeaponIs&quot;, m_Name), m_OnlyIfWeaponIs);
        
        // onlyIfBulletIs
        m_OnlyIfBulletIs = &quot;&quot;;
        g_Game.ConfigGetText(string.Format(&quot;%1 onlyIfBulletIs&quot;, m_Name), m_OnlyIfBulletIs);
        
        // onlyWithinHealthLabel[]
        array<float> health_limit = new array<float>;
        g_Game.ConfigGetFloatArray(string.Format(&quot;%1 onlyWithinHealthLabel&quot;, m_Name), health_limit);
        
        if (health_limit.Count() == 2)
        {
            m_OnlyWithinHealthLabelMin = health_limit.Get(0);
            m_OnlyWithinHealthLabelMax = health_limit.Get(1);
        }
        else
        {
            // Disable this filter
            m_OnlyWithinHealthLabelMin = -1;
            m_OnlyWithinHealthLabelMax = 99;
        }
        
        // onlyWithinOverheatLimits[]
        array<float> overheat_limit = new array<float>;
        g_Game.ConfigGetFloatArray(string.Format(&quot;%1 onlyWithinOverheatLimits&quot;, m_Name), overheat_limit);
        
        if (overheat_limit.Count() == 2)
        {
            m_OnlyWithinOverheatLimitsMin = overheat_limit.Get(0);
            m_OnlyWithinOverheatLimitsMax = overheat_limit.Get(1);
        }
        else
        {
            // Disable this filter
            m_OnlyWithinOverheatLimitsMin = -1;
            m_OnlyWithinOverheatLimitsMax = 2;
        }
        
        // onlyWithinRainLimits[]
        array<float> rain_limit = new array<float>;
        g_Game.ConfigGetFloatArray(string.Format(&quot;%1 onlyWithinRainLimits&quot;, m_Name), rain_limit);
        
        if (rain_limit.Count() == 2)
        {
            m_OnlyWithinRainLimitsMin = rain_limit.Get(0);
            m_OnlyWithinRainLimitsMax = rain_limit.Get(1);
        }
        else
        {
            // Disable this filter
            m_OnlyWithinRainLimitsMin = -1;
            m_OnlyWithinRainLimitsMax = 2;
        }
        
        // overridePoint
        m_OverridePoint = &quot;&quot;;
        g_Game.ConfigGetText(string.Format(&quot;%1 overridePoint&quot;, m_Name), m_OverridePoint);
        
        if (m_OverridePoint == &quot;&quot;)
            m_OverridePoint = &quot;Usti hlavne&quot;; // default memory point name
        
        // overrideParticle
        string particle_name = &quot;&quot;;
        g_Game.ConfigGetText( string.Format(&quot;%1 overrideParticle&quot;, m_Name), particle_name);
        
        if (particle_name != &quot;&quot;)
        {
            m_OverrideParticle = ParticleList.GetParticleIDByName(particle_name);
        }
        else
        {
            m_OverrideParticle = -1;
            ErrorEx(string.Format(&quot;&#39;%1&#39; does not contain a definition for &#39;overrideparticle&#39;&quot;,
                config_OnFire_entry), ErrorExSeverity.INFO);
        }
        
        // overrideDirectionPoint
        m_OverrideDirectionPoint = &quot;&quot;;
        g_Game.ConfigGetText(string.Format(&quot;%1 overrideDirectionPoint&quot;, m_Name), m_OverrideDirectionPoint);
        
        if (m_OverrideDirectionPoint == &quot;&quot;)
        {
            // overrideDirectionVector
            vector test_ori = g_Game.ConfigGetVector(string.Format(&quot;%1 overrideDirectionVector&quot;, m_Name));
            
            if (test_ori != vector.Zero)
            {
                m_OverrideDirectionVector = test_ori;
            }
        }
        
        // positionOffset[]
        array<float> v = new array<float>;
        g_Game.ConfigGetFloatArray(string.Format(&quot;%1 positionOffset&quot;, m_Name), v);
        
        if (v.Count() == 3)
        {
            float v1 = v.Get(0);
            float v2 = v.Get(1);
            float v3 = v.Get(2);
            m_PositionOffset = Vector(v1, v2, v3);
        }
