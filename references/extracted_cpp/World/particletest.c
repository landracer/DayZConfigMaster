// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/particletest.c Source File
// Extracted from DayZ Code Explorer by Zeroy

// Particle test for Maxman
 
class ParticleTest extends ItemBase
{
    protected int   PARTICLE_PATH;
    protected Particle  m_Particle;
    
    // Constructor
    void ParticleTest()
    {
        if ( !g_Game.IsServer()  ||  !g_Game.IsMultiplayer() ) // Client side
        {
            string path = ParticleList.GetPathToParticles();
            
            
            
            
            
            // Enter particle ID to play when ParticleTest spawns
            PARTICLE_PATH = ParticleList.DEBUG_DOT; 
            
            // Alternatively, uncomment the second line and enter particle filename without *.ptc suffix instead. Example: &quot;menu_engine_fire&quot; 
            string particle_filename = &quot;menu_engine_fire&quot;;
            //PARTICLE_PATH = ParticleList.GetParticleID( path + particle_filename );
            
            
            
            
            
            m_Particle = ParticleManager.GetInstance().PlayOnObject( PARTICLE_PATH, this, GetPosition());
        }
