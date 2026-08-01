// Source: F:/Games/Dayz/scripts/4_world/classes/meleetargeting.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class MeleeTargetData
{   
    Object Obj;
    vector HitPos;
    int HitComponent;
    
    void MeleeTargetData(Object o, vector p, int c)
    {
        Obj = o;
        HitPos = p;
        HitComponent = c;
