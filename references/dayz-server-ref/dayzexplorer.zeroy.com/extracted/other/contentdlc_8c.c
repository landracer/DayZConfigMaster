// ======================================================================
// File: contentdlc_8c_source.html
// Category: other
// ======================================================================

// Binded values from engine
enum EDLCId
{
DLC_UNKNOWN,
DLC_FROSTLINE,
};
class ContentDLC
{
ref ScriptInvoker m_OnChange = new ScriptInvoker();
proto native int GetEntitlements(out TIntArray dlcIdList);
proto native bool IsDLCInstalled(EDLCId dlcId);
bool OwnsAllDLC()
{
const int length = EnumTools.GetEnumSize(EDLCId);
// Start at 1, because 0 is DLC_UNKNOWN
for (int i = 1; i < length; ++i)
{
if (!IsDLCInstalled(EnumTools.GetEnumValue(EDLCId, i)))
return false;
}
return true;
}
void OnChange(EDLCId dlcId)
{
m_OnChange.Invoke(dlcId);
}
};