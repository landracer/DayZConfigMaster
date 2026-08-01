// Source: F:/Games/Dayz/scripts/3_game/tools/jsonobject.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class JsonObject
{   
    ref map<string, string>         m_Strings;
    ref map<string, int>            m_Ints;
    ref map<string, float>          m_Floats;
    ref map<string, bool>           m_Bools;
    ref map<string, ref Vector2>    m_Vectors2;
        
    void JsonObject()
    {
        m_Strings   = new map<string, string>;
        m_Ints      = new map<string, int>;
        m_Floats    = new map<string, float>;
        m_Bools     = new map<string, bool>;
        m_Vectors2  = new map<string, ref Vector2>;
