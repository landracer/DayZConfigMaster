// Source: F:/Games/Dayz/scripts/1_core/proto/enentity.c Source File
// Extracted from DayZ Code Explorer by Zeroy

typedef int[] BaseContainer;        
typedef int[] IEntitySource;
typedef int[] WidgetSource;
    
class BaseContainer 
{
    proto native owned string GetClassName();
    proto native owned string GetName();
    proto native int VarIndex(string varName);
    proto native bool IsVariableSet(int varIndex);
    proto bool IsType(int varIndex, typename type);
    proto bool Get(int varIndex, out void val);
