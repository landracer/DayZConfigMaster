// Source: F:/Games/Dayz/scripts/3_game/ppemanager/materials/matparameters/ppematclassparametercommanddata.c Source File
// Extracted from DayZ Code Explorer by Zeroy

typedef map<int,ref PPERequestParamDataBase> ActiveParameterRequestsMap; //<request_ID, data>
 
class PPEMatClassParameterCommandData
{
    //default layer constants, complex data like colors PPEMatClassParameterColor are handled separately
    const int LAYER_INFO_VALUE = 0;
    const int LAYER_INFO_OPERATOR = 1;
    
    ref array<int>                                  m_CommandLayersArray; //for tracking active priorities and sorting them //TODO - could have been &#39;set&#39;..
    protected int                                   m_UpdatedCount;
    protected int                                   m_MaterialIndex; //just a helper
    protected int                                   m_ParameterIndex;
    protected ref ActiveParameterRequestsMap        m_RequestMap;//<request_ID, parameter data>
    protected PPEClassBase                          m_Parent;
    protected ref Param                             m_Defaults; // Careful, formating is such, that param1 is ALWAYS string, containing parameter name. Actual values follow.
    protected ref Param                             m_CurrentValues; // Careful, only actual values, WITHOUT string
    
    protected ref map<int,ref array<int>>           m_Dependencies;
    
    void PPEMatClassParameterCommandData(int mat_idx, int parameter_idx, PPEClassBase parent)
    {
        m_MaterialIndex = mat_idx;
        m_ParameterIndex = parameter_idx;
        m_Parent = parent;
        
        m_CommandLayersArray = new array<int>;
        m_UpdatedCount = 0;
        m_RequestMap = new ActiveParameterRequestsMap;
