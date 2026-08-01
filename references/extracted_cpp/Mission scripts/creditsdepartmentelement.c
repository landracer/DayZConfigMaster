// Source: F:/Games/Dayz/scripts/5_mission/gui/newui/credits/elements/creditsdepartmentelement.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class CreditsDepartmentElement extends CreditsElement
{
    protected TextWidget                                m_DepartmentTitle;
    protected GridSpacerWidget                          m_DepartmentSections;
    protected ref array<ref CreditsDepartmentSection>   m_DepartmentSectionEntries = new array<ref CreditsDepartmentSection>;
    
    void CreditsDepartmentElement( int index, Widget parent, JsonDataCreditsDepartment department_data )
    {
        m_Root                  = g_Game.GetWorkspace().CreateWidgets( &quot;gui/layouts/new_ui/credits/department_element/department_element.layout&quot;, parent );
        m_DepartmentTitle       = TextWidget.Cast( m_Root.FindAnyWidget( &quot;department_title&quot; ) );
        m_DepartmentSections    = GridSpacerWidget.Cast( m_Root.FindAnyWidget( &quot;department_sections&quot; ) );
        
        if( department_data.DepartmentName != &quot;&quot; )
        {
            m_DepartmentTitle.SetText( department_data.DepartmentName );
        }
        else
        {
            m_DepartmentTitle.Show( false );
            m_Root.FindAnyWidget( &quot;SeparatorPanel&quot; ).Show( false );
        }
        
        m_Root.SetSort( index );
        LoadDataAsync( department_data.Sections );
