// Source: F:/Games/Dayz/scripts/4_world/classes/playersymptoms/states/primary/sneezestate.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class SneezeSymptom extends SymptomBase
{
    //this is just for the Symptom parameters set-up and is called even if the Symptom doesn&#39;t execute, don&#39;t put any gameplay code in here
    override void OnInit()
    {
        m_SymptomType = SymptomTypes.PRIMARY;
        m_Priority = 100;
        m_ID = SymptomIDs.SYMPTOM_SNEEZE;
        m_DestroyOnAnimFinish = true;
        m_SyncToClient = false;
