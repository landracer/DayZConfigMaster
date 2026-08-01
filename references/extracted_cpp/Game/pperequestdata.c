// Source: F:/Games/Dayz/scripts/3_game/ppemanager/pperequestdata.c Source File
// Extracted from DayZ Code Explorer by Zeroy


class PPERequestParamDataBase
{
    PPERequesterBase m_Requester;
    protected bool m_UpdatingDataValues; //new values are being sent periodically
    protected bool m_IsActive; //hasn&#39;t been stopped yet (does not have to update values)
    protected bool m_SettingDefaultValues;
    
    protected int m_MaterialID;
    protected int m_ParameterID;
    protected int m_VariableType; //PPEConstants.VAR_TYPE_INT etc.
    protected int m_RequesterIDX;
    protected int m_InteractionMask;
    protected int m_Priority; //de-facto blending layer
    
    void PPERequestParamDataBase(int requester_idx, int mat_id, int param_id, int data_type = 0, int priority = 0, int mask = PPOperators.SET)
    {
        m_RequesterIDX = requester_idx;
        m_MaterialID = mat_id;
        m_ParameterID = param_id;
        m_VariableType = data_type;
        m_Priority = priority;
        m_InteractionMask = mask;
        m_UpdatingDataValues = false;
        m_SettingDefaultValues = false;
        m_IsActive = false;
        InitValues();
