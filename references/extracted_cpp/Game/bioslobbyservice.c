// Source: F:/Games/Dayz/scripts/3_game/services/bioslobbyservice.c Source File
// Extracted from DayZ Code Explorer by Zeroy

// ip, name, connection port, queryPort
typedef Param4<string, string, int, int> CachedServerInfo;
 
// JSON struct data for news articles
class JsonDataNewsArticle: Managed
{
    [NonSerialized()]
    int categoryID;
    
    [NonSerialized()]
    string dlcName;
    
    [NonSerialized()]
    string qrCodeImageUrl;
    
    string date;
    string excerpt;
    string category;
    string title;
    string slug;
    string coverImage;
    string fullUrl;
