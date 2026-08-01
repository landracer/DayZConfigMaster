// Source: F:/Games/Dayz/scripts/3_game/tools/logtemplates.c Source File
// Extracted from DayZ Code Explorer by Zeroy

typedef Param3<string, string, string> LogTemplate;
typedef int LogTemplateID;
 
/*
 * OBSOLETE: kept for possible backward compatibility only
 */
class LogTemplates
{
    static private ref map<LogTemplateID, ref LogTemplate> m_LogTemplates;
    
    static private void RegisterLogTamplate(LogTemplateID template_id, string author, string plugin, string label)
    {
        if ( m_LogTemplates == NULL )
        {
            m_LogTemplates  = new map<LogTemplateID, ref LogTemplate>;
        }
        
        if ( m_LogTemplates.Contains(template_id) )
        {
            Debug.Log(&quot;Template ID: &quot;+string.ToString(template_id)+&quot; is alredy exist!&quot;, &quot;LogTemplate.h -> OnInit()&quot;, &quot;System&quot;, &quot;Template Registration&quot;, &quot;None&quot;);
        }
        else
        {
            LogTemplate params = new LogTemplate(author, plugin, label);
            m_LogTemplates.Set(template_id, params);
        }
