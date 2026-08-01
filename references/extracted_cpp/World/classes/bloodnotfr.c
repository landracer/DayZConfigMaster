// Source: F:/Games/Dayz/scripts/4_world/classes/playernotifiers/notifiers/bloodnotfr.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class BloodNotfr: NotifierBase
{
    protected const float   DEC_TRESHOLD_LOW    = 0;
    protected const float   DEC_TRESHOLD_MED    = -12;
    protected const float   DEC_TRESHOLD_HIGH   = -22;
    protected const float   INC_TRESHOLD_LOW    = 0;
    protected const float   INC_TRESHOLD_MED    = 3;
    protected const float   INC_TRESHOLD_HIGH   = 6;
 
    override int GetNotifierType()
    {
        return eNotifiers.NTF_BLOOD;
