// Source: F:/Games/Dayz/scripts/4_world/classes/playersymptoms/states/primary/painheavystate.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class PainHeavySymptom extends SymptomBase
{
    //this is just for the Symptom parameters set-up and is called even if the Symptom doesn&#39;t execute, don&#39;t put any gameplay code in here
    override void OnInit()
    {
        m_SymptomType = SymptomTypes.PRIMARY;
        m_Priority = 1;
        m_ID = SymptomIDs.SYMPTOM_PAIN_HEAVY;
        m_SyncToClient = true;
        m_MaxCount = 2;
        m_Duration = 1;
