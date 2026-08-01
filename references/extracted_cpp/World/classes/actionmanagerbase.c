// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actionmanagerbase.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class TSelectableActionInfoArray extends array<ref TSelectableActionInfo>
{
    bool IsSameAs(TSelectableActionInfoArray other)
    {
        if (this.Count() != other.Count())
        {
            return false;
        }
        
        for (int i = 0; i < Count(); ++i)
        {
            TSelectableActionInfo ai1 = this.Get(i);
            TSelectableActionInfo ai2 = other.Get(i);
            
            if (ai1.param2 != ai2.param2)
            {
                return false;
            }
            
            if (ai1.param3 != ai2.param3)
            {
                return false;
            }
        }
 
        return true;
