// Source: F:/Games/Dayz/scripts/4_world/entities/woodbase/trees.c Source File
// Extracted from DayZ Code Explorer by Zeroy

//-------------------------------------
//Birch tree baseclasses (white bark)
class TreeHard_BetulaPendula: TreeHard {};
class TreeSoft_BetulaPendula_Base: TreeSoft {};
 
 
//-----------------------------------------------
//TreeSoft
class TreeSoft_t_BetulaPendula_1f:                      TreeSoft_BetulaPendula_Base {};
class TreeSoft_t_BetulaPendula_1fb:                     TreeSoft_BetulaPendula_Base
{
    override bool HasPlayerCollisionSound() { return true; }
