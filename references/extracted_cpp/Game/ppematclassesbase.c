// Source: F:/Games/Dayz/scripts/3_game/ppemanager/materials/ppematclassesbase.c Source File
// Extracted from DayZ Code Explorer by Zeroy


class PPEClassBase
{
    protected PPEManager                                            m_Manager;
    protected string                                                m_MaterialPath = &quot;&quot;;
    protected Material                                              m_Material;
    
    protected ref map<int, ref array<int>>                          m_ParameterUpdateQueueMap;
    protected ref array<int>                                        m_ParameterRemovalQueue;
    protected ref array<int>                                        m_UpdatedParameters;
    
    protected ref map<int,ref PPEMatClassParameterCommandData>      m_MaterialParamMapStructure; //<param_idx, <ParamData>>
    
    void PPEClassBase(string mat_path_override = &quot;&quot;)
    {
        Init(mat_path_override);
        CreateMaterial();
        CreateDataStructure();
        RegisterMaterialParameters();
