// Source: F:/Games/Dayz/scripts/3_game/particles/particlelist.c Source File
// Extracted from DayZ Code Explorer by Zeroy

// Register all particles below!
 
// Example how to register particles from a mod
/*
modded class ParticleList
{
    static const int MODDED_PARTICLE = RegisterParticle( &quot;mod_folder/&quot; , &quot;my_modded_particle&quot;);
}
*/
 
class ParticleList
{
    ref static map<int, string> m_ParticlePaths; // Contains full paths to all particles. WARNING: Paths are without the &#39;.ptc&#39; suffix!
    ref static map<string, int> m_ParticleNames; // Contains file NAME (without suffix) to id. 
    
    static int m_lastID = 0;
    
    // REGISTER ALL PARTICLES BELOW:
    
    static const int INVALID                        = -1;
    static const int NONE                           = 0; // 0 does not exist either, valid particle starts from 1
    static const int PARTICLE_TEST                  = RegisterParticle(&quot;_test_orientation&quot;);
    static const int DEBUG_DOT                      = RegisterParticle(&quot;debug_dot&quot;);
    static const int DEBUG_DOT5M                    = RegisterParticle(&quot;debug_dot5m&quot;);
    
    // FIREPLACE    
    // Normal fireplace
    static const int CAMP_FIRE_START                = RegisterParticle(&quot;fire_small_camp_01_start&quot;);
    static const int CAMP_SMALL_FIRE                = RegisterParticle(&quot;fire_small_camp_01&quot;);
    static const int CAMP_NORMAL_FIRE               = RegisterParticle(&quot;fire_medium_camp_01&quot;);
    static const int CAMP_SMALL_SMOKE               = RegisterParticle(&quot;smoke_small_camp_01&quot;);
    static const int CAMP_NORMAL_SMOKE              = RegisterParticle(&quot;smoke_medium_camp_01&quot;);
    static const int CAMP_FIRE_END                  = RegisterParticle(&quot;fire_small_camp_01_end&quot;);
    static const int CAMP_STEAM_2END                = RegisterParticle(&quot;steam_medium_camp_2end&quot;);
    static const int CAMP_STEAM_EXTINGUISH_START    = RegisterParticle(&quot;default_01&quot;);
    static const int CAMP_STOVE_FIRE                = RegisterParticle(&quot;fire_small_stove_01&quot;);
    static const int CAMP_STOVE_FIRE_START          = RegisterParticle(&quot;fire_small_stove_01_start&quot;);
    static const int CAMP_STOVE_FIRE_END            = RegisterParticle(&quot;fire_small_stove_01_end&quot;);
    static const int CAMP_NO_IGNITE_WIND            = RegisterParticle(&quot;fire_extinguish_wind&quot;);
    // Fireplace indoor
    static const int HOUSE_FIRE_START               = RegisterParticle(&quot;fire_small_house_01_start&quot;);
    static const int HOUSE_SMALL_FIRE               = RegisterParticle(&quot;fire_small_house_01&quot;);
    static const int HOUSE_SMALL_SMOKE              = RegisterParticle(&quot;smoke_small_house_01&quot;);
    static const int HOUSE_NORMAL_FIRE              = RegisterParticle(&quot;fire_medium_house_01&quot;);
    static const int HOUSE_NORMAL_SMOKE             = RegisterParticle(&quot;smoke_medium_house_01&quot;);
    static const int HOUSE_FIRE_END                 = RegisterParticle(&quot;fire_small_house_01_end&quot;);
    static const int HOUSE_FIRE_STEAM_2END          = RegisterParticle(&quot;steam_medium_house_2end&quot;);
    // Fireplace in barrel with holes
    static const int BARREL_FIRE_START              = RegisterParticle(&quot;fire_small_barrel_01_start&quot;);
    static const int BARREL_SMALL_FIRE              = RegisterParticle(&quot;fire_small_barrel_01&quot;);
    static const int BARREL_SMALL_SMOKE             = RegisterParticle(&quot;smoke_small_barrel_01&quot;);
    static const int BARREL_NORMAL_FIRE             = RegisterParticle(&quot;fire_medium_barrel_01&quot;);
    static const int BARREL_NORMAL_SMOKE            = RegisterParticle(&quot;smoke_medium_barrel_01&quot;);
    static const int BARREL_FIRE_END                = RegisterParticle(&quot;fire_small_barrel_01_end&quot;);
    static const int BARREL_FIRE_STEAM_2END         = RegisterParticle(&quot;steam_medium_camp_2end&quot;);
    // Fireplace in indoor oven
    static const int OVEN_FIRE_START                = RegisterParticle(&quot;fire_small_oven_01_start&quot;);
    static const int OVEN_SMALL_FIRE                = RegisterParticle(&quot;fire_small_oven_01&quot;);
    static const int OVEN_NORMAL_FIRE               = RegisterParticle(&quot;fire_medium_oven_01&quot;);
    static const int OVEN_FIRE_END                  = RegisterParticle(&quot;fire_small_ovenl_01_end&quot;);
    
