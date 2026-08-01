// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/interact/vehicles/actioncardoorsoutside.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionCarDoorsOutside: ActionInteractBase
{   
    protected int m_CommandUIDPerCrewIdx[4];
    protected bool m_IsOpening = true;
    
    void ActionCarDoorsOutside()
    {
        m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_OPENDOORFW;
        m_StanceMask = DayZPlayerConstants.STANCEMASK_ALL;
        m_LockTargetOnUse = false;
