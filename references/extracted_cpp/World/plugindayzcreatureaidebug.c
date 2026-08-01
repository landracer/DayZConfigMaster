// Source: F:/Games/Dayz/scripts/4_world/plugins/pluginbase/plugindayzcreatureaidebug.c Source File
// Extracted from DayZ Code Explorer by Zeroy

typedef Param4<float, string, int, string>  DayZCreatureAnimScriptDebugAnimEventData;
typedef Param1<string>                      DayZCreatureAnimScriptDebugAnimPredictionData;
typedef Param1<string>                      DayZCreatureAnimScriptDebugAnimTagData;
 
class DayZCreatureAnimScriptDebug
{       
    proto native void   SetCreature(DayZCreature creature);
 
    proto native int    GetVariableCount();
    proto native owned string GetVariableName(int index);
    proto native int    GetVariableType(int index);
    proto native int    GetVariableInt(int index);
    proto native float  GetVariableFloat(int index);
    proto native bool   GetVariableBool(int index);
    
    proto native int    SetVariableInt(int index, int value);
    proto native float  SetVariableFloat(int index, float value);
    proto native bool   SetVariableBool(int index, bool value);
        
    proto native int    GetCommandCount();
    proto native owned string GetCommandName(int index);
    proto native int    GetCommandID(int index);
 
    proto native void   ActivateCommand(int index, int userInt, float userFloat);
    
    const int                                                       m_iMaxAnimEventsCount = 50;
    ref array<string>                                               m_EventsFilter = new array<string>;
    
    ref array<ref DayZCreatureAnimScriptDebugAnimEventData>         m_AnimEvents = new array<ref DayZCreatureAnimScriptDebugAnimEventData>;
    ref array<ref DayZCreatureAnimScriptDebugAnimPredictionData>    m_AnimPredictions = new array<ref DayZCreatureAnimScriptDebugAnimPredictionData>;
    ref array<ref DayZCreatureAnimScriptDebugAnimTagData>           m_AnimTags = new array<ref DayZCreatureAnimScriptDebugAnimTagData>;
    
    void OnAnimationEventsStart()
    {
        m_AnimPredictions.Clear();
        m_AnimTags.Clear();