    // COOKING
    static const int COOKING_BOILING_EMPTY          = RegisterParticle(&quot;cooking_boiling_empty&quot;);
    static const int COOKING_BOILING_START          = RegisterParticle(&quot;cooking_boiling_start&quot;);
    static const int COOKING_BOILING_DONE           = RegisterParticle(&quot;cooking_boiling_done&quot;);
    static const int COOKING_BAKING_START           = RegisterParticle(&quot;cooking_baking_start&quot;);
    static const int COOKING_BAKING_DONE            = RegisterParticle(&quot;cooking_baking_done&quot;);  
    static const int COOKING_DRYING_START           = RegisterParticle(&quot;cooking_drying_start&quot;);
    static const int COOKING_DRYING_DONE            = RegisterParticle(&quot;cooking_drying_done&quot;);
    static const int COOKING_BURNING_DONE           = RegisterParticle(&quot;cooking_burning_done&quot;);
    static const int ITEM_HOT_VAPOR                 = RegisterParticle(&quot;item_hot_vapor&quot;);
    
    // TORCH
    static const int TORCH_T3                       = RegisterParticle(&quot;fire_small_torch_01&quot;);
    static const int TORCH_T1                       = RegisterParticle(&quot;fire_small_torch_02&quot;);
    static const int TORCH_T2                       = RegisterParticle(&quot;fire_small_torch_03&quot;);
    
    //BROOM TORCH
    static const int BROOM_TORCH_T1                 = RegisterParticle(&quot;fire_small_broom_torch_01&quot;);
    static const int BROOM_TORCH_T2                 = RegisterParticle(&quot;fire_small_broom_torch_02&quot;);
    static const int BROOM_TORCH_T3                 = RegisterParticle(&quot;fire_small_broom_torch_03&quot;);
    
    // ROADFLARE
    static const int ROADFLARE_BURNING_INIT         = RegisterParticle(&quot;fire_small_roadflare_red_04&quot;);
    static const int ROADFLARE_BURNING_MAIN         = RegisterParticle(&quot;fire_small_roadflare_red_01&quot;);
    static const int ROADFLARE_BURNING_ENDING       = RegisterParticle(&quot;fire_small_roadflare_red_02&quot;);
    static const int ROADFLARE_BURNING_SMOKE        = RegisterParticle(&quot;fire_small_roadflare_red_03&quot;);
    
    // FLARE PROJECTILE
    static const int FLAREPROJ_FIRE                 = RegisterParticle(&quot;fire_small_roadflare_red_04&quot;);
    static const int FLAREPROJ_ACTIVATE             = RegisterParticle(&quot;fire_small_flare_yellow_01&quot;);
    static const int FLAREPROJ_ACTIVATE_RED         = RegisterParticle(&quot;fire_small_flare_red_01&quot;);
    static const int FLAREPROJ_ACTIVATE_GREEN       = RegisterParticle(&quot;fire_small_flare_green_01&quot;);
    static const int FLAREPROJ_ACTIVATE_BLUE        = RegisterParticle(&quot;fire_small_flare_blue_01&quot;);
    
    // DIGGING
    static const int DIGGING_STASH                  = RegisterParticle(&quot;digging_ground&quot;);
    
