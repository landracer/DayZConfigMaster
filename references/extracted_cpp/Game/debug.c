// Source: F:/Games/Dayz/scripts/3_game/tools/debug.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class Debug
{   
    private static const string LOG_DEBUG                   = &quot;Debug&quot;;
    private static const string LOG_DEBUG_ACTION            = &quot;Action&quot;;
    private static const string LOG_DEBUG_SYMPTOM           = &quot;Symptom&quot;;
    private static const string LOG_DEBUG_INV_MOVE          = &quot;Inv Move&quot;;
    private static const string LOG_DEBUG_INV_RESERVATION   = &quot;Inv Rrsv&quot;;
    private static const string LOG_DEBUG_INV_HFSM          = &quot;HFSM&quot;;
    private static const string LOG_DEBUG_QUICKBAR          = &quot;Quickbar&quot;;
    private static const string LOG_DEBUG_BASEBUILDING      = &quot;Base Building&quot;;
    private static const string LOG_DEBUG_BLEEDING_CHANCES  = &quot;Bleeding&quot;;
    private static const string LOG_DEBUG_TRIGGER           = &quot;Trigger&quot;;
    private static const string LOG_DEBUG_PARTICLE          = &quot;Particle&quot;;
    private static const string LOG_DEBUG_TF                = &quot;TestFramework&quot;;
    private static const string LOG_DEBUG_WEIGHT            = &quot;Weight&quot;;
    private static const string LOG_DEBUG_MELEE             = &quot;Melee&quot;;
    private static const string LOG_DEBUG_WEATHER           = &quot;Weather&quot;;
 
    private static const string LOG_INFO                    = &quot;Info&quot;;
    private static const string LOG_WARNING                 = &quot;Warning&quot;;
    private static const string LOG_ERROR                   = &quot;Error&quot;;
    private static const string LOG_DEFAULT                 = &quot;n/a&quot;;
    
    private static ref array<Shape> m_DebugShapes;
    private static ref array<ref DebugTextScreenSpace> m_DebugTextsSS;
    private static ref array<ref DebugTextWorldSpace> m_DebugTextsWS;
    
    static Widget m_DebugLayoutCanvas;
    static CanvasWidget m_CanvasDebug;
    
    static string GetDebugName(Managed entity)
    {
        if (!entity)
            return &quot;&quot;;
        
        Object obj;
        if (CastTo(obj, entity))
            return obj.GetDebugNameNative();
        
        return entity.GetDebugName();
