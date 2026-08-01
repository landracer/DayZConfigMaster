// Source: F:/Games/Dayz/scripts/4_world/classes/hologram.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class Hologram
{
    #ifdef SERVER
    protected const int SPAWN_FLAGS     = ECE_CREATEPHYSICS;
    #else
    protected const int SPAWN_FLAGS     = ECE_LOCAL;
    #endif
    
    
    #ifdef DIAG_DEVELOPER
    string m_CollisionDetails;
    #endif
    
    protected const string SUFFIX_MATERIAL_DEPLOYABLE   = &quot;_deployable.rvmat&quot;;
    protected const string SUFFIX_MATERIAL_UNDEPLOYABLE = &quot;_undeployable.rvmat&quot;;
    protected const string SUFFIX_MATERIAL_POWERED      = &quot;_powered.rvmat&quot;;
    
    protected const vector PLACEMENT_RC_START_OFFSET = &quot;0 1 0&quot;;
    protected const vector PLACEMENT_RC_END_OFFSET = &quot;0 -2 0&quot;;
 
    protected ItemBase          m_Parent;
    protected EntityAI          m_Projection;
    protected PlayerBase        m_Player;
    protected ProjectionTrigger m_ProjectionTrigger;
    protected string            m_ProjectionTypename;
 
    protected bool              m_IsColliding;
    protected bool              m_IsCollidingGPlot;
    protected bool              m_IsSlope;
    protected bool              m_IsCollidingPlayer;
    protected bool              m_IsFloating;
    protected bool              m_UpdatePosition;
    protected bool              m_IsHidden;
 
    protected vector            m_DefaultOrientation;
    protected vector            m_Rotation;
    protected vector            m_y_p_r_previous;
    protected vector            m_ContactDir;
    protected vector            m_FromAdjusted;
    protected const string      ANIMATION_PLACING               = &quot;Placing&quot;;
    protected const string      ANIMATION_INVENTORY             = &quot;Inventory&quot;;
    protected const string      SELECTION_PLACING               = &quot;placing&quot;;
    protected const string      SELECTION_INVENTORY             = &quot;inventory&quot;;
 
    protected const float       SMALL_PROJECTION_RADIUS         = 1;
    protected const float       SMALL_PROJECTION_GROUND         = 2;
    protected const float       DISTANCE_SMALL_PROJECTION       = 1; 
    protected const float       LARGE_PROJECTION_DISTANCE_LIMIT = 6;
    protected const float       PROJECTION_TRANSITION_MIN       = 1;
    protected const float       PROJECTION_TRANSITION_MAX       = 0.25;
    protected const float       LOOKING_TO_SKY                  = 0.75;
    static const float          DEFAULT_MAX_PLACEMENT_HEIGHT_DIFF = 1.5;
    
    protected float             m_SlopeTolerance;
    protected bool              m_AlignToTerrain;
    protected vector            m_YawPitchRollLimit;
    protected int               m_ContactComponent;
    
    protected ref set<string>   m_SelectionsToRefresh           = new set<string>;
    
    // Watchtower correction variables  
    // These watchtower component names should be corrected when colliding with them as they are supposed to be &quot;trigger boxes&quot;, not colliders
    static const protected ref array<string>    m_WatchtowerIgnoreComponentNames    = new array<string>;
    
    // These watchtower components are supposed to be trigger boxes, but should block placement on them (currently only the boxes above the stairs)
    static const protected ref array<string>    m_WatchtowerBlockedComponentNames   = new array<string>;
 
    void Hologram(PlayerBase player, vector pos, ItemBase item)
    {   
        m_Player        = player;
        m_Parent        = item;
        m_Projection    = null;
 
        m_ProjectionTrigger = null;
        m_UpdatePosition    = true;
        m_ContactComponent  = -1;
 
        m_Rotation      = &quot;0 0 0&quot;;
        m_FromAdjusted  = &quot;0 0 0&quot;;
        
        // If the static names are empty, generate the needed names
        // Refer to their definitions to see why these are required
        if (m_WatchtowerIgnoreComponentNames.Count() == 0)
        {
            string baseStringBegin      = Watchtower.BASE_VIEW_NAME;
            string baseIgnoreStringEnd  = Watchtower.BASE_WALL_NAME;
 
            int floors  = Watchtower.MAX_WATCHTOWER_FLOORS;
            int walls   = Watchtower.MAX_WATCHTOWER_WALLS;
 
            string compName;
            for (int i = 1; i < floors + 1; ++i)
            {
                compName = baseStringBegin + i.ToString();
                for (int j = 1; j < walls + 1; ++j)
                    m_WatchtowerIgnoreComponentNames.Insert(compName + baseIgnoreStringEnd + j.ToString());
                
                if (i != 1)
                    m_WatchtowerBlockedComponentNames.Insert(compName);
                else
                    m_WatchtowerIgnoreComponentNames.Insert(compName);
            }
        }
 
        string configPathProjectionTypename = string.Format(&quot;CfgVehicles %1 projectionTypename&quot;, m_Parent.GetType());
        if (g_Game.ConfigIsExisting(configPathProjectionTypename))
        {
            m_ProjectionTypename = g_Game.ConfigGetTextOut(configPathProjectionTypename);
        }
 
        EntityAI projectionEntity;
        if (g_Game.IsMultiplayer() && g_Game.IsServer())
        {   
            projectionEntity = EntityAI.Cast(g_Game.CreateObjectEx(ProjectionBasedOnParent(), pos, ECE_PLACE_ON_SURFACE));
            projectionEntity.SetAllowDamage(false);
            SetProjectionEntity(projectionEntity);
            SetAnimations();
        }
        else
        {
            projectionEntity = EntityAI.Cast(g_Game.CreateObjectEx(ProjectionBasedOnParent(), pos, ECE_TRACE|ECE_LOCAL));
            if (projectionEntity == null)
            {
                ErrorEx(string.Format(&quot;Cannot create hologram entity from config class %1&quot;, ProjectionBasedOnParent()), ErrorExSeverity.WARNING);
                return;
            }
 
            SetProjectionEntity(projectionEntity);
            SetAnimations();
            CreateTrigger();
            RefreshTrigger();
        }
        
        if (ItemBase.Cast(projectionEntity))
        {
            ItemBase.Cast(GetProjectionEntity()).SetIsHologram(true);
        }
        
        string configPathSlope = string.Format(&quot;CfgVehicles %1 slopeTolerance&quot;, GetProjectionEntity().GetType());
        if (g_Game.ConfigIsExisting(configPathSlope))
        {
            m_SlopeTolerance = g_Game.ConfigGetFloat(configPathSlope);
        }
        
        string configPathAlign = string.Format(&quot;CfgVehicles %1 alignHologramToTerain&quot;, GetProjectionEntity().GetType());
        if (g_Game.ConfigIsExisting(configPathAlign))
        {
            m_AlignToTerrain = g_Game.ConfigGetInt(configPathAlign);
        }
        
        string configPathOrientationLimit = string.Format(&quot;CfgVehicles %1 yawPitchRollLimit&quot;, GetProjectionEntity().GetType());
        if (g_Game.ConfigIsExisting(configPathOrientationLimit))
        {
            m_YawPitchRollLimit = g_Game.ConfigGetVector(configPathOrientationLimit);
        }
