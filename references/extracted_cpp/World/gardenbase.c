// Source: F:/Games/Dayz/scripts/4_world/entities/gardenbase.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class GardenBase extends ItemBase //BuildingSuper
{
    // Paths to slot textures. Slots can have multiple states, so multiple textures must be generated
    static const string SLOT_TEXTURE_DIGGED_WET_LIME        = &quot;dz\\gear\\cultivation\\data\\soil_digged_wet_lime_CO.paa&quot;;
    static const string SLOT_TEXTURE_DIGGED_WET_PLANT       = &quot;dz\\gear\\cultivation\\data\\soil_digged_wet_plant_CO.paa&quot;;
    
    // Wet/dry material
    static const string SLOT_MATERIAL_WET                   = &quot;dz\\gear\\cultivation\\data\\soil_cultivated_wet.rvmat&quot;;
    static const string SLOT_MATERIAL_DRY                   = &quot;dz\\gear\\cultivation\\data\\soil_cultivated.rvmat&quot;;
    
    static const string SLOT_MATERIAL_LIMED_WET             = &quot;dz\\gear\\cultivation\\data\\soil_cultivated_limed_wet.rvmat&quot;;
    static const string SLOT_MATERIAL_LIMED_DRY             = &quot;dz\\gear\\cultivation\\data\\soil_cultivated_limed.rvmat&quot;;
    static const string SLOT_MATERIAL_COMPOST_WET           = &quot;dz\\gear\\cultivation\\data\\soil_cultivated_compost_wet.rvmat&quot;;
    static const string SLOT_MATERIAL_COMPOST_DRY           = &quot;dz\\gear\\cultivation\\data\\soil_cultivated_compost.rvmat&quot;;
    
    // slot names -> MUST BE LOWERCASE
    private static const string SLOT_SELECTION_DIGGED_PREFIX    = &quot;seedbase_&quot;;
    private static const string SLOT_SELECTION_COVERED_PREFIX   = &quot;slotCovered_&quot;;
    private static const string SLOT_MEMORY_POINT_PREFIX        = &quot;slot_&quot;;
    private static const string SLOT_SEEDBASE_PREFIX            = &quot;seedbase_&quot;;
    
    private static const int    CHECK_RAIN_INTERVAL             = 15;
    private static const float  CREATE_PLANT_DELAY              = 1000.0;
    private static const float  RAIN_INTENSITY_THRESHOLD        = 0.05;
    
    protected ref array<ref Slot> m_Slots;
    protected int   m_SlotFertilityState = 0;   //Used to store fertility state of all slots
    protected int   m_SlotWateredState = 0;     //Used to store watered state of all slots
    protected int   m_MaxWateredStateVal = 0;   //Used to store water max state of all slots
    
    protected int   m_SlotState = 0;            //Used to handle and sync slot states
    
    protected int m_SlotWaterBitmap0; // Slots 1–4
    protected int m_SlotWaterBitmap1; // Slots 5–8
    protected int m_SlotWaterBitmap2; // Slot 9 (and could hold up to 3 more if needed)
    
    protected int m_SlotFertilizerBitmap0; // Slots 1–4
    protected int m_SlotFertilizerBitmap1; // Slots 5–8
    protected int m_SlotFertilizerBitmap2; // Slot 9 (and could hold up to 3 more if needed)
    
    protected float m_DefaultFertility = 1;
    ref Timer       m_CheckRainTimer;
    
    private static ref map<string,string> m_map_slots; // For the &#39;attachment slot -> plant slot&#39; conversion. It is possible that this will be removed later.
    
    void GardenBase()
    {
        RegisterNetSyncVariableInt(&quot;m_SlotState&quot;);
        RegisterNetSyncVariableInt(&quot;m_SlotFertilityState&quot;);
        RegisterNetSyncVariableInt(&quot;m_SlotWateredState&quot;);
        
        RegisterNetSyncVariableInt(&quot;m_SlotWaterBitmap0&quot;);
        RegisterNetSyncVariableInt(&quot;m_SlotWaterBitmap1&quot;);
        RegisterNetSyncVariableInt(&quot;m_SlotWaterBitmap2&quot;);
        
        RegisterNetSyncVariableInt(&quot;m_SlotFertilizerBitmap0&quot;);
        RegisterNetSyncVariableInt(&quot;m_SlotFertilizerBitmap1&quot;);
        RegisterNetSyncVariableInt(&quot;m_SlotFertilizerBitmap2&quot;);
        
        m_map_slots = new map<string,string>;
        
        SetEventMask(EntityEvent.INIT); // Enable EOnInit event
        
        // Prepare m_map_slots
        for (int i = 1; i <= GetGardenSlotsCount(); ++i)
        {
            // m_map_slots is supposed to be: <input, output>
            string input = SLOT_SEEDBASE_PREFIX + i.ToString();
            string output = SLOT_MEMORY_POINT_PREFIX;
            
            if (i < 10)
                output = output + &quot;0&quot;; // Example: &#39;1&#39; changes to &#39;01&#39;
            
            output = output + i.ToString();
            
            m_map_slots.Set(input, output);
        }
                
        InitializeSlots();
        SetMaxWaterStateVal();
        
        if (g_Game.IsServer())
        {
            CheckRainStart();
        }
