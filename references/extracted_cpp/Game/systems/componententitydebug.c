// Source: F:/Games/Dayz/scripts/3_game/tools/component/componententitydebug.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ComponentEntityDebug extends Component
{
    
    protected Shape             m_DebugShapeBBox;
    protected Shape             m_DebugShapeDirection;
    protected float             m_DebugShapeDirectionDist;
 
    // -------------------------------------------------------------------------
    override Shape DebugBBoxDraw()
    {
        #ifndef DEVELOPER
        return null;
        #endif
        
        if ( m_DebugShapeBBox )
            m_DebugShapeBBox.Destroy();
                
        vector min_max[2];
        
        if (!m_ThisEntityAI.GetCollisionBox(min_max))
            return null;
        
        m_DebugShapeBBox = Debug.DrawBox(min_max[0], min_max[1]);
        
        g_Game.GetCallQueue(CALL_CATEGORY_GUI).CallLater(OnDrawing, 0, true);
        
        m_ThisEntityAI.SetEventMask(EntityEvent.FRAME);
 
        return null;
