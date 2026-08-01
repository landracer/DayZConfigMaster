// ======================================================================
// File: enprofiler_8c_source.html
// Category: other
// ======================================================================

enum EnProfilerFlags
{
NONE,
RESET,
RECURSIVE,
ALL
};
enum EnProfilerModule
{
CORE,
GAMELIB,
GAME,
WORLD,
MISSION,
MISSION_CUSTOM,
ERROR,
};
enum EnProfilerEnabledFlags
{
NONE,
DIAG,
SCRP,
SCRC,
};
typedef Param2<float, typename> EnProfilerTimeClassPair;
typedef Param2<int, typename> EnProfilerCountClassPair;
typedef Param2<float, string> EnProfilerTimeFuncPair;
typedef Param2<int, string> EnProfilerCountFuncPair;
class EnProfiler
{
static proto void Enable(bool enable, bool immediate = false, bool sessionReset = true);
static proto int IsEnabled();
static bool IsEnabledD()
{
return (IsEnabled() & EnProfilerEnabledFlags.DIAG);
}
static bool IsEnabledP()
{
return (IsEnabled() & EnProfilerEnabledFlags.SCRP);
}
static bool IsEnabledC()
{
return (IsEnabled() & EnProfilerEnabledFlags.SCRC);
}
static proto void SortData();
static proto void ResetSession(bool fullReset = false);
static proto int SetFlags(int flags, bool sessionReset = true);
static proto int GetFlags();
static proto bool IsFlagsSet(int flags);
static proto int AddFlags(int flags, bool sessionReset = true);
static proto int RemoveFlags(int flags, bool sessionReset = true);
static proto int ClearFlags(bool sessionReset = true);
static proto void SetModule(EnProfilerModule module, bool sessionReset = true);
static proto EnProfilerModule GetModule();
static proto owned string ModuleToName(EnProfilerModule module);
static proto bool NameToModule(string moduleName, out EnProfilerModule module);
static proto void SetInterval(int interval, bool sessionReset = true);
static proto int GetInterval();
static proto void SetTimeResolution(int resolution);
static proto int GetTimeResolution();
static proto void EnableAverage(bool enable);
static proto bool IsAverage();
static proto void Dump();
static proto int GetGameFrame();
static proto int GetSessionFrame();
static proto int GetTotalFrames();
static proto int GetProfiledSessionFrames();
static proto int GetProfiledFrames();
static proto void GetTimePerClass(notnull out array<ref EnProfilerTimeClassPair> outArr, int count = int.MAX);
static proto void GetAllocationsPerClass(notnull out array<ref EnProfilerCountClassPair> outArr, int count = int.MAX);
static proto void GetInstancesPerClass(notnull out array<ref EnProfilerCountClassPair> outArr, int count = int.MAX);
static proto void GetTimePerFunc(notnull out array<ref EnProfilerTimeFuncPair> outArr, int count = int.MAX);
static proto void GetCountPerFunc(notnull out array<ref EnProfilerCountFuncPair> outArr, int count = int.MAX);
static proto float GetTimeOfClass(typename clss, bool immediate = false);
static proto int GetAllocationsOfClass(typename clss, bool immediate = false);
static proto int GetInstancesOfClass(typename clss, bool immediate = false);
static proto float GetTimeOfFunc(string funct, typename clss, bool immediate = false);
static proto float GetTimeOfFuncG(string funct, bool immediate = false);
static proto int GetCountOfFunc(string funct, typename clss, bool immediate = false);
static proto int GetCountOfFuncG(string funct, bool immediate = false);
static bool RequestImmediateData()
{
// I only care if it is actually profiling right now, so C
bool wasEnabled = IsEnabledC();
if (!wasEnabled)
{
// I want the data, and I want it now, so immediate
Enable(true, true);
}
return wasEnabled;
}
};