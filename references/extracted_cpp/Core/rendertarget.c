// Source: F:/Games/Dayz/scripts/2_gamelib/entities/rendertarget.c Source File
// Extracted from DayZ Code Explorer by Zeroy

#ifdef GAME_TEMPLATE
 
[EditorAttribute(&quot;box&quot;, &quot;GameLib/Scripted&quot;, &quot;Render target&quot;, &quot;-0.25 -0.25 -0.25&quot;, &quot;0.25 0.25 0.25&quot;, &quot;255 0 0 255&quot;)]
class RenderTargetClass
{
 
}
 
RenderTargetClass RenderTargetSource;
 
class RenderTarget: GenericEntity
{
    [Attribute(&quot;0&quot;, &quot;slider&quot;, &quot;Camera index&quot;, &quot;0 31 1&quot;)]
    int CameraIndex;
    [Attribute(&quot;0&quot;, &quot;editbox&quot;, &quot;Position X <0, 1>&quot;)]
    float X;
    [Attribute(&quot;0&quot;, &quot;editbox&quot;, &quot;Position Y <0, 1>&quot;)]
    float Y;
    [Attribute(&quot;1&quot;, &quot;editbox&quot;, &quot;Render target width <0, 1>&quot;)]
    float Width;
    [Attribute(&quot;1&quot;, &quot;editbox&quot;, &quot;Render target height <0, 1>&quot;)]
    float Height;
    [Attribute(&quot;-1&quot;, &quot;editbox&quot;, &quot;Sort index (the lesser the more important)&quot;)]
    int Sort;
    [Attribute(&quot;0&quot;, &quot;combobox&quot;, &quot;Autoinit&quot;, &quot;&quot;, { ParamEnum(&quot;No&quot;, &quot;0&quot;), ParamEnum(&quot;Yes&quot;, &quot;1&quot;) } )]
    int AutoInit;
    [Attribute(&quot;0&quot;, &quot;combobox&quot;, &quot;Forcing creation of render target for camera #0 in Workbench&quot;, &quot;&quot;, { ParamEnum(&quot;No&quot;, &quot;0&quot;), ParamEnum(&quot;Yes&quot;, &quot;1&quot;) } )]
    bool ForceCreation;
    bool m_Show = true; // when autoinit, wait with showing the render target after all entities are created (EOnInit)
    ref RenderTargetWidget m_RenderWidget;
    
    void RenderTarget(IEntitySource src, IEntity parent)
    {
        SetFlags(EntityFlags.ACTIVE, false);
 
        if (AutoInit)
        {
            m_Show = false;
            SetEventMask(EntityEvent.INIT);
            Init();
        }
    }
 
    void ~RenderTarget()
    {
        delete m_RenderWidget;
    }
    
    void Init()
    {
        #ifdef WORKBENCH // Workbench is using its own renderer for main camera, it is not using render target widget.
            if (!ForceCreation && CameraIndex == 0)
                return;
        #endif
 
        int screenW, screenH;
        GetScreenSize(screenW, screenH);
 
        int posX = (float)(screenW * X);
        int posY = (float)(screenH * Y);
        int widthPix = (float)(screenW * Width);
        int heightPix = (float)(screenH * Height);
        if (Class.CastTo(m_RenderWidget, g_Game.GetWorkspace().CreateWidget(RenderTargetWidgetTypeID, posX, posY, widthPix, heightPix, WidgetFlags.VISIBLE | WidgetFlags.HEXACTSIZE | WidgetFlags.VEXACTSIZE | WidgetFlags.HEXACTPOS | WidgetFlags.VEXACTPOS, 0xffffffff, Sort)))
        {   
            m_RenderWidget.Show(m_Show);    
            SetWidgetWorld(m_RenderWidget, g_Game.GetWorldEntity(), CameraIndex);
        }
    }
    override void EOnInit(IEntity other, int extra) 
    {
        if (m_RenderWidget)
        {
            m_Show = true;
            m_RenderWidget.Show(m_Show);
        }
    }
}
 
#endif
