// Source: F:/Games/Dayz/scripts/4_world/static/misceffects.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class MiscEffects : Managed
{
    protected static const int VEGE_COLLISION_COOLDOWN = 3000; // ms 
    protected static ref map <Object, float> m_VegeCollideColldowns = new map <Object, float>();
    
    // Called by individual clients when player collides with a vegetation
    static void PlayVegetationCollideParticles(Object object, DayZPlayerImplement player)
    {
        UpdateVegeCollisionCooldowns();
        if (m_VegeCollideColldowns.Get(object))
            return;
        
        int particleID;
        string particleName;
        string configPath = &quot;CfgNonAIVehicles &quot; + object.GetType() + &quot; collisionParticle&quot;;
        g_Game.ConfigGetText(configPath, particleName);
        if (particleName == string.Empty)   // no ptc assigned
            return;
        
        particleID = ParticleList.GetParticleIDByName(particleName);
        vector playerPos = player.GetPosition();
        vector ptcPos = playerPos - object.GetPosition();
        
        ptcPos[0] = ptcPos[0] * Math.RandomFloat(0, 0.5); // randomize
        ptcPos[2] = ptcPos[2] * Math.RandomFloat(0, 0.5);
        
        ptcPos = playerPos - ptcPos;
        ptcPos[1] = playerPos[1] + Math.RandomFloat(0.5, 1.5);
        
        ParticleManager.GetInstance().PlayInWorld(particleID, ptcPos);
        
        m_VegeCollideColldowns.Insert(object, GetWorldTime());
