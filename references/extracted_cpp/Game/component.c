// Source: F:/Games/Dayz/scripts/3_game/tools/component.c Source File
// Extracted from DayZ Code Explorer by Zeroy


const string    COMP_NAME_NONE              = &quot;None&quot;;
const int       COMP_TYPE_UNDEFINED         = -1;
const int       COMP_TYPE_ETITY_DEBUG       = 0;
const int       COMP_TYPE_ENERGY_MANAGER    = 1;
const int       COMP_TYPE_BODY_STAGING      = 2;
const int       COMP_TYPE_ANIMAL_BLEEDING   = 3;
const int       COMP_TYPE_COUNT             = 4;
 
class Component
{
 
    //==========================================
    // Variables Private Static
    private static string m_CompNames[COMP_TYPE_COUNT]; 
    
    //==========================================
    // Variables Private
    protected EntityAI      m_ThisEntityAI;
    
    //==========================================
    
    void    Event_OnFrame(IEntity other, float timeSlice);
    Shape   DebugBBoxDraw();
    void    DebugBBoxSetColor(int color);
    void    DebugBBoxDelete();
    Shape   DebugDirectionDraw(float distance = 1);
    void    DebugDirectionSetColor(int color);
    void    DebugDirectionDelete();
    
    // Methods Public Static
    static void Init()
    {
        m_CompNames[COMP_TYPE_ETITY_DEBUG]          = &quot;ComponentEntityDebug&quot;;
        m_CompNames[COMP_TYPE_ENERGY_MANAGER]       = &quot;ComponentEnergyManager&quot;;
        m_CompNames[COMP_TYPE_BODY_STAGING]         = &quot;ComponentBodyStaging&quot;;
        m_CompNames[COMP_TYPE_ANIMAL_BLEEDING]      = &quot;ComponentAnimalBleeding&quot;;
