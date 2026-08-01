// Source: F:/Games/Dayz/scripts/5_mission/gui/newui/charactercreation/charactercreationmenu.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class CharacterCreationMenu extends UIScriptedMenu
{
    #ifdef PLATFORM_CONSOLE
    DayZIntroSceneXbox                                      m_Scene;
    protected bool                                          m_CharacterSaved;
    #else
    DayZIntroScenePC                                        m_Scene;
    #endif
    
    const int                                               TOOLTIP_ID_SAVE = 1;
    const int                                               TOOLTIP_ID_APPLY = 2;
    
    protected Widget                                        m_CharacterRotationFrame;
    protected Widget                                        m_Apply;
    protected Widget                                        m_Save;
    protected Widget                                        m_RandomizeCharacter;
    protected Widget                                        m_BackButton;
    protected Widget                                        m_PlayedCharacterInfo;
    protected TextWidget                                    m_Version;
    protected Widget                                        m_DetailsRoot; //tooltips
    protected TextWidget                                    m_DetailsLabel;
    protected RichTextWidget                                m_DetailsText;
    protected TextWidget                                    m_CharacterHeaderText;
    
    protected ref OptionSelectorEditbox                     m_NameSelector;
    protected ref OptionSelectorMultistateCharacterMenu     m_GenderSelector;
    protected ref OptionSelectorMultistateCharacterMenu     m_SkinSelector;
    protected ref OptionSelectorMultistateCharacterMenu     m_TopSelector;
    protected ref OptionSelectorMultistateCharacterMenu     m_BottomSelector;
    protected ref OptionSelectorMultistateCharacterMenu     m_ShoesSelector;
    
    protected ref map<Widget, OptionSelectorMultistateCharacterMenu> m_MultiOptionSelectors;
    
    int m_OriginalCharacterID;
    
    void CharacterCreationMenu()
    {
        MissionMainMenu mission = MissionMainMenu.Cast(g_Game.GetMission());
        
        #ifdef PLATFORM_CONSOLE
        m_Scene = mission.GetIntroSceneXbox();
        #else
        m_Scene = mission.GetIntroScenePC();
        #endif
        
        m_Scene.ResetIntroCamera();
