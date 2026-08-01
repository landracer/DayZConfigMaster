// Source: F:/Games/Dayz/scripts/3_game/anim/animcommand.c Source File
// Extracted from DayZ Code Explorer by Zeroy


class AnimCommandBase
{
    // These would be private if not for the scripted commands
    // As other commands lifetime are handled internally
    protected void AnimCommandBase() {}
    protected void ~AnimCommandBase() {}
    
    proto native IEntity GetEntity();

    void OnActivate() {}

    void OnDeactivate() {}

    void PreAnimUpdate(float pDt) {}

    void PrePhysUpdate(float pDt) {}

    proto native void PreAnim_CallCommand(int pCommand, int pParamInt, float pParamFloat);
    proto native void PreAnim_SetFloat(int pVar, float pFlt);
    proto native void PreAnim_SetInt(int pVar, int pInt);
    proto native void PreAnim_SetBool(int pVar, bool pBool);

    proto native bool PrePhys_IsEvent(int pEvent);
    proto native bool PrePhys_IsTag(int pTag);
