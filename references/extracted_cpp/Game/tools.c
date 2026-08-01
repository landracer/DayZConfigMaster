// Source: F:/Games/Dayz/scripts/3_game/tools/tools.c Source File
// Extracted from DayZ Code Explorer by Zeroy


 
//--------------------------------------------------------------------------
const int CALL_CATEGORY_SYSTEM = 0; // Runs always
const int CALL_CATEGORY_GUI = 1; // Runs always (on client)
const int CALL_CATEGORY_GAMEPLAY = 2; // Runs unless ingame menu is opened
 
const int CALL_CATEGORY_COUNT = 3;
 
// -------------------------------------------------------------------------
class CallQueueContext
{
    Class m_target;
    string m_function;
    ref Param m_params;
    bool m_valid;
 
    void CallQueueContext(Class target, string fn, Param params)
    {
        m_target = target;
        m_function = fn;
        m_params = params;
        m_valid = true;