    // SMOKE GRENADES
    static const int GRENADE_RDG2_BLACK_START       = RegisterParticle(&quot;smoke_RDG2_black_01&quot;);
    static const int GRENADE_RDG2_BLACK_LOOP        = RegisterParticle(&quot;smoke_RDG2_black_02&quot;);
    static const int GRENADE_RDG2_BLACK_END         = RegisterParticle(&quot;smoke_RDG2_black_03&quot;);
    static const int GRENADE_RDG2_WHITE_START       = RegisterParticle(&quot;smoke_RDG2_white_01&quot;);
    static const int GRENADE_RDG2_WHITE_LOOP        = RegisterParticle(&quot;smoke_RDG2_white_02&quot;);
    static const int GRENADE_RDG2_WHITE_END         = RegisterParticle(&quot;smoke_RDG2_white_03&quot;);
    static const int GRENADE_M18_GREEN_START        = RegisterParticle(&quot;smoke_M18_green_01&quot;);
    static const int GRENADE_M18_GREEN_LOOP         = RegisterParticle(&quot;smoke_M18_green_02&quot;);
    static const int GRENADE_M18_GREEN_END          = RegisterParticle(&quot;smoke_M18_green_03&quot;);
    static const int GRENADE_M18_PURPLE_START       = RegisterParticle(&quot;smoke_M18_purple_01&quot;);
    static const int GRENADE_M18_PURPLE_LOOP        = RegisterParticle(&quot;smoke_M18_purple_02&quot;);
    static const int GRENADE_M18_PURPLE_END         = RegisterParticle(&quot;smoke_M18_purple_03&quot;);
    static const int GRENADE_M18_RED_START          = RegisterParticle(&quot;smoke_M18_red_01&quot;);
    static const int GRENADE_M18_RED_LOOP           = RegisterParticle(&quot;smoke_M18_red_02&quot;);
    static const int GRENADE_M18_RED_END            = RegisterParticle(&quot;smoke_M18_red_03&quot;);
    static const int GRENADE_M18_WHITE_START        = RegisterParticle(&quot;smoke_M18_white_01&quot;);
    static const int GRENADE_M18_WHITE_LOOP         = RegisterParticle(&quot;smoke_M18_white_02&quot;);
    static const int GRENADE_M18_WHITE_END          = RegisterParticle(&quot;smoke_M18_white_03&quot;);
    static const int GRENADE_M18_YELLOW_START       = RegisterParticle(&quot;smoke_M18_yellow_01&quot;);
    static const int GRENADE_M18_YELLOW_LOOP        = RegisterParticle(&quot;smoke_M18_yellow_02&quot;);
    static const int GRENADE_M18_YELLOW_END         = RegisterParticle(&quot;smoke_M18_yellow_03&quot;);
    static const int GRENADE_M18_BLACK_START        = RegisterParticle(&quot;smoke_M18_black_01&quot;);
    static const int GRENADE_M18_BLACK_LOOP         = RegisterParticle(&quot;smoke_M18_black_02&quot;);
    static const int GRENADE_M18_BLACK_END          = RegisterParticle(&quot;smoke_M18_black_03&quot;);

    static const int GRENADE_M84                    = RegisterParticle(&quot;explosion_M84_01&quot;);
    
    // FRAGMENTATION GRENADES
    static const int RGD5                           = RegisterParticle(&quot;explosion_RGD5_01&quot;);
    static const int M67                            = RegisterParticle(&quot;explosion_M67_01&quot;);
    
    // GRENADE EXPLOSION BY SURFACE
    static const int EXPLOSION_GRENADE_SNOW         = RegisterParticle(&quot;explosion_grenade_snow&quot;);
    static const int EXPLOSION_GRENADE_ICE          = RegisterParticle(&quot;explosion_grenade_ice&quot;);
    
    // ELECTRICITY          
    static const int POWER_GENERATOR_SMOKE          = RegisterParticle(&quot;smoke_small_generator_01&quot;);
    static const int BARBED_WIRE_SPARKS             = RegisterParticle(&quot;electro_shortc2&quot;);
    static const int LEVER_SPARKS                   = RegisterParticle(&quot;electro_sparks&quot;);
    static const int EASTER_EGG_ACTIVATE            = RegisterParticle(&quot;easter_egg_activate&quot;);
 
    // PLAYER
    static const int BLEEDING_SOURCE                = RegisterParticle(&quot;blood_bleeding_01&quot;);
    static const int BLEEDING_SOURCE_LIGHT          = RegisterParticle(&quot;blood_bleeding_02&quot;);
    static const int BLOOD_SURFACE_DROPS            = RegisterParticle(&quot;blood_surface_drops&quot;);
    static const int BLOOD_SURFACE_CHUNKS           = RegisterParticle(&quot;blood_surface_chunks&quot;);
    static const int VOMIT                          = RegisterParticle(&quot;character_vomit_01&quot;);
    static const int BREATH_VAPOUR_LIGHT            = RegisterParticle(&quot;breath_vapour_light&quot;);
    static const int BREATH_VAPOUR_MEDIUM           = RegisterParticle(&quot;breath_vapour_medium&quot;);
    static const int BREATH_VAPOUR_HEAVY            = RegisterParticle(&quot;breath_vapour_heavy&quot;);
    static const int VOMIT_CHUNKS                   = RegisterParticle(&quot;character_vomit_puddle&quot;);
    
