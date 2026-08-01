// Source: F:/Games/Dayz/scripts/5_mission/gui/inventorynew/inherited/containerwithelectricmanager.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ContainerWithElectricManager : ContainerWithCargoAndAttachments
{
    override void UpdateInterval()
    {
        super.UpdateInterval();
        
        if (m_Entity)
            ElectricityIcon();
