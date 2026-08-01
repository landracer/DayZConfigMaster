// Source: F:/Games/Dayz/scripts/1_core/proto/enscript.c Source File
// Extracted from DayZ Code Explorer by Zeroy



class Class
{
    proto native external bool IsInherited(typename type);
    
    proto native owned external string ClassName();
    
    string GetDebugName() { return ClassName(); }
    
    proto native external typename Type();
      
    proto external static typename StaticType();
    
    static typename StaticGetType(typename t)
    {
        return t;
