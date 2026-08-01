// Source: F:/Games/Dayz/scripts/5_mission/gui/inventorynew/containers/attachmentswrapper.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class AttachmentsWrapper: AttachmentsGroupContainer
{
    Attachments m_Attachments;
    protected Widget m_AttachmentsFalseHeader;
    protected TextWidget m_AttachmentsFalseHeaderText;
    
    void AttachmentsWrapper(LayoutHolder parent) 
    {
        m_AttachmentsFalseHeader        = m_RootWidget.FindAnyWidget( &quot;attachmets_wrapper_header_spacer&quot; );
        m_AttachmentsFalseHeaderText    = TextWidget.Cast(m_AttachmentsFalseHeader.FindAnyWidget(&quot;TextWidget0&quot;));
        m_AttachmentsFalseHeader.Show( false );
