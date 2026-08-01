// Source: F:/Games/Dayz/scripts/3_game/services/biossocialservice.c Source File
// Extracted from DayZ Code Explorer by Zeroy


 
class BiosFriendInfo
{
    string  m_Uid;          
    string  m_DisplayName;  
    bool    m_IsFavorite;   
    bool    m_IsFollowed;   
    
    static bool Compare( BiosFriendInfo a, BiosFriendInfo b )
    {
        return ( a.m_Uid == b.m_Uid && a.m_DisplayName == b.m_DisplayName && a.m_IsFavorite == b.m_IsFavorite && a.m_IsFollowed == b.m_IsFollowed );
