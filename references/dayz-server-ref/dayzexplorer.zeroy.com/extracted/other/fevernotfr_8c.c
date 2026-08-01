// ======================================================================
// File: fevernotfr_8c_source.html
// Category: other
// ======================================================================

class FeverNotfr: NotifierBase
{
void FeverNotfr(NotifiersManager manager)
{
m_Active = false;
}
override int GetNotifierType()
{
return eNotifiers.NTF_FEVERISH;
}
};