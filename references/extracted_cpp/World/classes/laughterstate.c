// Source: F:/Games/Dayz/scripts/4_world/classes/playersymptoms/states/primary/laughterstate.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class LaughterSymptom : SymptomBase
{
    //this is just for the Symptom parameters set-up and is called even if the Symptom doesn&#39;t execute, don&#39;t put any gameplay code in here
    override void OnInit()
    {
        m_ID            = SymptomIDs.SYMPTOM_LAUGHTER;
        m_SymptomType   = SymptomTypes.PRIMARY;
        m_Priority      = 0;
        m_SyncToClient  = false;
        m_Duration      = 4;
        
        m_DestroyOnAnimFinish = true;
