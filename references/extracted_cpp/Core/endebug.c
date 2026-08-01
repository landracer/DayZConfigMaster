// Source: F:/Games/Dayz/scripts/1_core/proto/endebug.c Source File
// Extracted from DayZ Code Explorer by Zeroy



proto void DumpStack();

proto void DumpStackString(out string stack);

proto void DebugBreak(bool condition = true, void param1 = NULL, void param2 = NULL, void param3 = NULL, void param4 = NULL, void param5 = NULL, void param6 = NULL, void param7 = NULL, void param8 = NULL, void param9 = NULL);

void CompileBreak();

proto void DPrint(string var);
 
enum ErrorExSeverity
{
    INFO,
    WARNING,
    ERROR,
