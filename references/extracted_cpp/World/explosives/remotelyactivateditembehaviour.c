// Source: F:/Games/Dayz/scripts/4_world/entities/explosivesbase/remotelyactivateditembehaviour.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class RemotelyActivatedItemBehaviour
{
    protected EntityAI      m_Parent;
    protected EntityAI      m_PairDevice;
    protected int           m_PairDeviceNetIdLow;
    protected int           m_PairDeviceNetIdHigh;
 
    void RemotelyActivatedItemBehaviour(notnull EntityAI pParent)
    {
        m_Parent = pParent;
        m_PairDeviceNetIdLow = -1;
        m_PairDeviceNetIdHigh = -1;
