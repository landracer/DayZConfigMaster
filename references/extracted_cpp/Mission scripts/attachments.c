// Source: F:/Games/Dayz/scripts/5_mission/gui/inventorynew/attachments.c Source File
// Extracted from DayZ Code Explorer by Zeroy

const int ITEMS_IN_ROW = 8;
 
class Attachments
{
    protected Container                         m_Parent;
    protected EntityAI                          m_Entity;
    protected ref AttachmentsWrapper            m_AttachmentsContainer;
    protected ref array<string>                 m_AttachmentSlotNames;
    protected ref array<string>                 m_AttachmentSlotDisplayable;
    protected ref map<int, SlotsIcon>           m_AttachmentSlots;
    protected ref array<int>                    m_AttachmentIDOrdered;
    
    protected int m_RowIndex;
    protected int m_AttachmentSlotID = -1;
 
    void Attachments( Container parent, EntityAI entity )
    {
        m_Parent                    = parent;
        m_Entity                    = entity;
        m_AttachmentSlots           = new map<int, SlotsIcon>;
        m_AttachmentIDOrdered       = new array<int>;
        m_AttachmentSlotNames       = GetItemSlots( entity );
        m_AttachmentSlotDisplayable = new array<string>;
        m_Entity.GetOnItemAttached().Insert( AttachmentAdded );
        m_Entity.GetOnItemDetached().Insert( AttachmentRemoved );
        m_Entity.GetOnAttachmentSetLock().Insert( OnAttachmentReservationSet );
        m_Entity.GetOnAttachmentReleaseLock().Insert( OnAttachmentReservationRelease );
        
