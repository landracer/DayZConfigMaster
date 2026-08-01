// Source: F:/Games/Dayz/scripts/1_core/workbenchapi.c Source File
// Extracted from DayZ Code Explorer by Zeroy

typedef int[] WBModuleDef;
typedef int[] ScriptEditor;
typedef int[] ResourceBrowser;
typedef int[] WorldEditor;
 
class Workbench
{
    static proto native WBModuleDef GetModule(string type);
    static proto native bool OpenModule(string type);
    static proto native bool CloseModule(string type);
    static proto native void Dialog(string caption, string text);
    static proto int ScriptDialog(string caption, string text, Class data);
    static proto bool SearchResources(string filter, func callback);
    static proto native int RunCmd(string command, bool wait = false);
    static proto void GetCwd(out string currentDir);
    static proto bool GetAbsolutePath(string relativePath, out string absPath);
