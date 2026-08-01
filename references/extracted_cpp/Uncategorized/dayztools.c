// Source: F:/Games/Dayz/scripts/editor/plugins/dayztools.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class DayZTool: WorkbenchPlugin
{
    void RunDayZBat(string filepath, bool wait = false)
    {
        if (filepath.Length() < 2) return;
        
        filepath.Replace(&quot;\\&quot;, &quot;/&quot;);
        
        if (filepath[1] != &quot;:&quot;)
        {
            string cwd;
            Workbench.GetCwd(cwd);
            filepath = cwd + &quot;/&quot; + filepath;
        }
        
        int index = filepath.IndexOf(&quot;/&quot;);
        int last_index = index;
        
        while(index != -1)
        {
            last_index = index;
            index = filepath.IndexOfFrom(last_index + 1, &quot;/&quot;);
        }
        
        if (last_index == -1) return;
        
        string path = filepath.Substring(0, last_index);
        string bat = filepath.Substring(last_index + 1, filepath.Length() - last_index - 1);
        /*Print(filepath);
        Print(path);
        Print(bat);*/
        Workbench.RunCmd(&quot;cmd /c \&quot;cd &quot; + path + &quot; & call &quot; + bat + &quot;\&quot;&quot;, wait);
