// Source: F:/Games/Dayz/scripts/4_world/entities/manbase/playerbase/aitargetcallbacksplayer.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class AITargetCallbacksPlayer : AITargetCallbacks
{
    private int m_iChestBoneIndex;
 
    private PlayerBase m_Player;
 
    void AITargetCallbacksPlayer(PlayerBase pPlayer)
    {
        m_Player = pPlayer;
        
        m_iChestBoneIndex = m_Player.GetBoneIndexByName(&quot;Spine3&quot;);
