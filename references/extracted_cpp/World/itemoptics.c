// Source: F:/Games/Dayz/scripts/4_world/entities/core/inherited/itemoptics.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ItemOptics : InventoryItemSuper
{
    bool                m_data_set;
    bool                m_allowsDOF; //true if optics DOES NOT have magnification (FOV >= DZPLAYER_CAMERA_FOV_IRONSIGHTS)
    bool                m_reddot_displayed
    bool                m_isNVOptic = false;
    int                 m_CurrentOpticMode; //generic optic mode, currently used for NV optics only (could be expanded)
    int                 m_CurrentOpticModeLocal; //local mirror for sync purposes;
    int                 m_reddot_index;
    float               m_blur_float;
    float               m_nearplane_override; //override value for DayZPlayerCameraOptics only!
    string              m_optic_sight_texture;
    string              m_optic_sight_material;
    string              m_2D_preload_type;
    ref array<float>    m_mask_array;
    ref array<float>    m_lens_array;
    ref array<float>    m_OpticsDOFProperties;
        
    void ItemOptics()
    {
        m_mask_array = new array<float>;
        m_lens_array = new array<float>;
        m_OpticsDOFProperties = new array<float>;
        
        InitReddotData();
        InitOpticsPPInfo();
        InitCameraOverrideProperties();
        InitOpticsDOFProperties(m_OpticsDOFProperties);
        Init2DPreloadType();
        InitOpticMode();
        
        m_CurrentOpticModeLocal = -1;
        RegisterNetSyncVariableInt( &quot;m_CurrentOpticMode&quot;, 0, 63 );
