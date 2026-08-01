// Source: F:/Games/Dayz/scripts/4_world/classes/virtualhud/displayelementtendency.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class DisplayElementTendency extends DisplayElementBase
{
    protected int TENDENCY_MASK = 7;// first x bits
    protected int SERIOUSNESS_BIT_MASK = 7;// second x bits
    protected int SERIOUSNESS_BIT_OFFSET = 3;//bit offset - points to where seriousness bit starts(TODO: get as log from mask)
    
    void DisplayElementTendency(PlayerBase player)
    {
        NUM_OF_BITS = 6;//the overall num of bits this element occupies(can be calculated from masks, better leave it explicit)
