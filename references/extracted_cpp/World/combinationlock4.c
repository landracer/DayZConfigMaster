// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/combinationlock4.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class CombinationLock4 extends CombinationLock
{
    override void SetBaseLockValues()
    {
        //set lock init values
        m_LockDigits            = 4;
        m_Combination           = 1111;
        m_CombinationLocked     = 9999;
        m_IsLocked              = false;