    // GUNS
    static const int GUN_MUZZLE_FLASH_SVD_STAR      = RegisterParticle(&quot;weapon_shot_Flame_3D_4star&quot;);
    static const int GUN_SOLO_MUZZLE_FLASH          = RegisterParticle(&quot;weapon_shot_Flame_3D&quot;);
    static const int GUN_FNX                        = RegisterParticle(&quot;weapon_shot_fnx_01&quot;);
    static const int GUN_FNX_SUPPRESSED             = RegisterParticle(&quot;weapon_shot_fnx_02&quot;);
    static const int GUN_PARTICLE_TEST              = RegisterParticle(&quot;weapon_shot_fnx_01&quot;);
    static const int GUN_PARTICLE_CASING            = RegisterParticle(&quot;weapon_shot_chamber_smoke&quot;);
    static const int GUN_PARTICLE_CASING_RAISE      = RegisterParticle(&quot;weapon_shot_chamber_smoke_raise&quot;);
    static const int GUN_CZ75                       = RegisterParticle(&quot;weapon_shot_cz75_01&quot;);
    static const int GUN_AKM                        = RegisterParticle(&quot;weapon_shot_akm_01&quot;);
    static const int GUN_AKM_SUPPRESSED             = RegisterParticle(&quot;weapon_shot_akm_02&quot;);
    static const int GUN_IZH18                      = RegisterParticle(&quot;weapon_shot_izh18_01&quot;);
    static const int GUN_IZH18_SUPPRESSED           = RegisterParticle(&quot;weapon_shot_izh18_02&quot;);
    static const int GUN_MP5K                       = RegisterParticle(&quot;weapon_shot_mp5k_01&quot;);
    static const int GUN_MP5K_2                     = RegisterParticle(&quot;weapon_shot_mp5k_02_boris&quot;);
    static const int GUN_MP5K_SUPPRESSED            = RegisterParticle(&quot;weapon_shot_mp5k_02&quot;);
    static const int GUN_MP5K_COMPENSATOR           = RegisterParticle(&quot;weapon_shot_mp5k_02&quot;);
    static const int GUN_UMP45                      = RegisterParticle(&quot;weapon_shot_ump45_01&quot;);
    static const int GUN_M4A1                       = RegisterParticle(&quot;weapon_shot_m4a1_01&quot;);
    static const int GUN_MP133                      = RegisterParticle(&quot;weapon_shot_mp133_01&quot;);
    static const int GUN_PELLETS                    = RegisterParticle(&quot;weapon_shot_pellets&quot;);
    static const int GUN_MOSIN9130                  = RegisterParticle(&quot;weapon_shot_mosin9130_01&quot;);
    static const int GUN_MOSIN_COMPENSATOR          = RegisterParticle(&quot;weapon_shot_mosin_compensator_01&quot;);
    static const int GUN_CZ527                      = RegisterParticle(&quot;weapon_shot_cz527_01&quot;);
    static const int GUN_SKS                        = RegisterParticle(&quot;weapon_shot_sks_01&quot;);
    static const int GUN_WINCHESTER70               = RegisterParticle(&quot;weapon_shot_winch70_01&quot;);
    static const int GUN_VSS                        = RegisterParticle(&quot;weapon_shot_vss_01&quot;);
    static const int GUN_AK74                       = RegisterParticle(&quot;weapon_shot_ak74_01&quot;);
    static const int GUN_AK101                      = RegisterParticle(&quot;weapon_shot_ak101_01&quot;);
    
    static const int GUN_MAGNUM                     = RegisterParticle(&quot;weapon_shot_magnum_01&quot;);
    
    static const int GUN_CZ61                       = RegisterParticle(&quot;weapon_shot_cz61_01&quot;);
    static const int GUN_LONG_WINDED_SMOKE          = RegisterParticle(&quot;weapon_shot_winded_smoke&quot;);
    static const int GUN_LONG_WINDED_SMOKE_SMALL    = RegisterParticle(&quot;weapon_shot_winded_smoke_small&quot;);
    static const int SMOKING_BARREL                 = RegisterParticle(&quot;smoking_barrel&quot;);
    static const int SMOKING_BARREL_SMALL           = RegisterParticle(&quot;smoking_barrel_small&quot;);
    static const int SMOKING_BARREL_HEAVY           = RegisterParticle(&quot;smoking_barrel_heavy&quot;);
    static const int SMOKING_BARREL_STEAM           = RegisterParticle(&quot;smoking_barrel_steam&quot;);
    static const int SMOKING_BARREL_STEAM_SMALL     = RegisterParticle(&quot;smoking_barrel_steam_small&quot;);
    static const int SMARKS_CHAMBER                 = RegisterParticle(&quot;weapon_shot_chamber_spark&quot;);
    
