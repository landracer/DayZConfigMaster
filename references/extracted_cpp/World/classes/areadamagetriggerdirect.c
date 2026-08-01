// Source: F:/Games/Dayz/scripts/4_world/classes/areadamage/areadamagenew/areadamagetriggerdirect.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class AreaDamageTriggerDirect extends AreaDamageTriggerBase
{
    override void OnStayServerEvent(TriggerInsider insider, float deltaTime)
    {
        super.OnStayServerEvent(insider, deltaTime);
        
        if (m_AreaDamageManager)
            m_AreaDamageManager.OnEvaluateDamage(insider, deltaTime);
