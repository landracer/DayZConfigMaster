// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/paper.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class Paper extends ItemBase
{
    protected ref WrittenNoteData m_NoteContents;
    
    void Paper()
    {
        m_NoteContents = new WrittenNoteData(this);
