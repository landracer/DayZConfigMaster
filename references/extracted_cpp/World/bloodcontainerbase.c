// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/bloodcontainerbase/bloodcontainerbase.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class BloodContainerBase extends ItemBase
{
    private bool m_IsBloodTypeVisible = false;
    
    void BloodContainerBase()
    {
        RegisterNetSyncVariableBool(&quot;m_IsBloodTypeVisible&quot;);
