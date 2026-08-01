// Source: F:/Games/Dayz/scripts/3_game/ppemanager/requesters/pperundergroundacco.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class PPERUndergroundAcco extends PPERequester_GameplayBase
{
    void SetEyeAccommodation(float value)
    {
        SetTargetValueFloat(PPEExceptions.EYEACCOM,PPEEyeAccomodationNative.PARAM_INTENSITY,false,value,PPEEyeAccomodationNative.L_0_UNDERGROUND,PPOperators.SET);