    // BULLET & MELEE IMPACTS
    static const int IMPACT_TEST                    = RegisterParticle(&quot;impacts/bullet_impact_placeholder&quot;);
    static const int IMPACT_DISTANT_DUST            = RegisterParticle(&quot;impacts/distant_dust&quot;);
    static const int IMPACT_TEST_RICOCHET           = RegisterParticle(&quot;impacts/bullet_riochet_placeholder&quot;);
    static const int IMPACT_TEST2                   = RegisterParticle(&quot;_test_orientation_02&quot;);
    static const int IMPACT_TEST_ENTER_DEBUG        = RegisterParticle(&quot;impacts/_test_impact_enter_debug&quot;);
    static const int IMPACT_TEST_RICOCHET_DEBUG     = RegisterParticle(&quot;impacts/_test_impact_ricochet_debug&quot;);
    static const int IMPACT_TEST_EXIT_DEBUG         = RegisterParticle(&quot;impacts/_test_impact_exit_debug&quot;);
    static const int IMPACT_TEST_NO_MATERIAL_ERROR  = RegisterParticle(&quot;_test_no_material&quot;);
    static const int IMPACT_WOOD_ENTER              = RegisterParticle(&quot;impacts/hit_wood_ent_01&quot;);
    static const int IMPACT_WOOD_RICOCHET           = RegisterParticle(&quot;impacts/hit_wood_ric_01&quot;);
    static const int IMPACT_WOOD_EXIT               = RegisterParticle(&quot;impacts/hit_wood_ext_01&quot;);
    static const int IMPACT_CONCRETE_ENTER          = RegisterParticle(&quot;impacts/hit_concrete_ent_01&quot;);
    static const int IMPACT_CONCRETE_RICOCHET       = RegisterParticle(&quot;impacts/hit_concrete_ric_01&quot;);
    static const int IMPACT_CONCRETE_EXIT           = RegisterParticle(&quot;impacts/hit_concrete_ext_01&quot;);
    static const int IMPACT_FOLIAGE_ENTER           = RegisterParticle(&quot;impacts/hit_foliage_ent_01&quot;);
    static const int IMPACT_FOLIAGE_RICOCHET        = RegisterParticle(&quot;impacts/hit_foliage_ric_01&quot;);
    static const int IMPACT_FOLIAGE_EXIT            = RegisterParticle(&quot;impacts/hit_foliage_ext_01&quot;);
    static const int IMPACT_FOLIAGE_GREEN_ENTER     = RegisterParticle(&quot;impacts/hit_foliage_green_ent_01&quot;);
    static const int IMPACT_FOLIAGE_GREEN_RICOCHET  = RegisterParticle(&quot;impacts/hit_foliage_green_ric_01&quot;);
    static const int IMPACT_FOLIAGE_GREEN_EXIT      = RegisterParticle(&quot;impacts/hit_foliage_green_ext_01&quot;);
    static const int IMPACT_FOLIAGE_CONIFER_ENTER   = RegisterParticle(&quot;impacts/hit_foliage_conifer_ent_01&quot;);
    static const int IMPACT_FOLIAGE_CONIFER_RICOCHET= RegisterParticle(&quot;impacts/hit_foliage_conifer_ric_01&quot;);
    static const int IMPACT_FOLIAGE_CONIFER_EXIT    = RegisterParticle(&quot;impacts/hit_foliage_conifer_ext_01&quot;);
    static const int IMPACT_GRASS_ENTER             = RegisterParticle(&quot;impacts/hit_grass_ent_01&quot;);
    static const int IMPACT_GRASS_RICOCHET          = RegisterParticle(&quot;impacts/hit_grass_ric_01&quot;);
    static const int IMPACT_DIRT_ENTER              = RegisterParticle(&quot;impacts/hit_dirt_ent_01&quot;);
    static const int IMPACT_DIRT_RICOCHET           = RegisterParticle(&quot;impacts/hit_dirt_ric_01&quot;);
    static const int IMPACT_DIRT_EXIT               = RegisterParticle(&quot;impacts/hit_dirt_ext_01&quot;);
    static const int IMPACT_RUBBER_ENTER            = RegisterParticle(&quot;impacts/hit_rubber_ent_01&quot;);
    static const int IMPACT_RUBBER_RICOCHET         = RegisterParticle(&quot;impacts/hit_rubber_ric_01&quot;);
    static const int IMPACT_RUBBER_EXIT             = RegisterParticle(&quot;impacts/hit_rubber_ext_01&quot;);
    static const int IMPACT_GRAVEL_ENTER            = RegisterParticle(&quot;impacts/hit_gravel_ent_01&quot;);
    static const int IMPACT_GRAVEL_RICOCHET         = RegisterParticle(&quot;impacts/hit_gravel_ric_01&quot;);
    static const int IMPACT_GRAVEL_EXIT             = RegisterParticle(&quot;impacts/hit_gravel_ext_01&quot;);
    static const int IMPACT_PLASTER_ENTER           = RegisterParticle(&quot;impacts/hit_plaster_ent_01&quot;);
    static const int IMPACT_PLASTER_RICOCHET        = RegisterParticle(&quot;impacts/hit_plaster_ric_01&quot;);
    static const int IMPACT_PLASTER_EXIT            = RegisterParticle(&quot;impacts/hit_plaster_ext_01&quot;);
    static const int IMPACT_METAL_ENTER             = RegisterParticle(&quot;impacts/hit_metal_ent_01&quot;);
    static const int IMPACT_METAL_RICOCHET          = RegisterParticle(&quot;impacts/hit_metal_ric_01&quot;);
    static const int IMPACT_METAL_EXIT              = RegisterParticle(&quot;impacts/hit_metal_ext_01&quot;);
    static const int IMPACT_MEAT_ENTER              = RegisterParticle(&quot;impacts/hit_meat_ent_01&quot;);
    static const int IMPACT_MEAT_RICOCHET           = RegisterParticle(&quot;impacts/hit_meat_ric_01&quot;);
    static const int IMPACT_MEAT_EXIT               = RegisterParticle(&quot;impacts/hit_meat_ext_01&quot;);
    static const int IMPACT_MEATBONES_ENTER         = RegisterParticle(&quot;impacts/hit_meatbones_ent_01&quot;);
    static const int IMPACT_MEATBONES_RICOCHET      = RegisterParticle(&quot;impacts/hit_meatbones_ent_01&quot;);
    static const int IMPACT_MEATBONES_EXIT          = RegisterParticle(&quot;impacts/hit_meatbones_ext_01&quot;);
    static const int IMPACT_GLASS_ENTER             = RegisterParticle(&quot;impacts/hit_glass_ent_01&quot;);
    static const int IMPACT_GLASS_RICOCHET          = RegisterParticle(&quot;impacts/hit_glass_ric_01&quot;);
    static const int IMPACT_GLASS_EXIT              = RegisterParticle(&quot;impacts/hit_glass_ext_01&quot;);
    static const int IMPACT_WATER_SMALL_ENTER       = RegisterParticle(&quot;impacts/hit_water_ent_01&quot;);
    static const int IMPACT_WATER_MEDIUM_ENTER      = RegisterParticle(&quot;impacts/hit_water_ent_02&quot;);
    static const int IMPACT_WATER_LARGE_ENTER       = RegisterParticle(&quot;impacts/hit_water_ent_03&quot;);
    static const int IMPACT_TEXTILE_ENTER           = RegisterParticle(&quot;impacts/hit_textile_ent_01&quot;);
    static const int IMPACT_TEXTILE_RICOCHET        = RegisterParticle(&quot;impacts/hit_textile_ric_01&quot;);
    static const int IMPACT_TEXTILE_EXIT            = RegisterParticle(&quot;impacts/hit_textile_ext_01&quot;);
    static const int IMPACT_SAND_ENTER              = RegisterParticle(&quot;impacts/hit_sand_ent_01&quot;);
    static const int IMPACT_SAND_RICOCHET           = RegisterParticle(&quot;impacts/hit_sand_ric_01&quot;);
    static const int IMPACT_SAND_EXIT               = RegisterParticle(&quot;impacts/hit_sand_ext_01&quot;);
    static const int IMPACT_PLASTIC_ENTER           = RegisterParticle(&quot;impacts/hit_plastic_ent_01&quot;);
    static const int IMPACT_PLASTIC_RICOCHET        = RegisterParticle(&quot;impacts/hit_plastic_ric_01&quot;);
    static const int IMPACT_PLASTIC_EXIT            = RegisterParticle(&quot;impacts/hit_plastic_ext_01&quot;);
    static const int IMPACT_SNOW_ENTER              = RegisterParticle(&quot;impacts/hit_snow_ent_01&quot;);
    static const int IMPACT_SNOW_RICOCHET           = RegisterParticle(&quot;impacts/hit_snow_ric_01&quot;);
    static const int IMPACT_SNOW_EXIT               = RegisterParticle(&quot;impacts/hit_snow_ext_01&quot;);
    static const int IMPACT_ICE_ENTER               = RegisterParticle(&quot;impacts/hit_ice_ent_01&quot;);
    static const int IMPACT_ICE_RICOCHET            = RegisterParticle(&quot;impacts/hit_ice_ric_01&quot;);
    static const int IMPACT_ICE_EXIT                = RegisterParticle(&quot;impacts/hit_ice_ext_01&quot;);
    
