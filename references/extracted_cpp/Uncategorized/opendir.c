// Source: F:/Games/Dayz/scripts/editor/workbench/opendir.c Source File
// Extracted from DayZ Code Explorer by Zeroy

[WorkbenchPluginAttribute(&quot;Open Dir&quot;, &quot;Just for testing&quot;, &quot;ctrl+-&quot;, &quot;&quot;, {&quot;ScriptEditor&quot;})]
class OpenDirPlugin: WorkbenchPlugin
{
    override void Run()
    {
        ScriptEditor mod = Workbench.GetModule(&quot;ScriptEditor&quot;);
        if (mod)
        {
            string file;
            string absPath;
            if (mod.GetCurrentFile(file) && Workbench.GetAbsolutePath(file, absPath))
            {
                if (absPath.Length() < 2) return;
                
                absPath.Replace(&quot;\\&quot;, &quot;/&quot;);
                
                if (absPath[1] != &quot;:&quot;)
                {
                    string cwd;
                    Workbench.GetCwd(cwd);
                    absPath = cwd + &quot;/&quot; + absPath;
                }
                
                int index = absPath.IndexOf(&quot;/&quot;);
                int last_index = index;
                
                while(index != -1)
                {
                    last_index = index;
                    index = absPath.IndexOfFrom(last_index + 1, &quot;/&quot;);
                }
                
                if (last_index == -1) return;
                
                string path = absPath.Substring(0, last_index);
                string command;
                command.Replace(&quot;$path&quot;, path);
                //Print(path);
                //Workbench.RunCmd(command);
                Workbench.RunCmd(&quot;cmd /c \&quot;start &quot; + path +&quot;\&quot;&quot;);
            }
        }
