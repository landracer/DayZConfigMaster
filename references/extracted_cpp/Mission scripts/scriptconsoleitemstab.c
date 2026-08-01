// Source: F:/Games/Dayz/scripts/5_mission/gui/scriptconsoleitemstab.c Source File
// Extracted from DayZ Code Explorer by Zeroy

typedef Param3<string, bool, bool> PresetParams;//  param1 - ??, param2 - ??, param3 - ??
 
class ScriptConsoleItemsTab : ScriptConsoleTabBase
{
    protected const string                      CONST_DEFAULT_PRESET_PREFIX = &quot;[Default]&quot;;
    protected static int                        ITEMS_SELECTED_ROW = -1;
    protected static int                        PRESETS_SELECTED_ROW = 0;
    protected static int                        ITEMS_IN_PRESET_SELECTED_ROW;
    protected static float                      DRAW_DISTANCE = 1000;
    protected static bool                       CLEAR_IVN;
    protected static int                        m_ObjectsScope = 2;
    protected static string                     m_LastSelectedObject;
    protected static int                        m_SelectedObjectIsPreset = -1;
    protected static ref array<Shape>           m_DebugShapes = new array<Shape>;
    protected static EntityAI                   m_PreviewEntity;
    protected static float                      m_ItemQuantity = 1;
    protected static bool                       m_WithPhysics = false;
    
    
    protected ref array<Widget>                 m_CategoryButtonsWidgets = new array<Widget>;
    protected ref array<string>                 m_CategoryButtonsNames = {&quot;FIREARMS&quot;,&quot;MELEE&quot;,&quot;ATTACHMENTS&quot;,&quot;MAGAZINES&quot;,&quot;AMMO&quot;,&quot;FOOD&quot;,&quot;MEDICAL&quot;,&quot;CARS&quot;,&quot;BACKPACKS&quot;,&quot;B-BUILDING&quot;};
    protected ref map<Widget, string>           m_SpawnButtonsOriginalText = new map<Widget, string>();
    protected int                               m_CategoryMask;
    protected MissionGameplay                   m_MissionGameplay;
    protected ref Timer                         m_RefreshFilterTimer = new Timer();
    protected PluginDeveloper                   m_Developer;
    protected bool                              m_FilterOrderReversed;
    protected string                            m_SelectedObject;
    protected string                            m_FilterTextPrev;
    //Widgets
    protected CheckBoxWidget            m_CategoryMergeType;
    protected CheckBoxWidget            m_ItemPreviewCheckbox;
    protected CheckBoxWidget            m_ShowProtected;
    protected CheckBoxWidget            m_ClearInventory;
    protected TextListboxWidget         m_ObjectsTextListbox;
    protected TextListboxWidget         m_PresetsTextListbox;
    protected TextListboxWidget         m_PresetItemsTextListbox;
    protected ButtonWidget              m_DrawInWorld;
    protected ButtonWidget              m_DrawInWorldClear; 
    protected ButtonWidget              m_PresetNewButton;
    protected ButtonWidget              m_PresetDeleteButton;
    protected ButtonWidget              m_PresetRenameButton;
    protected ButtonWidget              m_PresetSetDefaultButton;
    protected ButtonWidget              m_PresetAddItemtButton;
    protected ButtonWidget              m_ItemNameCopy;
    protected ButtonWidget              m_PresetRemoveItemButton;
    protected ButtonWidget              m_ItemMoveUpButton;
    protected ButtonWidget              m_ItemMoveDownButton;
    protected ButtonWidget              m_FilterOrderButton;
    protected ButtonWidget              m_SpawnInInvButton;
    protected ButtonWidget              m_SpawnGroundButton;
    protected ButtonWidget              m_SpawnGroundPatternGrid;
    protected ButtonWidget              m_SpawnOnTarget;
    protected ButtonWidget              m_SpawnOnCursor;
    protected ButtonWidget              m_SpawnInHands;
    protected ButtonWidget              m_SpawnBatchButton;
    protected ButtonWidget              m_ListActions;
    protected MultilineEditBoxWidget    m_ObjectFilter;
    protected EditBoxWidget             m_SpawnDistanceEditBox;
    protected EditBoxWidget             m_RectSpawnRow;
    protected EditBoxWidget             m_RectSpawnColumn;
    protected EditBoxWidget             m_RectSpawnRowStep;
    protected EditBoxWidget             m_RectSpawnColumnStep;
    protected EditBoxWidget             m_DrawDistanceWidget;
    protected EditBoxWidget             m_QuantityEditBox;
    protected EditBoxWidget             m_DamageEditBox;
    protected EditBoxWidget             m_BatchSpawnQuantity;
    protected CheckBoxWidget            m_WithPhysicsCheckbox;
    protected ButtonWidget              m_ReloadShapeButton;
    protected TextWidget                m_ItemDamageLabel;
    protected TextWidget                m_ItemQuantityLabel;
    protected TextWidget                m_SelectedObjectText;
    protected TextWidget                m_SelectedObjectLocalized;
    protected ItemPreviewWidget         m_ItemPreviewWidget;
    protected ImageWidget               m_FilterOrderImage;
    
    
    void ScriptConsoleItemsTab(Widget root, ScriptConsole console, Widget button, ScriptConsoleTabBase parent = null)
    {
        m_Developer                 = PluginDeveloper.Cast(GetPlugin(PluginDeveloper));
        
        m_DrawInWorld = ButtonWidget.Cast(root.FindAnyWidget(&quot;ButtonDrawItem&quot;));
        m_DrawInWorldClear = ButtonWidget.Cast(root.FindAnyWidget(&quot;ButtonDrawItemClear&quot;));
        
        m_CategoryMergeType = CheckBoxWidget.Cast(root.FindAnyWidget(&quot;CategoryMergeType&quot;));
        m_ItemPreviewCheckbox   = CheckBoxWidget.Cast(root.FindAnyWidget(&quot;ItemPreviewCheckbox&quot;));
        m_ReloadShapeButton = ButtonWidget.Cast(root.FindAnyWidget(&quot;ReloadShapeButton&quot;));
        m_ShowProtected     = CheckBoxWidget.Cast(root.FindAnyWidget(&quot;ShowProtectedCheckbox&quot;));
        m_FilterOrderImage  = ImageWidget.Cast(root.FindAnyWidget(&quot;ObjectFilterSortPic&quot;));
        m_ClearInventory        = CheckBoxWidget.Cast(root.FindAnyWidget(&quot;ForceClearCheckbox&quot;));
        m_DrawDistanceWidget    = EditBoxWidget.Cast(root.FindAnyWidget(&quot;DrawItemDistance&quot;));
        m_ObjectFilter = MultilineEditBoxWidget.Cast(root.FindAnyWidget(&quot;ObjectFilter&quot;));
        m_SpawnDistanceEditBox = EditBoxWidget.Cast(root.FindAnyWidget(&quot;SpawnDistance&quot;));
        m_SelectedObjectText = TextWidget.Cast(root.FindAnyWidget(&quot;SelectedObject&quot;));
        m_SelectedObjectLocalized = TextWidget.Cast(root.FindAnyWidget(&quot;SelectedObjectLocalized&quot;));
        m_ObjectsTextListbox = TextListboxWidget.Cast(root.FindAnyWidget(&quot;ObjectsList&quot;));
        m_PresetsTextListbox = TextListboxWidget.Cast(root.FindAnyWidget(&quot;PresetList&quot;));
        m_PresetItemsTextListbox = TextListboxWidget.Cast(root.FindAnyWidget(&quot;PresetItemsList&quot;));
        m_PresetNewButton = ButtonWidget.Cast(root.FindAnyWidget(&quot;NewPresetButton&quot;));
        m_PresetDeleteButton = ButtonWidget.Cast(root.FindAnyWidget(&quot;DeletePresetButton&quot;));
        m_PresetRenameButton = ButtonWidget.Cast(root.FindAnyWidget(&quot;RenamePresetButton&quot;));
        m_PresetSetDefaultButton = ButtonWidget.Cast(root.FindAnyWidget(&quot;SetDefaultPresetButton&quot;));
        m_PresetAddItemtButton = ButtonWidget.Cast(root.FindAnyWidget(&quot;AddToPresetButton&quot;));
        m_ItemNameCopy = ButtonWidget.Cast(root.FindAnyWidget(&quot;ItemNameCopy&quot;));
        m_PresetRemoveItemButton = ButtonWidget.Cast(root.FindAnyWidget(&quot;RemoveFromPresetButton&quot;));
        m_ItemMoveUpButton = ButtonWidget.Cast(root.FindAnyWidget(&quot;ItemMoveUpButton&quot;));
        m_ItemMoveDownButton = ButtonWidget.Cast(root.FindAnyWidget(&quot;ItemMoveDownButton&quot;));
        m_ItemPreviewWidget = ItemPreviewWidget.Cast(root.FindAnyWidget(&quot;ItemPreviewWidget&quot;));
 
        m_QuantityEditBox = EditBoxWidget.Cast(root.FindAnyWidget(&quot;QuantityValue&quot;));
        m_DamageEditBox = EditBoxWidget.Cast(root.FindAnyWidget(&quot;DamageValue&quot;));
        m_ItemDamageLabel = TextWidget.Cast(root.FindAnyWidget(&quot;DamageLabel&quot;));
        m_ItemQuantityLabel = TextWidget.Cast(root.FindAnyWidget(&quot;QuantityLabel&quot;));
        m_WithPhysicsCheckbox = CheckBoxWidget.Cast(root.FindAnyWidget(&quot;WithPhysicsCheckbox&quot;));
 
        m_SpawnInInvButton = ButtonWidget.Cast(root.FindAnyWidget(&quot;ButtonSpawnInInv&quot;));
        m_SpawnGroundButton = ButtonWidget.Cast(root.FindAnyWidget(&quot;ButtonSpawnInGround&quot;));
        m_SpawnOnTarget = ButtonWidget.Cast(root.FindAnyWidget(&quot;ButtonSpawnInTarget&quot;));
        m_SpawnOnCursor = ButtonWidget.Cast(root.FindAnyWidget(&quot;ButtonSpawnCursor&quot;));
        m_SpawnInHands = ButtonWidget.Cast(root.FindAnyWidget(&quot;ButtonSpawnHands&quot;));
        m_SpawnBatchButton = ButtonWidget.Cast(root.FindAnyWidget(&quot;ButtonSpawnBatch&quot;));
        
        string text;
        m_SpawnInInvButton.GetText(text);
        m_SpawnButtonsOriginalText.Insert(m_SpawnInInvButton, text);
        m_SpawnGroundButton.GetText(text);
        m_SpawnButtonsOriginalText.Insert(m_SpawnGroundButton, text);
        m_SpawnOnTarget.GetText(text);
        m_SpawnButtonsOriginalText.Insert(m_SpawnOnTarget, text);
        m_SpawnOnCursor.GetText(text);
        m_SpawnButtonsOriginalText.Insert(m_SpawnOnCursor, text);
        m_SpawnInHands.GetText(text);
        m_SpawnButtonsOriginalText.Insert(m_SpawnInHands, text);
        m_SpawnBatchButton.GetText(text);
        m_SpawnButtonsOriginalText.Insert(m_SpawnBatchButton, text);
 
        m_ListActions = ButtonWidget.Cast(root.FindAnyWidget(&quot;ListActions&quot;));
 
        
        m_RectSpawnColumn = EditBoxWidget.Cast(root.FindAnyWidget(&quot;RectangleColumnValue&quot;));
        m_RectSpawnRow = EditBoxWidget.Cast(root.FindAnyWidget(&quot;RectangleRowValue&quot;));
        m_BatchSpawnQuantity = EditBoxWidget.Cast(root.FindAnyWidget(&quot;BatchCount&quot;));
        
        m_RectSpawnColumnStep = EditBoxWidget.Cast(root.FindAnyWidget(&quot;RectangleStepXValue&quot;));
        m_RectSpawnRowStep = EditBoxWidget.Cast(root.FindAnyWidget(&quot;RectangleStepYValue&quot;));
        
        m_FilterOrderButton = ButtonWidget.Cast(root.FindAnyWidget(&quot;ObjectFilterSortButton&quot;));
        Init();
