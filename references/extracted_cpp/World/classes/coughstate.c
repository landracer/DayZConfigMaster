// Source: F:/Games/Dayz/scripts/4_world/classes/playersymptoms/states/primary/coughstate.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class CoughSymptom extends SymptomBase
{
    const int COUGH_BLOOD_LOSS = 50;
    //this is just for the Symptom parameters set-up and is called even if the Symptom doesn&#39;t execute, don&#39;t put any gameplay code in here
    override void OnInit()
    {
        m_SymptomType = SymptomTypes.PRIMARY;
        m_Priority = 100;
        m_ID = SymptomIDs.SYMPTOM_COUGH;
        m_DestroyOnAnimFinish = true;
        m_SyncToClient = false;
