// ======================================================================
// File: biospackageservice_8c_source.html
// Category: other
// ======================================================================

class BiosCheckUpdateResult
{
bool m_IsUpdate;
bool m_IsMandatory;
};
class BiosPackageService
{
proto native EBiosError CheckUpdateAsync();
proto native EBiosError PromptUpdateAsync();
proto native EBiosError ShowStoreAsync();
void OnCheckUpdate(BiosCheckUpdateResult checkUpdateResult, EBiosError error)
{
if ( !error && ( checkUpdateResult.m_IsUpdate || checkUpdateResult.m_IsMandatory ) )
{
OnlineServices.PromptUpdate();
}
}
void OnPromptUpdate(EBiosError error)
{
OnlineServices.ErrorCaught( error );
}
void OnShowStore(EBiosError error)
{
OnlineServices.ErrorCaught( error );
}
};