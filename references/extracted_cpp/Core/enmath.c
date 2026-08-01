// Source: F:/Games/Dayz/scripts/1_core/proto/enmath.c Source File
// Extracted from DayZ Code Explorer by Zeroy


 
class Math
{
    private void Math() {}
    private void ~Math() {}
    
    static const float EULER = 2.7182818284590452353;
    static const float PI = 3.14159265358979;
    static const float PI2 = 6.28318530717958;
    static const float PI_HALF = 1.570796326794;
 
    static const float RAD2DEG = 57.2957795130823208768;
    static const float DEG2RAD = 0.01745329251994329577;

    proto static int GetNumberOfSetBits(int i);
    
    proto static int GetNthBitSet(int value, int n);
    
    proto static int RandomInt(int min, int max);
    
    
    static int RandomIntInclusive(int min, int max)
    {
        return Math.RandomInt(min, max+1);
