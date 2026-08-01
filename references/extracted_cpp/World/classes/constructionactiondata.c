// Source: F:/Games/Dayz/scripts/4_world/classes/basebuilding/constructionactiondata.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ConstructionActionData
{
    Object m_Target;
    
    //base building
    ref array<ConstructionPart>     m_BuildParts;
    ref array<ConstructionPart>     m_BuildPartsNoTool;
    
    int                             m_PartIndex; //used on client only, action synchronizes it to server to avoid mismatch
    string                          m_MainPartName;
    string                          m_MainPartNameNoTool;
    ref ConstructionPart            m_TargetPart;
    
    //combination lock
    CombinationLock                 m_CombinationLock;
    
    //attaching
    int                             m_SlotId;
    PlayerBase                      m_ActionInitiator;
    //detaching
    ref array<EntityAI>             m_Attachments;
    
    protected ActionVariantManager  m_ActionVariantManager;
    protected ActionVariantManager  m_ActionNoToolVariantManager;
 
    int                             m_AttachmentsIndex;
 
    void ConstructionActionData()
    {
        m_BuildParts = new array<ConstructionPart>;
        m_BuildPartsNoTool = new array<ConstructionPart>;
        m_PartIndex = 0;
        
        m_Attachments = new array<EntityAI>;
        m_AttachmentsIndex = 0;
 
        if ( g_Game.IsClient() || !g_Game.IsMultiplayer() )
        {
            m_ActionVariantManager = ActionManagerClient.GetVariantManager( ActionBuildPart );
            m_ActionVariantManager.GetOnUpdateInvoker().Clear();
            m_ActionVariantManager.GetOnUpdateInvoker().Insert(OnUpdateActions);
                
            m_ActionNoToolVariantManager = ActionManagerClient.GetVariantManager( ActionBuildShelter );
            m_ActionNoToolVariantManager.GetOnUpdateInvoker().Clear();
            m_ActionNoToolVariantManager.GetOnUpdateInvoker().Insert(OnUpdateActionsNoTool);
        }
