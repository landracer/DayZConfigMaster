// Source: F:/Games/Dayz/scripts/4_world/classes/bleedingsources/bleedingsourcesmanagerremote.c Source File
// Extracted from DayZ Code Explorer by Zeroy

//this is instantiated on the client for both the controlled character, as well as the remote characters
class BleedingSourcesManagerRemote extends BleedingSourcesManagerBase
{
    int m_BleedingBits;
    bool m_ShowDiag;
    bool m_ShowingDiag;
    bool m_ShowingDiagDraw;
    Shape m_Point;
    bool m_EnableHitIndication = false;
    
    override protected void Init()
    {
        super.Init();
        
        if (g_Game.GetMission().GetEffectWidgets()/* && m_Player.IsControlledPlayer()*/)
        {
            Param3<bool,int,float> par = new Param3<bool,int,float>(true,0,0);
            g_Game.GetMission().GetEffectWidgets().RegisterGameplayEffectData(EffectWidgetsTypes.BLEEDING_LAYER,par);
        }
