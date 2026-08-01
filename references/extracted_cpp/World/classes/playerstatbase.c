// Source: F:/Games/Dayz/scripts/4_world/classes/playerstats/playerstatbase.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class PlayerStatBase
{   
    protected Man m_Player;
 
    int m_Type;
    void    OnStoreSave( ParamsWriteContext ctx );
    bool    OnStoreLoad( ParamsReadContext ctx);
    void    OnRPC(ParamsReadContext ctx);
    void    OnAfterStoreLoad();
    float   Get();
    string  GetLabel();
    void    SetByFloat(float value);
    void    SetByFloatEx(float value, string system = &quot;&quot; );
    bool    IsSynced();
    array<PlayerStatRecord> GetRecords();
    void    Init(int id);
    void    SerializeValue(array<ref StatDebugObject> objects, int flags);
    float   GetNormalized();
    float   GetMax();
    float   GetMin();
    int     GetType()
    {
        return m_Type;
