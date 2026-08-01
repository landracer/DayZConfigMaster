// Source: F:/Games/Dayz/scripts/3_game/remotelyactivateditembehaviour.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class RemotelyActivatedItemBehaviour
{
    protected EntityAI      m_Parent;
    protected bool          m_IsTrigger;
    protected EntityAI      m_PairDevice;
    protected int           m_PairDeviceNetIdLow;
    protected int           m_PairDeviceNetIdHigh;
    protected int           m_PersistentPairID = int.MIN;
    
    protected static ref map<int,EntityAI> m_RemoteReceivers = new map<int,EntityAI>();
 
    void RemotelyActivatedItemBehaviour(notnull EntityAI pParent)
    {
        m_Parent = pParent;
 
        m_PairDeviceNetIdLow    = -1;
        m_PairDeviceNetIdHigh   = -1;
