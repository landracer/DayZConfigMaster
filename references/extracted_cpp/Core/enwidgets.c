// Source: F:/Games/Dayz/scripts/1_core/proto/enwidgets.c Source File
// Extracted from DayZ Code Explorer by Zeroy


 
#ifdef DOXYGEN

    TextWidgetTypeID,
    MultilineTextWidgetTypeID,
    MultilineEditBoxWidgetTypeID,
    RichTextWidgetTypeID,
    RenderTargetWidgetTypeID,
    ImageWidgetTypeID,
    ConsoleWidgetTypeID,
    VideoWidgetTypeID,
    RTTextureWidgetTypeID,
    FrameWidgetTypeID,
    EmbededWidgetTypeID,
    ButtonWidgetTypeID,
    CheckBoxWidgetTypeID,
    WindowWidgetTypeID,
    ComboBoxWidgetTypeID,
    SimpleProgressBarWidgetTypeID,
    ProgressBarWidgetTypeID,
    SliderWidgetTypeID,
    BaseListboxWidgetTypeID,
    TextListboxWidgetTypeID,
    GenericListboxWidgetTypeID,
    EditBoxWidgetTypeID,
    PasswordEditBoxWidgetTypeID,
    WorkspaceWidgetTypeID,
    GridSpacerWidgetTypeID,
    WrapSpacerWidgetTypeID,
    ScrollWidgetTypeID, 
#else
    typedef TypeID WidgetType;
#endif
 
typedef TypeID EventType;
 
enum WidgetFlags
{
    SOURCEALPHA, //< takes alpha from texture * alpha from color. If not set, considers texture as non-transparent and transparency is set only by color
    BLEND,  //< texture is blended with a surface base on alpha
    ADDITIVE,   //< texture is added to a surface based on alpha
    VISIBLE,    //<Widget is visible - ShowWidget(w, true)
    NOWRAP, //< Do not do texture wrapping
    CENTER, //< Centers TextWidgetTypeID
    VCENTER,    //< Centers TextWidgetTypeID verticaly
    HEXACTPOS,
    VEXACTPOS,
    EXACTPOS,   //< Uses physical resolution (g_iWidth, h_iHeight)
    HEXACTSIZE, //< Uses physical resolution (g_iWidth)
    VEXACTSIZE, //< Uses physical resolution (h_iHeight)
    EXACTSIZE,  //< Uses physical resolution
    NOFILTER,   //< no texture filtering (no blur)
    RALIGN, //< Right alignment TextWidgetTypeID
    STRETCH,    //< Stretch texture to a full size
    FLIPU,  //< Flips texture in U axis
    FLIPV,  //< Flips texture in V axis
    CUSTOMUV,   //< ignores STRETCH/FLIPU/FLIPV and take custom UV set by SetWidgetUV()
    IGNOREPOINTER,
    DISABLED,
    NOFOCUS,
    CLIPCHILDREN,
    RENDER_ALWAYS,
    NOCLEAR,
    DRAGGABLE
