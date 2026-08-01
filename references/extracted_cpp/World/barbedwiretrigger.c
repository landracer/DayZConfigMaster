// Source: F:/Games/Dayz/scripts/4_world/entities/scriptedentities/triggers/barbedwiretrigger.c Source File
// Extracted from DayZ Code Explorer by Zeroy


class BarbedWireTrigger : Trigger
{
    ItemBase            m_ParentBarbedWire;
    const static int    SOUNDS_COLLISION_COUNT                      = 4;
    const static int    SOUNDS_SHOCK_COUNT                          = 4;
    const static string m_SoundsCollision[SOUNDS_COLLISION_COUNT]   = {&quot;barbedFenceCollision1&quot;, &quot;barbedFenceCollision2&quot;, &quot;barbedFenceCollision3&quot;, &quot;barbedFenceCollision4&quot;};
    const static string m_SoundsShock[SOUNDS_SHOCK_COUNT]           = {&quot;electricFenceShock1&quot;, &quot;electricFenceShock2&quot;, &quot;electricFenceShock3&quot;, &quot;electricFenceShock4&quot;};
 
    // When a player / AI touches the Barbed Wire
    override void OnEnter( Object obj )
    {
        if ( g_Game.IsServer() )
        {
            if ( m_ParentBarbedWire )
            {
                if ( obj.IsInherited(PlayerBase) )
                {
                    // When a player touches the barbed wire
                    string cfg = &quot;CfgVehicles BarbedWire barbedWireShockEnergyConsumption&quot;;
                    float needed_energy = g_Game.ConfigGetFloat(cfg);
                    bool energy_consumed = m_ParentBarbedWire.GetCompEM().ConsumeEnergy(needed_energy);
                    PlayerBase player = PlayerBase.Cast( obj );
                    
                    if ( energy_consumed )
                    {
                        // TO DO: 
                        //      -Do electrical damage.
                        //      -Cause bleeding?
                        //      -Do some damage!
                        
                        player.MessageImportant( &quot;*SCRATCH and ELECTROCUTION*&quot; );
                        
                        // Play sound
                        SoundCollision();
                        SoundElectricShock();
                    }
                    else
                    {
                        // TO DO: 
                        //      -Cause bleeding?
                        //      -Do some damage!
                        
                        player.MessageImportant( &quot;*SCRATCH*&quot; );
                        
                        // Play sound
                        SoundCollision();
                    }
                }
                else
                {
                    // When an AI Agent touches the barbed wire
                    if ( obj.IsInherited(ManBase) )
                    {
                        ManBase AI_unit = ManBase.Cast( obj );
                        AI_unit.SetHealth(&quot;&quot;, &quot;&quot;, 0);
                    }
                }
            }
        }
