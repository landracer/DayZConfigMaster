// Source: F:/Games/Dayz/scripts/4_world/classes/playernotifiers/notifiers/hungernotfr.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class HungerNotfr: NotifierBase
{
    protected const float   DEC_TRESHOLD_LOW            = 0;
    protected const float   DEC_TRESHOLD_MED            = -0.35;
    protected const float   DEC_TRESHOLD_HIGH           = -0.57;
    protected const float   INC_TRESHOLD_LOW            = 0;
    protected const float   INC_TRESHOLD_MED            = 0.35;
    protected const float   INC_TRESHOLD_HIGH           = 0.57;
 
    override int GetNotifierType()
    {
        return eNotifiers.NTF_HUNGRY;