    // EXPLOSIONS
    static const int EXPLOSION_LANDMINE             = RegisterParticle(&quot;explosion_landmine_01&quot;);
    static const int EXPLOSION_TEST                 = RegisterParticle(&quot;explosion_placeholder&quot;);
    static const int EXPLOSION_GOAT                 = RegisterParticle(&quot;explosion_goat&quot;);
    
    // ENVIRO EFX
    static const int SMOKING_HELI_WRECK             = RegisterParticle(&quot;smoke_heli_wreck_01&quot;);
    static const int AURORA_SANTA_WRECK             = RegisterParticle(&quot;smoke_santa_wreck&quot;);
    static const int SMOKE_GENERIC_WRECK            = RegisterParticle(&quot;smoke_generic_wreck&quot;);
    static const int SMOKING_CAR_ENGINE             = RegisterParticle(&quot;menu_engine_fire&quot;);
    static const int EVAPORATION                    = RegisterParticle(&quot;menu_evaporation&quot;);
    
    // VEHICLES
    static const int HATCHBACK_COOLANT_OVERHEATING  = RegisterParticle(&quot;Hatchback_coolant_overheating&quot;);
    static const int HATCHBACK_COOLANT_OVERHEATED   = RegisterParticle(&quot;Hatchback_coolant_overheated&quot;);
    static const int HATCHBACK_ENGINE_OVERHEATING   = RegisterParticle(&quot;Hatchback_engine_failing&quot;);
    static const int HATCHBACK_ENGINE_OVERHEATED    = RegisterParticle(&quot;Hatchback_engine_failure&quot;);
    static const int HATCHBACK_EXHAUST_SMOKE        = RegisterParticle(&quot;Hatchback_exhaust&quot;);
    static const int BOAT_WATER_FRONT               = RegisterParticle(&quot;vehicles/boat/boat_water_front&quot;);
    static const int BOAT_WATER_BACK                = RegisterParticle(&quot;vehicles/boat/boat_water_back&quot;);
    static const int BOAT_WATER_SIDE                = RegisterParticle(&quot;vehicles/boat/boat_water_side&quot;);
    
