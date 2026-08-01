// Source: F:/Games/Dayz/scripts/3_game/ppemanager/requesters/pperequestplatformsbase.c Source File
// Extracted from DayZ Code Explorer by Zeroy

 
class PPERequesterBase
{
    protected bool m_IsRunning; //helps determine if the requester has been stopped or not
    protected bool m_ValuesSent;
    protected int m_IDX;
    
    protected bool m_Valid = false;
    
    protected ref map<int,ref map<int,ref PPERequestParamDataBase>> m_RequestDataStructure; // <material<param_id,data>> // parameter data objects are singletons
    
    void PPERequesterBase(int requester_IDX)
    {
        m_Valid = PPERequesterBank.VerifyRequester(this);
        
        m_IDX = requester_IDX;
        m_ValuesSent = true;
        m_IsRunning = false;
        m_RequestDataStructure = new map<int,ref map<int,ref PPERequestParamDataBase>>;
