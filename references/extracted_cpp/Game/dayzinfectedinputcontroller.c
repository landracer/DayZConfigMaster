// Source: F:/Games/Dayz/scripts/3_game/entities/dayzinfectedinputcontroller.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class DayZInfectedInputController : DayZCreatureAIInputController
{
    proto native bool IsVault();

    proto native float GetVaultHeight();
    
    proto native int GetMindState();
    proto native EntityAI GetTargetEntity();
    
    //--------------------------------------------------------------
    private void DayZInfectedInputController()
    {
