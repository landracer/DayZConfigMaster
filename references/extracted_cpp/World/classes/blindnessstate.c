// Source: F:/Games/Dayz/scripts/4_world/classes/playersymptoms/states/secondary/blindnessstate.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class BlindnessSymptom extends SymptomBase
{
    Material m_MatGauss;
    const int BLUR_DURATION = 3000;
    ref Param1<vector> m_Position = new Param1<vector>(&quot;0 0 0&quot;);
    
    //this is just for the Symptom parameters set-up and is called even if the Symptom doesn&#39;t execute, don&#39;t put any gameplay code in here
    override void OnInit()
    {
        m_SymptomType = SymptomTypes.SECONDARY;
        m_Priority = 0;
        m_ID = SymptomIDs.SYMPTOM_BLINDNESS;
        m_DestroyOnAnimFinish = true;
        m_IsPersistent = false;
        MakeParamObjectPersistent(m_Position);
