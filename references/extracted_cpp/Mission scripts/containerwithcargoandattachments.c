// Source: F:/Games/Dayz/scripts/5_mission/gui/inventorynew/inherited/containerwithcargoandattachments.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ContainerWithCargoAndAttachments extends ClosableContainer
{
    protected ref Attachments                               m_Atts;
    protected ref CargoContainer                            m_CargoGrid;
    
    protected ref map<EntityAI, ref CargoContainer>         m_AttachmentCargos;
    protected ref map<EntityAI, ref AttachmentsWrapper>     m_AttachmentAttachmentsContainers;
    protected ref map<EntityAI, ref Attachments>            m_AttachmentAttachments;
    protected ref array<int>                                m_AttachmentSlotsSorted;
    
    void ContainerWithCargoAndAttachments( LayoutHolder parent, int sort = -1 )
    {
        WidgetEventHandler.GetInstance().RegisterOnDraggingOver( m_MainWidget,  this, &quot;DraggingOverHeader2&quot; );
