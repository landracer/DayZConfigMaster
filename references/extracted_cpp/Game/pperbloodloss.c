// Source: F:/Games/Dayz/scripts/3_game/ppemanager/requesters/pperbloodloss.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class PPERequester_BloodLoss extends PPERequester_GameplayBase
{
    void SetBloodLossLevel(float level)
    {
        SetTargetValueFloat(PostProcessEffectType.Glow,PPEGlow.PARAM_SATURATION,true,level,PPEGlow.L_22_BLOODLOSS,PPOperators.SET);