    // CORPSE DECAY
    static const int ENV_SWARMING_FLIES             = RegisterParticle(&quot;env_fly_swarm_01&quot;);
    
    // BONFIRE 
    static const int BONFIRE_FIRE                   = RegisterParticle(&quot;fire_bonfire&quot;);
    static const int BONFIRE_SMOKE                  = RegisterParticle(&quot;smoke_bonfire&quot;);
    static const int TIREPILE_FIRE                  = RegisterParticle(&quot;fire_tirepile&quot;);
    static const int SPOOKY_MIST                    = RegisterParticle(&quot;spooky_mist&quot;);
    
    static const int VOMIT_BLOOD                    = RegisterParticle(&quot;character_vomitBlood_01&quot;);
 
    // CONTAMINATED AREAS
    static const int CONTAMINATED_AREA_GAS_TINY     = RegisterParticle(&quot;contaminated_area_gas_around_tiny&quot;);
    static const int CONTAMINATED_AREA_GAS_AROUND   = RegisterParticle(&quot;contaminated_area_gas_around&quot;);
    static const int CONTAMINATED_AREA_GAS_BIGASS   = RegisterParticle(&quot;contaminated_area_gas_bigass&quot;);
    static const int CONTAMINATED_AREA_GAS_GROUND   = RegisterParticle(&quot;contaminated_area_gas_ground&quot;);
    static const int CONTAMINATED_AREA_GAS_SHELL    = RegisterParticle(&quot;contaminated_area_gas_shell&quot;);
    static const int CONTAMINATED_AREA_GAS_DEBUG    = RegisterParticle(&quot;contaminated_area_gas_bigass_debug&quot;);
    
    //Fireworks
    static const int FIREWORKS_SHOT                 = RegisterParticle(&quot;fireworks_small_01&quot;);
    static const int FIREWORKS_EXPLOSION_RED        = RegisterParticle(&quot;fireworks_large_01_Red&quot;);
    static const int FIREWORKS_EXPLOSION_GREEN      = RegisterParticle(&quot;fireworks_large_01_Green&quot;);
    static const int FIREWORKS_EXPLOSION_BLUE       = RegisterParticle(&quot;fireworks_large_01_Blue&quot;);
    static const int FIREWORKS_EXPLOSION_YELLOW     = RegisterParticle(&quot;fireworks_large_01_Yellow&quot;);
    static const int FIREWORKS_EXPLOSION_PINK       = RegisterParticle(&quot;fireworks_large_01_Pink&quot;);
    
    static const int FIREWORKS_FUSE                 = RegisterParticle(&quot;fireworks_small_04&quot;);
    static const int FIREWORKS_AFTERBURN_START      = RegisterParticle(&quot;fireworks_small_02&quot;);
    static const int FIREWORKS_AFTERBURN_END        = RegisterParticle(&quot;fireworks_small_03&quot;);
    
