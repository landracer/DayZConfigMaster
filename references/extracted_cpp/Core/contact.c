// Source: F:/Games/Dayz/scripts/1_core/physics/contact.c Source File
// Extracted from DayZ Code Explorer by Zeroy



sealed class Contact
{
    private void Contact() {}
    private void ~Contact() {}
 
    Physics Physics1;
    Physics Physics2;
    SurfaceProperties Material1; 
    SurfaceProperties Material2;
    float Impulse;
    int ShapeIndex1;
    int ShapeIndex2;
    vector  Normal;
    vector  Position;
    float PenetrationDepth;
 
    float       RelativeNormalVelocityBefore;
    float       RelativeNormalVelocityAfter;
    vector      RelativeVelocityBefore;
    vector      RelativeVelocityAfter;
    
    vector  VelocityBefore1;
    vector  VelocityBefore2;
    vector  VelocityAfter1;
    vector  VelocityAfter2;
 
    proto native vector GetNormalImpulse();
    proto native float GetRelativeVelocityBefore(vector vel);
    proto native float GetRelativeVelocityAfter(vector vel);
