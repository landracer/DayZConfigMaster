// Source: F:/Games/Dayz/scripts/4_world/classes/playersymptoms/states/secondary/deafnesscomplete.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class DeafnessCompleteSymptom : SymptomBase
{
    override void OnInit()
    {
        m_SymptomType   = SymptomTypes.SECONDARY;
        m_Priority      = 0;
        m_ID            = SymptomIDs.SYMPTOM_DEAFNESS_COMPLETE;
        m_IsPersistent  = false;
        m_MaxCount      = 1;
        m_SyncToClient  = true;
 
        m_DestroyOnAnimFinish = true;
