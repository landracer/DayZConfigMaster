// Source: F:/Games/Dayz/scripts/5_mission/gui/scriptconsoleconfigtab.c Source File
// Extracted from DayZ Code Explorer by Zeroy

typedef Param5<bool, string, int, string, int> ConfigParams; // param1 - isCollapsed, param2 - string name, param3 - num of childs, param4 - path, param5 - deep
typedef Param6<bool, string, int, string, int, string> ConfigParamsEx; // param1 - isCollapsed, param2 - string name, param3 - num of childs, param4 - path, param5 - deep
 
class ScriptConsoleConfigTab : ScriptConsoleTabBase
{
    
    protected static string                         m_ConfigTextField;
    protected static string                         m_VariableTextField;
    protected static ref ConfigParamsEx             m_ConfigData;
    protected ref TStringArray                      m_BaseConfigClasses = new TStringArray;
    protected ref TStringArray                      m_BaseConfigClassesToggled = new TStringArray;
    protected ref map<CheckBoxWidget, int>          m_ClassCheckboxes = new map<CheckBoxWidget, int>;
 
    protected EditBoxWidget         m_ObjectConfigFilter;
    protected EditBoxWidget         m_VariableConfigFilter;
    protected TextListboxWidget     m_ConfigHierarchyTextListbox;
    protected TextListboxWidget     m_ConfigVariablesTextListbox;
    protected ButtonWidget          m_SelectedRowCopy;
    protected ButtonWidget          m_DumpParamButton;
    protected TextWidget            m_ClassPath;
    
    
    protected Widget                m_WgtClassesConfig;
    protected PluginConfigViewer    m_ModuleConfigViewer;
    protected int                   m_Row;
    
    void ScriptConsoleConfigTab(Widget root, ScriptConsole console, Widget button, ScriptConsoleTabBase parent = null)
    {
        m_WgtClassesConfig              = root.FindAnyWidget(&quot;ClassesPanel&quot;);
        m_DumpParamButton               = ButtonWidget.Cast(root.FindAnyWidget(&quot;DumpParamButton&quot;));
        m_ObjectConfigFilter            = EditBoxWidget.Cast(root.FindAnyWidget(&quot;ObjectConfigFilter&quot;));
        m_VariableConfigFilter          = EditBoxWidget.Cast(root.FindAnyWidget(&quot;VariableConfigFilter&quot;));
        m_ConfigHierarchyTextListbox    = TextListboxWidget.Cast(root.FindAnyWidget(&quot;ConfigHierarchy&quot;));
        m_ConfigVariablesTextListbox    = TextListboxWidget.Cast(root.FindAnyWidget(&quot;ConfigVariables&quot;));
        m_SelectedRowCopy               = ButtonWidget.Cast(root.FindAnyWidget(&quot;SelectedRowCopy&quot;));
        m_ClassPath                     = TextWidget.Cast(root.FindAnyWidget(&quot;ClassPath&quot;));
        
        if (m_ConfigTextField)
            m_ObjectConfigFilter.SetText(m_ConfigTextField);
        if (m_VariableTextField)
            m_VariableConfigFilter.SetText(m_VariableTextField);
        Init();
