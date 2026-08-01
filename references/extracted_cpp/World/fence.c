// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/basebuildingbase/fence.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class Fence extends BaseBuildingBase
{
    const int GATE_STATE_NONE               = 0;
    const int GATE_STATE_PARTIAL            = 1;
    const int GATE_STATE_FULL               = 2;
    
    const string ATTACHMENT_SLOT_COMBINATION_LOCK   = &quot;Att_CombinationLock&quot;;
    const string SOUND_GATE_OPEN_START              = &quot;DoorWoodTowerOpen_SoundSet&quot;;
    const string SOUND_GATE_CLOSE_START             = &quot;DoorWoodTowerClose_start_SoundSet&quot;;
    const string SOUND_GATE_CLOSE_END               = &quot;DoorWoodTowerClose_end_SoundSet&quot;;
 
    //gate openining
    const float GATE_ROTATION_ANGLE_DEG             = 100;
    const float GATE_ROTATION_TIME_APPROX           = 2000;     //ms
    
    const float MAX_ACTION_DETECTION_ANGLE_RAD      = 1.3;      //1.3 RAD = ~75 DEG
    const float MAX_ACTION_DETECTION_DISTANCE       = 2.0;      //meters
    
    typename ATTACHMENT_WOODEN_LOG          = WoodenLog;
    typename ATTACHMENT_COMBINATION_LOCK    = CombinationLock;
    
    string ATTSLOT_CAMONET              = &quot;Wall_Camonet&quot;;
    string ATTSLOT_BARBEDWIRE_DOWN      = &quot;Wall_Barbedwire_1&quot;;
    string ATTSLOT_BARBEDWIRE_UP        = &quot;Wall_Barbedwire_2&quot;;
    
    //protected bool m_HasHinges                = false;
    //protected bool m_GateFullyConstructed     = false;
    protected bool m_ToDiscard              = false; //for legacy OnStoreLoad handling
    protected bool m_IsOpened               = false;
    protected bool m_IsOpenedClient         = false;
    protected int m_GateState               = 0;
    
    protected EffectSound m_SoundGate_Start;
    protected EffectSound m_SoundGate_End;
    
    void Fence()
    {
        //synchronized variables
        //RegisterNetSyncVariableBool( &quot;m_HasHinges&quot; );
        //RegisterNetSyncVariableBool( &quot;m_GateFullyConstructed&quot; );
        RegisterNetSyncVariableBool( &quot;m_IsOpened&quot; );
        RegisterNetSyncVariableInt( &quot;m_GateState&quot; );
