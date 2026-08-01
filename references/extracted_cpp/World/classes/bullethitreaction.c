// Source: F:/Games/Dayz/scripts/4_world/classes/bullethitreaction/bullethitreaction.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class DamageDealtEffect
{
    const float ALPHA_MIN = 0;
    const float ALPHA_MAX = 1;
    float m_HitDuration;
    float m_BreakPoint;
    float m_TimeActive;
    PPERequester_HealthHitReaction m_Requester;
    
    void DamageDealtEffect()
    {
        m_HitDuration = 0.10;
        m_BreakPoint = 0.05;
        Class.CastTo(m_Requester, PPERequesterBank.GetRequester(PPERequester_HealthHitReaction));
        //Print(&quot;=====================================================&quot;);
        //Print(&quot;=====================================================&quot;);
