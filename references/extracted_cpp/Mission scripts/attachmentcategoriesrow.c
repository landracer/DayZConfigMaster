// Source: F:/Games/Dayz/scripts/5_mission/gui/inventorynew/inherited/attachmentcategoriesrow.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class AttachmentCategoriesRow: ClosableContainer
{
    protected string                                m_CategoryIdentifier;
    protected string                                m_RowConfigPath;
    protected bool                                  m_Refreshing;
    
    protected ref AttachmentsGroupContainer         m_AttachmentsContainer;
    protected ref map<int, ref ContainerWithCargo>  m_AttachmentCargos;
    
    void AttachmentCategoriesRow( LayoutHolder parent, int sort = -1 )
    {
        m_Refreshing = false;
        m_AttachmentCargos      = new map<int, ref ContainerWithCargo>;
        m_AttachmentsContainer  = new AttachmentsGroupContainer(this);
        
        m_Body.Insert(m_AttachmentsContainer);
        
        m_ClosableHeader.GetMainWidget().ClearFlags( WidgetFlags.DRAGGABLE );
        
        RecomputeOpenedContainers();
