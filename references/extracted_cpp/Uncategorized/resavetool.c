// Source: F:/Games/Dayz/scripts/editor/workbench/resavetool.c Source File
// Extracted from DayZ Code Explorer by Zeroy

[WorkbenchPluginAttribute(&quot;Re-Save Tool&quot;, &quot;Saves all files with given extension&quot;, &quot;&quot;, &quot;&quot;, {&quot;ResourceManager&quot;})]
class ResavePlugin: WorkbenchPlugin
{
    [Attribute(&quot;.layout&quot;, &quot;editbox&quot;, &quot;File extension&quot; )]
    string Extension;
    WBModuleDef m_module;
    
    void Resave(string file)
    {
        Print(&quot;Resaving: &quot; + file);
        m_module.SetOpenedResource(file);   
        m_module.Save();
