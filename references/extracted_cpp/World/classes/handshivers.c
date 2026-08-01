// Source: F:/Games/Dayz/scripts/4_world/classes/playersymptoms/states/secondary/handshivers.c Source File
// Extracted from DayZ Code Explorer by Zeroy

 
class HandShiversSymptom extends SymptomBase
{
    int m_ShakeLevel;
    //this is just for the Symptom parameters set-up and is called even if the Symptom doesn&#39;t execute, don&#39;t put any gameplay code in here
    override void OnInit()
    {
        m_SymptomType = SymptomTypes.SECONDARY;
        m_Priority = 0;
        m_ID = SymptomIDs.SYMPTOM_HAND_SHIVER;
        m_SyncToClient = false;
 
