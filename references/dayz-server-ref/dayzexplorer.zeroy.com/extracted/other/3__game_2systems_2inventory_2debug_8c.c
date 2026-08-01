// ======================================================================
// File: 3__game_2systems_2inventory_2debug_8c_source.html
// Category: other
// ======================================================================

void syncDebugPrint (string s)
{
#ifdef LOG_TO_RPT
PrintToRPT("" + s); // comment/uncomment to hide/see debug logs
#endif
#ifdef LOG_TO_SCRIPT
Print(string.Format("%1", s));
#endif
}
void actionDebugPrint (string s)
{
#ifdef LOG_TO_RPT
PrintToRPT("" + s); // comment/uncomment to hide/see debug logs
#endif
#ifdef LOG_TO_SCRIPT
Print(string.Format("%1", s));
#endif
}
void inventoryDebugPrint (string s)
{
#ifdef LOG_TO_RPT
PrintToRPT("" + s); // comment/uncomment to hide/see debug logs
#endif
#ifdef LOG_TO_SCRIPT
Print(string.Format("%1", s));
#endif
}
#ifdef DIAG_DEVELOPER
class InventoryDebug
{
static bool m_IsHandAckEnable;
static void SetHandAckEnable(bool enable)
{
m_IsHandAckEnable = enable;
}
static bool IsHandAckEnable()
{
return m_IsHandAckEnable;
}
}
#endif