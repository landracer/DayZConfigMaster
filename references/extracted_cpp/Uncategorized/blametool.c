// Source: F:/Games/Dayz/scripts/editor/workbench/blametool.c Source File
// Extracted from DayZ Code Explorer by Zeroy

[WorkbenchPluginAttribute(&quot;SVN Blame&quot;, &quot;Just for testing&quot;, &quot;ctrl+b&quot;, &quot;&quot;, {&quot;ScriptEditor&quot;})]
class SVNBlamePlugin: WorkbenchPlugin
{
    [Attribute(&quot;TortoiseProc /command:blame /path:$path /startrev:1 /endrev:\&quot;HEAD\&quot; /ignoreeol /ignoreallspaces /line:$line&quot;, &quot;editbox&quot;)]
    string CommandLine;
    
    override void Run()
    {
        ScriptEditor mod = Workbench.GetModule(&quot;ScriptEditor&quot;);
        if (mod)
        {
            string file;
            string absPath;
            if (mod.GetCurrentFile(file) && Workbench.GetAbsolutePath(file, absPath))
            {
                int line = mod.GetCurrentLine();
                string command = CommandLine;
                command.Replace(&quot;$path&quot;, absPath);
                command.Replace(&quot;$line&quot;, (line + 1).ToString());
                Workbench.RunCmd(command);
            }
        }
