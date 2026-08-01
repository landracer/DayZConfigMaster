// Source: F:/Games/Dayz/scripts/3_game/client/onlineservices.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class OnlineServices
{
    static ref ScriptInvoker                                                m_FriendsAsyncInvoker       = new ScriptInvoker();
    static ref ScriptInvoker                                                m_PermissionsAsyncInvoker   = new ScriptInvoker();
    static ref ScriptInvoker                                                m_ServersAsyncInvoker       = new ScriptInvoker();
    static ref ScriptInvoker                                                m_ServerAsyncInvoker        = new ScriptInvoker();
    static ref ScriptInvoker                                                m_MuteUpdateAsyncInvoker    = new ScriptInvoker(); // DEPRECATED
    static ref ScriptInvoker                                                m_ServerModLoadAsyncInvoker = new ScriptInvoker();
    
    static BiosClientServices                                               m_ClientServices;
    static ref TrialService                                                 m_TrialService;
    
    protected static string                                                 m_InviteServerIP;
    protected static int                                                    m_InviteServerPort;
    protected static string                                                 m_CurrentServerIP;
    protected static int                                                    m_CurrentServerPort;
    protected static ref GetServersResultRow                                m_CurrentServerInfo;
    
    
    protected static ref map<string, ref BiosFriendInfo>                    m_FriendsList;
    protected static ref map<string, bool>                                  m_MuteList;
    protected static ref map<string, ref BiosPrivacyPermissionResultArray>  m_PermissionsList;
    
    protected static bool                                                   m_FirstFriendsLoad          = true;
    protected static bool                                                   m_MultiplayState            = false;
    protected static ref array<string>                                      m_PendingInvites;
 
    protected static ref BiosUser                                           m_BiosUser;
    
    static void Init()
    {
        #ifdef PLATFORM_CONSOLE
            #ifndef PLATFORM_WINDOWS // if app is not on Windows with -XBOX parameter
            if ( !m_TrialService )
                m_TrialService = new TrialService;
            if ( !m_FriendsList )
                m_FriendsList = new map<string, ref BiosFriendInfo>;
            if ( !m_MuteList )
                m_MuteList = new map<string, bool>;
            if ( !m_PermissionsList )
                m_PermissionsList = new map<string, ref BiosPrivacyPermissionResultArray>;
            
            m_FriendsList.Clear();
            m_PermissionsList.Clear();
            m_MuteList.Clear();
            #endif
        #endif
        
        GetClientServices();
