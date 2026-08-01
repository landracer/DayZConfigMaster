// Source: F:/Games/Dayz/scripts/4_world/classes/playersymptoms/states/secondary/bloodloss.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class BloodLoss extends SymptomBase
{
    Material m_MatGauss;
    const int BLUR_DURATION = 3000;
    float m_BloodSet;
    PPERequester_BloodLoss m_RequesterBloodLoss;
    
    //this is just for the Symptom parameters set-up and is called even if the Symptom doesn&#39;t execute, don&#39;t put any gameplay code in here
    override void OnInit()
    {
        m_SymptomType = SymptomTypes.SECONDARY;
        m_Priority = 0;
        m_ID = SymptomIDs.SYMPTOM_BLOODLOSS;
        m_DestroyOnAnimFinish = true;
        m_IsPersistent = false;
        m_SyncToClient = true;
        m_BloodSet = -1;
        
        if ( !g_Game.IsDedicatedServer() )
        {
            Class.CastTo(m_RequesterBloodLoss,PPERequester_BloodLoss.Cast(PPERequesterBank.GetRequester(PPERequester_BloodLoss)));
        }
        
