// Source: F:/Games/Dayz/scripts/4_world/classes/weapons/weaponmanager.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class WeaponManager
{
    const float MAX_DROP_MAGAZINE_DISTANCE_SQ = 4;
    
    protected PlayerBase                    m_player;
    
    protected int                           m_LastAcknowledgmentID;
    
    protected int                           m_PendingWeaponActionAcknowledgmentID;
    protected Magazine                      m_PendingTargetMagazine;
    protected ref InventoryLocation         m_TargetInventoryLocation;
    protected int                           m_PendingWeaponAction; 
    protected ref InventoryLocation         m_PendingInventoryLocation;
    
    protected bool                          m_canEnd;
    protected bool                          m_justStart;
    protected bool                          m_InProgress;
    protected bool                          m_IsEventSended;
    protected bool                          m_WantContinue;
    protected bool                          m_InIronSight;
    protected bool                          m_InOptic;
    protected bool                          m_readyToStart;
    protected Weapon_Base                   m_WeaponInHand;
    protected MagazineStorage               m_MagazineInHand;
    protected ActionBase                    m_ControlAction;
    protected int                           m_ForceEjectBulletTimestamp;
    
    protected const int                     FORCE_EJECT_BULLET_TIMEOUT = 2000;
#ifdef DIAG_DEVELOPER   
    protected int                           m_BurstOption;
#endif
    //Reload
    protected ref array<Magazine>           m_MagazinePilesInInventory;
    protected ref array<MagazineStorage>    m_MagazineStorageInInventory;
    protected ref array<Magazine>           m_SuitableMagazines;
    protected Magazine                      m_PreparedMagazine;
    
    //Jamming
    protected float m_NewJamChance;
    protected bool m_WaitToSyncJamChance;
    
    
    protected int m_AnimationRefreshCooldown;
    
    void WeaponManager(PlayerBase player)
    {
        m_ForceEjectBulletTimestamp = -1;
        m_player = player;
        m_PendingWeaponActionAcknowledgmentID = -1;
        m_PendingTargetMagazine = NULL;
        m_PendingInventoryLocation = NULL;
        m_WeaponInHand = NULL;
        m_MagazineInHand = NULL;
        m_ControlAction = NULL;
        m_PendingWeaponAction = -1;
        m_LastAcknowledgmentID = 1;
        m_InProgress = false;
        m_WantContinue = true;
        m_IsEventSended = false;
        m_canEnd = false;
        m_readyToStart = false;
        m_AnimationRefreshCooldown = 0;
        
        m_NewJamChance = -1;
        m_WaitToSyncJamChance = false;
        
        m_MagazinePilesInInventory = new array<Magazine>;
        m_MagazineStorageInInventory = new array<MagazineStorage>;
        m_SuitableMagazines = new array<Magazine>;
        m_PreparedMagazine = null;
        
#ifdef DEVELOPER    
        m_BurstOption = 0;
#endif
