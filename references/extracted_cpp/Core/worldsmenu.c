// Source: F:/Games/Dayz/scripts/2_gamelib/entities/worldsmenu.c Source File
// Extracted from DayZ Code Explorer by Zeroy

#ifdef GAME_TEMPLATE
 
[EditorAttribute(&quot;box&quot;, &quot;GameLib/Scripted&quot;, &quot;Worlds menu&quot;, &quot;-0.25 -0.25 -0.25&quot;, &quot;0.25 0.25 0.25&quot;, &quot;255 0 0 255&quot;)]
class WorldsMenuClass
{
 
}
 
WorldsMenuClass WorldsMenuSource;
 
class WorldsMenu: GenericEntity
{
    int m_DbgListSelection = 0;
    int m_WorldsCount = 0;
    string DEFAULT_WORLD = &quot;worlds/default.ent&quot;;
    ref array<string> m_DbgOptions = {};
    ref ImageWidget m_MouseWidget;
    
    void WorldsMenu(IEntitySource src, IEntity parent)
    {
        SetFlags(EntityFlags.ACTIVE, false);
        SetEventMask(EntityEvent.POSTFRAME);
        
        Class.CastTo(m_MouseWidget, g_Game.GetWorkspace().CreateWidgets(&quot;gui/layouts/mouse.layout&quot;));
        m_MouseWidget.SetSort(1024);
        SetCursorWidget(m_MouseWidget);
        
        LoadWorlds();
    }
    
    void ~WorldsMenu()
    {
        delete m_MouseWidget;
    }
 
    array<string> GetWorldList()
    {
        return m_DbgOptions;
    }
 
    override void EOnPostFrame(IEntity other, int extra) //EntityEvent.POSTFRAME
    {
        InputManager im = g_Game.GetInputManager();
        im.ActivateContext(&quot;MenuContext&quot;);
        bool menuSelect = im.GetActionTriggered(&quot;MenuSelect&quot;);
        bool menuBack = im.GetActionTriggered(&quot;MenuBack&quot;);
        
        DbgUI.Begin(&quot;Load world&quot;, 400, 100);
 
        DbgUI.Text(&quot;Select world to load from worlds directory&quot;);
 
        if (m_DbgOptions.Count() > 0)
        {
            DbgUI.List(&quot;Worlds&quot;, m_DbgListSelection, m_DbgOptions);
            if (DbgUI.Button(&quot;Start&quot;) || menuSelect)
            {
                string worldToLoad = m_DbgOptions.Get(m_DbgListSelection);
                g_Game.SetWorldFile(worldToLoad, true);
            }
        }
        if (DbgUI.Button(&quot;Exit&quot;) || menuBack)
        {
            g_Game.RequestClose();
        }       
        DbgUI.End();
    }
 
    void LoadWorlds()
    {
        string fileName;
        FileAttr fileAttr;
        FindFileHandle worlds = FindFile(&quot;worlds/*.ent&quot;, fileName, fileAttr, 0);
        
        if (!worlds)
            return;
 
        InsertWorldToList(fileName);
 
        while(FindNextFile(worlds, fileName, fileAttr))
        {
            InsertWorldToList(fileName);
        }
 
        CloseFindFile(worlds);
 
        m_WorldsCount = m_DbgOptions.Count();
    }
 
    void InsertWorldToList(string fileName)
    {
        string path = String(&quot;worlds/&quot; + fileName);
        if (path != DEFAULT_WORLD)
            m_DbgOptions.Insert(String(path));
    }
}
 
#endif
