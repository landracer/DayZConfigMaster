// Source: F:/Games/Dayz/scripts/4_world/classes/basebuilding/constructionpart.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ConstructionPart
{
    string  m_Name;             //localized text name that is displayed ingame
    int     m_Id;               //a number used for synchronization and persistence purposes, must be unique and withing sync limit (total number of bits in all sync/persistence variables)
    string  m_PartName;         //config class name
    string  m_MainPartName;     //main (parent) config class name
    bool    m_IsBuilt;          //defines part build state
    bool    m_IsBase;           //defines if this part is the foundation of the whole construction
    bool    m_IsGate;           //defines part gate state
    ref array<string> m_RequiredParts; //list of parts required by this part
    
    void ConstructionPart( string name, string part_name, string main_part_name, int id, bool is_built, bool is_base, bool is_gate, array<string> required_parts )
    {
        m_Name = name;
        m_PartName = part_name;
        m_MainPartName = main_part_name;
        m_Id = id;
        m_IsBuilt = is_built;
        m_IsBase = is_base;
        m_IsGate = is_gate;
        m_RequiredParts = required_parts;
