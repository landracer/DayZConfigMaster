// Source: F:/Games/Dayz/scripts/4_world/classes/kurushake.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class KuruShake
{
    const float RANDOM_RANGE_POINT = 1;
    const float RANDOM_RANGE_DEVIATION = 0.50;
    const float SHAKE_DURATION_PHASE1 = 0.01;
    const float SHAKE_STRENGTH_MIN = 1;
    const float SHAKE_STRENGTH_MAX = 5;
    
    PlayerBase m_Player;
    float m_Time;
    float m_Time2;
    float m_RelativeTime;
    float m_RelativeTime2;
    float m_ShakeStrength;
    
    
    ref array<vector> m_Curve = new array<vector>;
    
    void KuruShake(PlayerBase player, float amount)
    {
        
        vector pos1;
        int seed = Math.Randomize(player.GetRandomGeneratorSyncManager().GetRandomInRange(RandomGeneratorSyncUsage.RGSGeneric,0,10000));
        Math.RandomFloat(-RANDOM_RANGE_POINT,RANDOM_RANGE_POINT);//throw away
        m_ShakeStrength = Math.Lerp(SHAKE_STRENGTH_MIN, SHAKE_STRENGTH_MAX, amount);
        pos1[0] = Math.RandomFloat(-RANDOM_RANGE_POINT,RANDOM_RANGE_POINT);
        pos1[1] = Math.RandomFloat(-RANDOM_RANGE_POINT,RANDOM_RANGE_POINT);
        pos1.Normalize();
        pos1 = pos1 * m_ShakeStrength;
        
        vector pos2;
        pos2[0] = pos1[0] + Math.RandomFloat(-RANDOM_RANGE_DEVIATION,RANDOM_RANGE_DEVIATION) + pos1[0];
        pos2[1] = pos1[1] + Math.RandomFloat(-RANDOM_RANGE_DEVIATION,RANDOM_RANGE_DEVIATION) + pos1[1];
        m_Curve.Insert(&quot;0 0 0&quot;);
        m_Curve.Insert(pos1);
        m_Curve.Insert(pos2);
        
        /*
        Print(&quot;==========================&quot;);
        for(int i = 0; i < m_Curve.Count(); i++)
        {
            Print(m_Curve[i]);
        }
        Print(&quot;==========================&quot;);
        */
