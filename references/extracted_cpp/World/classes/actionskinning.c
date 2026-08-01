// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/continuous/actionskinning.c Source File
// Extracted from DayZ Code Explorer by Zeroy

/*
Example of skinning config which should be inside animal&#39;s base class:
class Skinning
{
    // All classes in this scope are parsed, so they can have any name.
    class ObtainedSteaks 
    {
        item = &quot;DeerSteakMeat&quot;; // item to spawn
        count = 10; // How many items to spawn
        transferToolDamageCoef = 1; // Optional: How much tool&#39;s damage is transfered to item&#39;s damage.
        
        // Make sure to have only 1 of the following quantity paramenter
        quantity = 100; // Optional: Set item&#39;s quantity
        quantityCoef = 1; // Optional: Set item&#39;s quantity (in coefficient)
        quantityMinMax[] = {100, 200}; // Optional: Set item&#39;s quantity within min/max values
        quantityMinMaxCoef[] = {0, 1}; // Optional: Set item&#39;s quantity (in coefficient) within min/max values
    };
};
*/
 
class ActionSkinningCB : ActionContinuousBaseCB
{
    override void CreateActionComponent()
    {
        m_ActionData.m_ActionComponent = new CAContinuousTime(UATimeSpent.SKIN);
