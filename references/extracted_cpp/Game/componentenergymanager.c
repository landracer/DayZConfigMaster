// Source: F:/Games/Dayz/scripts/3_game/tools/component/componentenergymanager.c Source File
// Extracted from DayZ Code Explorer by Zeroy

//-----------------------------
// ENERGY MANAGER
//-----------------------------
/*
Author: Boris Vacula
 
Documentation can be found at DayZ Confluence >> Camping & Squatting >> Electricity >> Energy Manager functionalities
 
This system controls storage, spending and sharing of energy between instances.
 
Every EntityAI object which uses this API gains these functions:
    -It can store some amout of energy
    -It can use this amount of energy for any kind of functionality
    -It can share this energy with other devices plugged into it
    -It will have an ON/OFF switch
*/
 
class ComponentEnergyManager : Component
{
    protected const  float DEFAULT_UPDATE_INTERVAL = 15;
    protected static bool           m_DebugPlugs = false; //true; // Use this to toggle visualisation of plug connections
    protected       Shape           m_DebugPlugArrow;
    
    protected       bool            m_IsSwichedOn;
    protected       bool            m_IsSwichedOnPreviousState; // Necesarry due to synchronization of m_IsSwichedOn
    protected       bool            m_IsPassiveDevice;
    protected       bool            m_IsWorking;
    protected       bool            m_CanWork;
    protected       bool            m_CanStopWork;
    protected       bool            m_RestorePlugState; // After server restart, this value reports if this device was plugged into something or not at the end of last session.
    protected       bool            m_AutoSwitchOff;
    protected       bool            m_ShowSocketsInInventory;
    protected       bool            m_HasElectricityIcon; // Electricity icon over the item in inventory
    protected       bool            m_AutoSwitchOffWhenInCargo;
    protected       bool            m_IsPlugged; // Synchronized variable
    protected       bool            m_ConvertEnergyToQuantity;
    
    protected       int             m_MySocketID = -1;
    protected       int             m_PlugType;
    protected       int             m_EnergySourceStorageIDb1; // Storage persistence ID
    protected       int             m_EnergySourceStorageIDb2; // Storage persistence ID
    protected       int             m_EnergySourceStorageIDb3; // Storage persistence ID
    protected       int             m_EnergySourceStorageIDb4; // Storage persistence ID
    protected       int             m_AttachmentActionType;
    protected       int             m_EnergySourceNetworkIDLow = -1;  // Network ID
    protected       int             m_EnergySourceNetworkIDHigh = -1; // Network ID
    
    protected       float           m_EnergyUsage;
    protected       float           m_Energy;
    protected       float           m_EnergyAtSpawn;
    protected       float           m_EnergyStorageMax;
    protected       float           m_ReduceMaxEnergyByDamageCoef;
    protected       float           m_SocketsCount;
    protected       float           m_CordLength;
    protected       float           m_LastUpdateTime;
    protected       float           m_WetnessExposure;
    protected       float           m_UpdateInterval; // Interval of OnWork(...) calls and device updates.
    
    protected       string          m_CordTextureFile;
    
    // Concatenated strings for p3d selections
    protected static const string   SOCKET_             = &quot;socket_&quot;;
    protected static const string   _PLUGGED            = &quot;_plugged&quot;;
    protected static const string   _AVAILABLE          = &quot;_available&quot;;
    static const string             SEL_CORD_PLUGGED    = &quot;cord_plugged&quot;;
    static const string             SEL_CORD_FOLDED     = &quot;cord_folded&quot;;
    
    protected ref TIntArray         m_CompatiblePlugTypes;
    EntityAI                        m_EnergySource; // Energy source can be any EntityAI object
    ref array<EntityAI>             m_PluggedDevices;
    ref map<string,EntityAI>        m_DeviceByPlugSelection;
    
    ref Timer                       m_UpdateTimer;
    ref Timer                       m_UpdateQuantityTimer;
    ref Timer                       m_DebugUpdate;
    
    const int MAX_SOCKETS_COUNT     = 4;
    EntityAI m_Sockets[MAX_SOCKETS_COUNT];
    
    
 
    // Constructor
    void ComponentEnergyManager()
    {
        // Disable debug arrows on public release, so that they don&#39;t use their timers.
        #ifndef DEVELOPER
        m_DebugPlugs = false;
        #endif
