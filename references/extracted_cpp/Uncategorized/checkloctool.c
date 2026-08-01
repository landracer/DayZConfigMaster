// Source: F:/Games/Dayz/scripts/editor/workbench/checkloctool.c Source File
// Extracted from DayZ Code Explorer by Zeroy

[WorkbenchPluginAttribute(&quot;Check localisation in UI&quot;, &quot;Find non localised text in UI&quot;, &quot;ctrl+l&quot;, &quot;&quot;, {&quot;ResourceManager&quot;})]
class CheckLocalisationPlugin: WorkbenchPlugin
{
    ref array<string> m_results = new array<string>;
    WBModuleDef m_module;
    
    void FindTexts(WidgetSource src)
    {
        if (src)
        {
            int idx = src.VarIndex(&quot;text&quot;);
            
            if (idx != -1)
            {
                string text;
                if (src.Get(idx, text) && !text.Contains(&quot;#&quot;))
                {
                    m_results.Insert(&quot;Text = \&quot;&quot; + text + &quot;\&quot; in Widget: &quot; + src.GetName());
                }
            }
            
            FindTexts(src.GetChildren());
            FindTexts(src.GetSibling());
        }
