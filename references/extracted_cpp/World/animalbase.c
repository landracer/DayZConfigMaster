// Source: F:/Games/Dayz/scripts/4_world/entities/creatures/animals/animalbase.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class AnimalBase extends DayZAnimal
{
    protected ref ArrowManagerBase m_ArrowManager;
    
    void AnimalBase()
    {
        m_ArrowManager = new ArrowManagerBase(this);
        CacheSkinningBloodInfectionChance(eAgents.SALMONELLA);
