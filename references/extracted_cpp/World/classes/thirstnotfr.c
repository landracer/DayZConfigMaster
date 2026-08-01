// Source: F:/Games/Dayz/scripts/4_world/classes/playernotifiers/notifiers/thirstnotfr.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ThirstNotfr: NotifierBase
{
    private const float     HYDRATED_TRESHOLD           = 3500;
    private const float     THIRSTY_TRESHOLD            = 2500;
    private const float     VERY_THIRSTY_TRESHOLD       = 1500;
    private const float     FATALLY_THIRSTY_TRESHOLD    = 0;
    private const float     DEC_TRESHOLD_LOW            = 0;
    private const float     DEC_TRESHOLD_MED            = -0.2;
    private const float     DEC_TRESHOLD_HIGH           = -0.85;
    private const float     INC_TRESHOLD_LOW            = 0;
    private const float     INC_TRESHOLD_MED            = 0.2;
    private const float     INC_TRESHOLD_HIGH           = 0.85;
 
    override int GetNotifierType()
    {
        return eNotifiers.NTF_THIRSTY;
