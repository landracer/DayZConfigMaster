// Source: F:/Games/Dayz/scripts/3_game/entities/entity.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class Entity extends ObjectTyped
{
    proto native void DisableSimulation(bool disable);

    proto native bool GetIsSimulationDisabled();
    
    proto native int GetSimulationTimeStamp();

    proto native float GetAnimationPhase(string animation);
    
    proto native void SetAnimationPhase(string animation, float phase);
    
    proto int GetNumUserAnimationSourceNames();
 
    proto string GetUserAnimationSourceName(int index);

    void SetAnimationPhaseNow(string animation, float phase)
    {
        ResetAnimationPhase(animation, phase);
        SetAnimationPhase(animation, phase);
