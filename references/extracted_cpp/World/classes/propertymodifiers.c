// Source: F:/Games/Dayz/scripts/4_world/classes/propertymodifiers.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class PropertyModifiers
{
    vector m_RecoilModifiers;
    vector m_SwayModifiers;
    vector m_SightMisalignment;
    
    float m_BarrelLength;
    //ref Timer test_timer;
    ItemBase m_OwnerItem;
    float m_Weapon
    
    void PropertyModifiers(ItemBase owner)
    {
        //test_timer = new Timer();
        //test_timer.Run(5, this, &quot;Refresh&quot;, NULL, true);
        m_OwnerItem = owner;
        UpdateModifiers();
        m_BarrelLength = CalculateBarrelLength(owner);
