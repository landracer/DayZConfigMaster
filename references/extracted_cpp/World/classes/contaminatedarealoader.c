// Source: F:/Games/Dayz/scripts/4_world/classes/contaminatedarea/contaminatedarealoader.c Source File
// Extracted from DayZ Code Explorer by Zeroy

// This will be used to parse and load contaminated area related data
class EffectAreaLoader
{
    private static string m_Path = &quot;$mission:cfgeffectarea.json&quot;;
 
    static void CreateZones()
    {
        JsonDataContaminatedAreas effectAreaData;
        
        // We confirm the contaminated area configuration file exists in mission folder
        if (!FileExist(m_Path))
        {
            // We fallback to check in data and notify user file was not found in mission
            PrintToRPT(&quot;[WARNING] :: [EffectAreaLoader CreateZones] :: No contaminated area file found in MISSION folder, your path is &quot; + m_Path + &quot; Attempting DATA folder&quot;); // If the path is invalid, we warn the user
            
            m_Path = &quot;&quot;;
            g_Game.GetWorldName(m_Path);
            m_Path = string.Format(&quot;dz/worlds/%1/ce/cfgeffectarea.json&quot;, m_Path);
            
            if (!FileExist(m_Path))
            {
                PrintToRPT(&quot;[WARNING] :: [EffectAreaLoader CreateZones] :: No contaminated area file found in DATA folder, your path is &quot; + m_Path); // If the path is invalid, we warn the user
                return; // Nothing could be read, just end here
            }
        }
        
        // We load the data from file, in case of failure we notify user
        effectAreaData = EffectAreaLoader.GetData();
        if (effectAreaData)
        {
            // Now that we have extracted the data we go through every declared area
            //Debug.Log(&quot;Contaminated area JSON contains : &quot; + effectAreaData.Areas.Count());
            
            for (int i = 0; i < effectAreaData.Areas.Count(); ++i)
            {
                EffectAreaParams params = new EffectAreaParams();
                
                // We feed in all relevant data
                params.m_ParamName = effectAreaData.Areas.Get( i ).AreaName;
                string areaType = effectAreaData.Areas.Get( i ).Type;
                params.m_ParamTriggerType = effectAreaData.Areas.Get( i ).TriggerType;
                JsonDataAreaData data = effectAreaData.Areas.Get( i ).Data;
                
                // World level area data ( Trigger info, world particles, etc... )
                vector pos = Vector( data.Pos[0], data.Pos[1], data.Pos[2] );
                if (data.Radius <= 0)
                {
                    ErrorEx(string.Format(&quot;Radius cannot be <= 0. Fix [%1] area definition in cfgeffectarea.json&quot;, params.m_ParamName));
                    continue;
                }
                
                params.m_ParamRadius = data.Radius;
                
                params.m_ParamPosHeight = data.PosHeight;
                params.m_ParamNegHeight = data.NegHeight;
 
                params.m_ParamInnerRings = data.InnerRingCount;
                params.m_ParamInnerSpace = data.InnerPartDist;
                params.m_ParamOuterToggle = data.OuterRingToggle;
                params.m_ParamOuterSpace = data.OuterPartDist;
                params.m_ParamOuterOffset = data.OuterOffset;
                params.m_ParamVertLayers = data.VerticalLayers;
                params.m_ParamVerticalOffset = data.VerticalOffset;
                string particleName = data.ParticleName;
                params.m_ParamEffectInterval = data.EffectInterval;
                params.m_ParamEffectDuration = data.EffectDuration;
                params.m_ParamEffectModifier = data.EffectModifier;
                
                // Local level area data ( Player particles and PPE )
                JsonDataPlayerData playerData = effectAreaData.Areas.Get( i ).PlayerData;
                string aroundPartName = playerData.AroundPartName;
                string tinyPartName = playerData.TinyPartName;
                string ppeRequesterType = playerData.PPERequesterType;
                
                // Conversion of particle name to ID for synchronization and loading
                if (particleName != &quot;&quot;)
                    params.m_ParamPartId = ParticleList.GetParticleID( particleName );
                
                if (aroundPartName != &quot;&quot;)
                    params.m_ParamAroundPartId = ParticleList.GetParticleID(aroundPartName);
                
                if (tinyPartName != &quot;&quot;)
                    params.m_ParamTinyPartId = ParticleList.GetParticleID(tinyPartName);
                
                params.m_ParamPpeRequesterType = ppeRequesterType;
 
                EffectArea newZone; // Zones MUST inherit from EffectArea
 
                // We snap item position to ground before creating if specified Y is 0
                if (pos[1] == 0)
                {
                    pos[1] = g_Game.SurfaceRoadY(pos[0], pos[2]);
                    newZone = EffectArea.Cast(g_Game.CreateObjectEx(areaType, pos, ECE_PLACE_ON_SURFACE));
                }
                else
                    newZone = EffectArea.Cast(g_Game.CreateObjectEx(areaType, pos, ECE_NONE));
                
                // We created a new zone, we feed in the data to finalize setup
                if (newZone)
                    newZone.SetupZoneData(params);
                else
                    Error(&quot;[WARNING] :: [EffectAreaLoader CreateZones] :: Cast failed, are you sure your class ( &#39;Type:&#39; ) inherits from EffectArea and that there are no Typos?&quot;);
            }
        }
        else
            Error(&quot;[WARNING] :: [EffectAreaLoader CreateZones] :: Data could not be read, please check data and syntax&quot;); // Most JSON related errors should be handled, but we have an extra check in case data could not be read
