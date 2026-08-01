// ======================================================================
// File: biosclientservices_8c_source.html
// Category: other
// ======================================================================

class BiosClientServices
{
private void BiosClientServices() {}
private void ~BiosClientServices() {}
proto native BiosPrivacyService GetPrivacyService();
proto native BiosSocialService GetSocialService();
proto native BiosLobbyService GetLobbyService();
proto native BiosSessionService GetSessionService();
proto native BiosPackageService GetPackageService();
};