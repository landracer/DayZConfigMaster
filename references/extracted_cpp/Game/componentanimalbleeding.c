// Source: F:/Games/Dayz/scripts/3_game/tools/component/componentanimalbleeding.c Source File
// Extracted from DayZ Code Explorer by Zeroy

//-----------------------------
// ANIMAL BLEEDING
//-----------------------------
/*
Animal bleeding is handled by this component.
*/
 
class ComponentAnimalBleeding : Component
{
    // Member variables
    protected ref Timer m_BleedTimer;
    protected const float BASE_BLEED_RATE = 250;
    protected const float PASS_OUT_AMOUT = 500;
 
    // Constructor
    void ComponentAnimalBleeding()
    {
        
