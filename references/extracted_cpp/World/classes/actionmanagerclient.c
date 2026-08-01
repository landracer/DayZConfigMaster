// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actionmanagerclient.c Source File
// Extracted from DayZ Code Explorer by Zeroy

typedef map<typename,ref array<ActionBase_Basic>>       TInputActionMap;
typedef map<typename,ref ActionInput>                   TTypeNameActionInputMap;
 
class ActionManagerClient: ActionManagerBase 
{
    
    //Last send AcknowledgmentID (client can send more requests before recive ackfor first action)
    protected int                               m_LastAcknowledgmentID;
    protected bool                              m_ActionPossible;
    protected ref array<ref InventoryLocation>  m_ReservedInventoryLocations; // obsolete
    protected ref InventoryActionHandler        m_InventoryActionHandler;
    protected ref InventoryLocation             m_HandInventoryLocationTest;
    protected ref TTypeNameActionInputMap       m_RegistredInputsMap;
    protected ref array<ActionInput>            m_OrederedAllActionInput;
    protected ref array<ActionInput>            m_OrderedStandartActionInputs;
    protected ref array<ActionInput>            m_DefaultOrderOfActionInputs;
    protected int                               m_SelectedActionInputToSrollIndex;
    protected bool                              m_IgnoreAutoInputEnd;
    
    protected ref ActionData                    m_PendingActionData;
 
    protected bool                              m_ActionWantEndRequest_Send;        //Request to server was sended
    protected bool                              m_ActionInputWantEnd_Send;
 
    void ActionManagerClient(PlayerBase player) 
    {
        m_HandInventoryLocationTest = new InventoryLocation;
        m_HandInventoryLocationTest.SetHands(player,null);
        m_LastAcknowledgmentID = 1;
        m_Targets   = new ActionTargets(player);
        //m_ReservedInventoryLocations = new array<ref InventoryLocation>;
        m_InventoryActionHandler = new InventoryActionHandler(player);
 
        m_ActionWantEndRequest_Send = false;
        m_ActionInputWantEnd_Send = false;
        RegisterInputs(player);
        m_SelectedActionInputToSrollIndex = 0;
