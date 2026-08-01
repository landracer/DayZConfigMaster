// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/interact/vehicles/actioncardoors.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionCarDoors: ActionInteractBase
{   
    protected int m_CommandUIDPerCrewIdx[4];
    protected bool m_IsOpening = true;
    // --- Backwards compatibility 1.09 -> 1.10 (But please do not use these)
    CarScript m_Car = null;
    string m_AnimSource = &quot;&quot;;
    // ---
    
    void ActionCarDoors()
    {
        m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_OPENDOORFW;
        m_StanceMask = DayZPlayerConstants.STANCEMASK_ALL;
        m_LockTargetOnUse = false;
