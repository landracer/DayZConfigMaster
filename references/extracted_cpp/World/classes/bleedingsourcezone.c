// Source: F:/Games/Dayz/scripts/4_world/classes/bleedingsources/bleedingsourcezone.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class BleedingSourceZone
{
    string      m_FireSelectionName;
    int         m_Bit;
    vector      m_EmitterOffset;
    vector      m_EmitterOrientation;
    string      m_Bone;
    float       m_FlowModifier;
    int         m_MaxTime;
    string      m_ParticleName;
    int         m_InventorySlotLocation;
    
    void BleedingSourceZone(string selection_name, int bit, vector offset, vector orientation, string bone, int max_time, float flow_modifier, string particle_name)
    {
        m_FireSelectionName = selection_name;
        m_Bit = bit;
        m_EmitterOffset = offset;
        m_EmitterOrientation = orientation;
        m_Bone = bone;
        m_FlowModifier = flow_modifier;
        m_MaxTime = max_time;
        m_ParticleName = particle_name;
