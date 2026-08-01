// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/edible_base/tetracyclineantibiotics.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class TetracyclineAntibiotics : Edible_Base
{
    override void OnConsume(float amount, PlayerBase consumer)
    {
        if (consumer.GetModifiersManager().IsModifierActive(eModifiers.MDF_ANTIBIOTICS)) //effectively resets the timer
            consumer.GetModifiersManager().DeactivateModifier(eModifiers.MDF_ANTIBIOTICS);
 
        consumer.GetModifiersManager().ActivateModifier(eModifiers.MDF_ANTIBIOTICS);
