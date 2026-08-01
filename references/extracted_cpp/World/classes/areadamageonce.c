// Source: F:/Games/Dayz/scripts/4_world/classes/areadamage/areadamagenew/areadamageonce.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class AreaDamageOnce : AreaDamageManager
{   
    override void OnEnterServerEvent(TriggerInsider insider)
    {
        super.OnEnterServerEvent(insider);
        
        OnEvaluateDamageEx(insider, 1);
