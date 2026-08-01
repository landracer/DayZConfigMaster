// Source: F:/Games/Dayz/scripts/4_world/classes/areadamage/areadamagenew/areadamageevents.c Source File
// Extracted from DayZ Code Explorer by Zeroy


class AreaDamageEvents
{
    void OnTriggerCreated() {}
    void OnTriggerDestroyed() {}    
    
    void OnEnterBeginEvent(TriggerInsider insider) {}
    void OnEnterServerEvent(TriggerInsider insider) {}
    void OnEnterClientEvent(TriggerInsider insider) {}
    void OnEnterEndEvent(TriggerInsider insider) {}
    
    void OnStayStartBeginEvent(int nrOfInsiders) {}
    void OnStayStartServerEvent(int nrOfInsiders) {}
    void OnStayStartClientEvent(int nrOfInsiders) {}
    void OnStayStartEndEvent(int nrOfInsiders) {}
    
    void OnStayBeginEvent(TriggerInsider insider, float deltaTime) {}
    void OnStayServerEvent(TriggerInsider insider, float deltaTime) {}
    void OnStayClientEvent(TriggerInsider insider, float deltaTime) {}
    void OnStayEndEvent(TriggerInsider insider, float deltaTime) {}
    
    void OnStayFinishBeginEvent() {}
    void OnStayFinishServerEvent() {}
    void OnStayFinishClientEvent() {}
    void OnStayFinishEndEvent() {}
    
    void OnLeaveBeginEvent(TriggerInsider insider) {}
    void OnLeaveServerEvent(TriggerInsider insider) {}
    void OnLeaveClientEvent(TriggerInsider insider) {}
    void OnLeaveEndEvent(TriggerInsider insider) {}
    
    protected bool OnEvaluateDamage(TriggerInsider insider, float damageCoef) { return false; }
    
    protected void PreDamageActions() {}
    protected void PostDamageActions() {}
