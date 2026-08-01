// ======================================================================
// File: biossocialservice_8c_source.html
// Category: other
// ======================================================================

class BiosFriendInfo
{
string m_Uid;
string m_DisplayName;
bool m_IsFavorite;
bool m_IsFollowed;
static bool Compare( BiosFriendInfo a, BiosFriendInfo b )
{
return ( a.m_Uid == b.m_Uid && a.m_DisplayName == b.m_DisplayName && a.m_IsFavorite == b.m_IsFavorite && a.m_IsFollowed == b.m_IsFollowed );
}
};
typedef array<ref BiosFriendInfo> BiosFriendInfoArray;
class BiosSocialService
{
proto native EBiosError ShowUserProfileAsync(string uid_target);
proto native EBiosError GetFriendsAsync();
void OnUserProfileAsync(EBiosError error)
{
OnlineServices.OnUserProfileAsync( error );
}
void OnFriendsAsync(BiosFriendInfoArray friend_list, EBiosError error)
{
OnlineServices.OnFriendsAsync( friend_list, error );
}
};