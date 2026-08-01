// ======================================================================
// File: biosprivacyservice_8c_source.html
// Category: other
// ======================================================================

enum EBiosPrivacyPermission
{
COMMUNICATE_VOICE,
COMMUNICATE_TEXT,
VIEW_PROFILE,
VIEW_PRESENCE,
};
enum EBiosPrivacyPrivilege
{
COMMUNICATE_VOICE,
COMMUNICATE_TEXT,
MULTIPLAYER_GAMEPLAY,
MULTIPLAYER_LOBBY,
};
class BiosPrivacyPermissionResult
{
EBiosPrivacyPermission m_Permission;
bool m_IsAllowed;
static bool Compare( BiosPrivacyPermissionResult a, BiosPrivacyPermissionResult b )
{
return ( a.m_Permission == b.m_Permission && a.m_IsAllowed == b.m_IsAllowed );
}
};
typedef array<ref BiosPrivacyPermissionResult> BiosPrivacyPermissionResultArray;
class BiosPrivacyUidResult
{
string m_Uid;
ref BiosPrivacyPermissionResultArray m_Results;
};
typedef array<ref BiosPrivacyUidResult> BiosPrivacyUidResultArray;
class BiosPrivacyService
{
proto native EBiosError GetPermissionsAsync(array<string> uid_list, array<EBiosPrivacyPermission> permission_list);
proto native EBiosError GetPrivilegeAsync(EBiosPrivacyPrivilege privilege, bool try_resolution);
void OnPermissions(BiosPrivacyUidResultArray result_list, EBiosError error)
{
if (result_list == null)
{
Print("biapi privacy error: " + error);
return;
}
OnlineServices.OnPermissionsAsync( result_list, error );
}
void OnPrivilege(EBiosPrivacyPrivilege privilege, EBiosError error)
{
switch ( privilege )
{
case EBiosPrivacyPrivilege.MULTIPLAYER_GAMEPLAY:
{
OnlineServices.OnLoadMPPrivilege( error );
break;
}
case EBiosPrivacyPrivilege.COMMUNICATE_VOICE:
{
OnlineServices.OnLoadVoicePrivilege( error );
break;
}
}
}
};