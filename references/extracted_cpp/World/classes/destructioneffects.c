// Source: F:/Games/Dayz/scripts/4_world/classes/destructioneffects/destructioneffects.c Source File
// Extracted from DayZ Code Explorer by Zeroy

/*
class TestDestructionEffect : DestructionEffectBase
{
    ref AreaDamageManager m_AreaManager;
    override void Init()
    {
        m_EntityIsTakeable = true;
        
        m_ParticlePersistent = ParticleList.GRENADE_M18_GREEN_LOOP;
        m_ParticleOneTime = ParticleList.GRENADE_M84;
        
        m_SoundSetOneTime = &quot;AlarmClock_Destroyed_SoundSet&quot;;
        m_SoundSetPersistent= &quot;KitchenTimer_Ticking_Loop_SoundSet&quot;;
        
        m_ReplaceWithEntity = &quot;&quot;;
        m_KeepHealthOnReplace = true;
        m_ReplaceDelay = 1000;
        
        m_HasExplosionDamage = true;
        m_AmmoType = &quot;&quot;;
        m_DamageType = 0;
    }
    
    override void OnEntityDestroyedOneTimeClient(EntityAI entity, int oldLevel, string zone)
    {
    }
    
    override void OnEntityDestroyedOneTimeServer(EntityAI entity, int oldLevel, string zone)
    {
    }
    
    override void OnEntityDestroyedPersistentClient(EntityAI entity, string zone)
    {
    }
    
    override void OnEntityDestroyedPersistentServer(EntityAI entity, string zone)
    {
        AreaDamageLoopedDeferred_NoVehicle area_damage = new AreaDamageLoopedDeferred_NoVehicle( m_Entity );
        area_damage.SetDamageComponentType(AreaDamageComponentTypes.HITZONE);
        
        vector min, max;
        m_Entity.GetBounds(min, max);
        vector center = m_Entity.GetPosition();
 
        vector orientation = m_Entity.GetOrientation();
        
        area_damage.SetExtents( &quot;-1 -1 -1&quot;, &quot;1 1 1&quot; );
        area_damage.SetAreaPosition( center );
        area_damage.SetAreaOrientation( orientation );
        area_damage.SetLoopInterval( 1.0 );
        area_damage.SetDeferDuration( 0.2 );
        area_damage.SetHitZones( { &quot;Torso&quot;,&quot;LeftHand&quot;,&quot;LeftLeg&quot;,&quot;LeftFoot&quot;,&quot;RightHand&quot;,&quot;RightLeg&quot;,&quot;RightFoot&quot; } );
        area_damage.SetAmmoName( &quot;BarbedWireHit&quot; );
        area_damage.Spawn();
        
        m_AreaManager = area_damage;
        
    }
}
 
//--------------------------------------------------------------------
 
 
 
class TestDestructionEffect2 : DestructionEffectBase
{
    override void Init()
    {
        m_EntityIsTakeable = false;
        
        
        m_ParticlePersistent = ParticleList.GRENADE_M18_GREEN_LOOP;
        m_ParticleOneTime = ParticleList.GRENADE_M84;
        
        m_SoundSetOneTime = &quot;AlarmClock_Destroyed_SoundSet&quot;;
        m_SoundSetPersistent= &quot;KitchenTimer_Ticking_Loop_SoundSet&quot;;
        
        m_ReplaceWithEntity = &quot;&quot;;
        m_KeepHealthOnReplace = true;
        m_ReplaceDelay = 1000;
        
        m_HasExplosionDamage = true;
        m_AmmoType = &quot;LandFuelFeed_Ammo&quot;;
        m_DamageType = DamageType.EXPLOSION;
    }
    
    
    override void OnEntityDestroyedOneTimeClient(EntityAI entity, int oldLevel, string zone)
    {
    }
    
    override void OnEntityDestroyedOneTimeServer(EntityAI entity, int oldLevel, string zone)
    {
    }
    
    override void OnEntityDestroyedPersistentClient(EntityAI entity, string zone)
    {
    }
    
    override void OnEntityDestroyedPersistentServer(EntityAI entity, string zone)
    {
    }
    
    override void OnExplosionEffects(Object source, Object directHit, int componentIndex, string surface, vector pos, vector surfNormal, float energyFactor, float explosionFactor, bool isWater, string ammoType)
    {
        if ( !g_Game.IsDedicatedServer() )
        {   
            vector n = surfNormal.VectorToAngles() + &quot;0 90 0&quot;;
            Particle p1 = ParticleManager.GetInstance().PlayInWorld(ParticleList.SMOKE_GENERIC_WRECK, pos);
            p1.SetOrientation(n);
        
            Particle p2 = ParticleManager.GetInstance().PlayInWorld(ParticleList.EXPLOSION_LANDMINE, pos);
            p2.SetOrientation(n);
 
            Particle p3 = ParticleManager.GetInstance().PlayInWorld(ParticleList.IMPACT_METAL_RICOCHET, pos);
            p3.SetOrientation(n);
        
            Particle p4 = ParticleManager.GetInstance().PlayInWorld(ParticleList.IMPACT_GRAVEL_RICOCHET, pos);
            p4.SetOrientation(n);
        }
    }
}
*/
class DestructionEffectGasCanister : DestructionEffectBase
{
    override void Init()
    {
        m_EntityIsTakeable = true;
        
        
        m_ParticlePersistent = ParticleList.NONE;
        m_ParticleOneTime = ParticleList.RGD5;
        
        m_SoundSetOneTime = &quot;&quot;;
        m_SoundSetPersistent= &quot;&quot;;
        
        m_ReplaceWithEntity = &quot;&quot;;
        m_KeepHealthOnReplace = true;
        m_ReplaceDelay = 1000;
        
        m_HasExplosionDamage = true;
        m_AmmoType = &quot;GasCanister_Ammo&quot;;
        m_DamageType = DamageType.EXPLOSION;
