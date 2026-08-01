// Source: F:/Games/Dayz/scripts/2_gamelib/tools.c Source File
// Extracted from DayZ Code Explorer by Zeroy


class ScriptCallQueue
{
    proto native void Tick(float timeslice);
    proto void Call(func fn, void param1 = NULL, void param2 = NULL, void param3 = NULL, void param4 = NULL, void param5 = NULL, void param6 = NULL, void param7 = NULL, void param8 = NULL, void param9 = NULL);
    proto void CallByName(Class obj, string fnName , Param params = NULL);
    proto void CallLater(func fn, int delay = 0, bool repeat = false, void param1 = NULL, void param2 = NULL, void param3 = NULL, void param4 = NULL, void param5 = NULL, void param6 = NULL, void param7 = NULL, void param8 = NULL, void param9 = NULL);  
    proto void CallLaterByName(Class obj, string fnName, int delay = 0, bool repeat = false, Param params = NULL);  
    proto void Remove(func fn);
    proto int GetRemainingTime(func fn);
    proto void RemoveByName(Class obj, string fnName);
    proto int GetRemainingTimeByName(Class obj, string fnName);
    proto native void Clear();
