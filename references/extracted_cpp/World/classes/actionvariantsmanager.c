// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actionvariantsmanager.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionVariantManager
{
    protected ref array<ref ActionBase> m_VariantActions;
    protected typename m_type;
    protected ref ScriptInvoker e_OnUpdate;
    
    void ActionVariantManager( typename type )
    {
        m_type = type;
        m_VariantActions = new array<ref ActionBase>;
        e_OnUpdate = new ScriptInvoker();
