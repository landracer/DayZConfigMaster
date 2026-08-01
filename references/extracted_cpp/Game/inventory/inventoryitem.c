// Source: F:/Games/Dayz/scripts/3_game/entities/inventoryitem.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class InventoryItem extends EntityAI
{   
    static private const float SOUND_CONTACT_SKIP = 0.33;//second
    
#ifdef DIAG_DEVELOPER
    static private ref array<ref string> s_ImpactSoundsInfo = new array<ref string>();
#endif
 
    private SoundLookupTable m_SoundImpactTable;
    private float m_SoundContactTickTime;
    private bool m_IsMeleeWeapon = false;
    
    proto native InventoryItemType GetInventoryItemType();

    proto native void SwitchOn(bool onOff);
    proto native bool IsOn();
    
    proto native void EnableCollisionsWithCharacter(bool state);
    proto native bool HasCollisionsWithCharacter();
 
    proto native MeleeCombatData GetMeleeCombatData();  
    
    proto native void ThrowPhysically(DayZPlayer player, vector force, bool collideWithCharacters = true);

    //  This method performs an OR operation with the config &#39;forceFarBubble&#39;. If set in the config 
    //  this method has no effect. 
    proto native void ForceFarBubble(bool state);
    
    void InventoryItem()
    {
        InitImpactSoundData();
        
        if (ConfigIsExisting(&quot;isMeleeWeapon&quot;))
            m_IsMeleeWeapon = ConfigGetBool(&quot;isMeleeWeapon&quot;);
