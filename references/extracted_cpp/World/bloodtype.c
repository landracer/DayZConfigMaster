// Source: F:/Games/Dayz/scripts/4_world/static/bloodtype.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class BloodTypes
{
    static const int NUM_OF_BLOOD_TYPES = 8;
    
    
    
    static int GenerateBloodType()
    {
        int types[NUM_OF_BLOOD_TYPES]       = {LIQUID_BLOOD_0_P, LIQUID_BLOOD_0_N, LIQUID_BLOOD_A_P, LIQUID_BLOOD_A_N, LIQUID_BLOOD_B_P, LIQUID_BLOOD_B_N, LIQUID_BLOOD_AB_P, LIQUID_BLOOD_AB_N};
        int probability[NUM_OF_BLOOD_TYPES]     = {42,10,25,5,9,3,2,2};
        
        int probability_sum = 0;
        
        for(int i = 0; i < NUM_OF_BLOOD_TYPES; i++)
        {
            probability_sum += probability[i];
        }
        
        //TIMERDEPRECATED - randomized blodtype for new character
        int probability_randomized = Math.RandomInt(0,probability_sum);
        
        int tmp = 0;
        int index;
        
        for(i = 0; i < NUM_OF_BLOOD_TYPES; i++)
        {
            tmp += probability[i];
            
            if( tmp > probability_randomized )
            {
                index = i;
                break;
            }
        }
        
        return types[index];
