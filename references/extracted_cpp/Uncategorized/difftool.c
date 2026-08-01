// Source: F:/Games/Dayz/scripts/editor/workbench/difftool.c Source File
// Extracted from DayZ Code Explorer by Zeroy

[WorkbenchPluginAttribute(&quot;SVN Diff&quot;, &quot;Just for testing&quot;, &quot;ctrl+`&quot;, &quot;&quot;, {&quot;ScriptEditor&quot;})]
class SVNDiffPlugin: WorkbenchPlugin
{
    [Attribute(&quot;TortoiseProc /command:diff /path:$path&quot;, &quot;editbox&quot;)]
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
                string command = CommandLine;
                command.Replace(&quot;$path&quot;, absPath);
                Workbench.RunCmd(command);
                Print( command );
                Print( absPath );
            }
        }