    //Fireworks anniversary
    static const int FIREWORKS_EXPLOSION_THANKS1    = RegisterParticle(&quot;fireworks_ThankYou_anim&quot;);
    static const int FIREWORKS_EXPLOSION_THANKS2    = RegisterParticle(&quot;fireworks_For10_anim&quot;);
    static const int FIREWORKS_EXPLOSION_THANKS3    = RegisterParticle(&quot;fireworks_Amazing_anim&quot;);
    static const int FIREWORKS_EXPLOSION_THANKS4    = RegisterParticle(&quot;fireworks_Years_anim&quot;);
    static const int FIREWORKS_EXPLOSION_THANKS5    = RegisterParticle(&quot;fireworks_Dayz_anim&quot;);
    
    
    //pox grenade
    static const int GRENADE_CHEM_BREAK             = RegisterParticle(&quot;contaminated_area_gas_grenade&quot;);
    //Claymore
    static const int CLAYMORE_EXPLOSION             = RegisterParticle(&quot;explosion_claymore_01&quot;);
    static const int PLASTIC_EXPLOSION              = RegisterParticle(&quot;explosion_plastic_01&quot;);
    
    // Water jet/spilling
    static const int WATER_JET                      = RegisterParticle(&quot;water_jet&quot;);
    static const int WATER_JET_WEAK                 = RegisterParticle(&quot;water_jet_weak&quot;);
    static const int WATER_SPILLING                 = RegisterParticle(&quot;water_spilling&quot;);
    
    // Drowning bubbles
    static const int DROWNING_BUBBLES               = RegisterParticle(&quot;breath_bubbles&quot;);
    
    static const int BOLT_CUPID_TAIL                = RegisterParticle(&quot;cupid_bolt&quot;);
    static const int BOLT_CUPID_HIT                 = RegisterParticle(&quot;cupid_hit&quot;);
    
    // VOLCANIC
    static const int HOTPSRING_WATERVAPOR           = RegisterParticle(&quot;hotspring_watervapor&quot;);
    static const int GEYSER_NORMAL                  = RegisterParticle(&quot;geyser_normal&quot;);
    static const int GEYSER_STRONG                  = RegisterParticle(&quot;geyser_strong&quot;);
    static const int GEYSER_SPLASH                  = RegisterParticle(&quot;geyser_strong_splash&quot;);
    static const int GEYSER_BUBBLES                 = RegisterParticle(&quot;geyser_bubbles&quot;);
    static const int VOLCANO                        = RegisterParticle(&quot;volcano_smoke&quot;);
    
    // FISHING
    static const int FISHING_SIGNAL_SPLASH          = RegisterParticle(&quot;fishing_signal_splash&quot;);
    
    // STEPS
    static const int STEP_SNOW                      = RegisterParticle(&quot;step_snow&quot;);
    static const int STEP_DESERT                    = RegisterParticle(&quot;step_desert&quot;);
    static const int STEP_SOIL                      = RegisterParticle(&quot;step_soil&quot;);
    
    // STEPS
    static const int VEHICLE_WHEEL_SNOW             = RegisterParticle(&quot;vehicle_wheel_snow&quot;);//
    static const int VEHICLE_WHEEL_GRAVEL           = RegisterParticle(&quot;vehicle_wheel_gravel&quot;);//
    static const int VEHICLE_WHEEL_ASPHALT          = RegisterParticle(&quot;vehicle_wheel_asphalt&quot;);//
    
    //TREE FALLING PARTICLES
    static const int TREE_FALLING_SNOW              = RegisterParticle(&quot;tree_falling_snow&quot;);//
    static const int TREE_SOFT_FALLING_SNOW         = RegisterParticle(&quot;tree_soft_falling_snow&quot;);//
    static const int TREE_SOFT_LARGE_FALLING_SNOW   = RegisterParticle(&quot;tree_soft_large_falling_snow&quot;);//
    static const int TREE_SMALL_FALLING_SNOW        = RegisterParticle(&quot;tree_small_falling_snow&quot;);//
    static const int TREE_FALLING_NEEDLE            = RegisterParticle(&quot;tree_falling_needle&quot;);//
    static const int TREE_FALLING_LEAF              = RegisterParticle(&quot;tree_falling_leaf&quot;);//
    static const int BUSH_FALLING_SNOW              = RegisterParticle(&quot;bush_falling_snow&quot;);//
    //TREE PASSING ParticleSource
    static const int TREE_PASSING_SNOW              = RegisterParticle(&quot;tree_passing_snow&quot;);//
    static const int BUSH_PASSING_SNOW              = RegisterParticle(&quot;bush_passing_snow&quot;);// same as bush_falling_snow
    
    
    static int RegisterParticle(string file_name)
    {
        return RegisterParticle(GetPathToParticles(), file_name);
