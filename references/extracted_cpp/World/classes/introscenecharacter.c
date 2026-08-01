// Source: F:/Games/Dayz/scripts/4_world/classes/introscenecharacter.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class IntroSceneCharacter extends Managed
{
    protected int           m_CharacterId;
    protected string        m_CharacterType;
    protected MenuData      m_CharacterDta;
    protected PlayerBase    m_CharacterObj;
    protected vector        m_CharacterPos;
    protected vector        m_CharacterRot;
        
    protected ref TStringArray  m_CharGenderList                = new TStringArray;
    protected ref TStringArray  m_CharShirtList                 = new TStringArray; //legacy
    protected ref TStringArray  m_CharPantsList                 = new TStringArray; //legacy
    protected ref TStringArray  m_CharShoesList                 = new TStringArray; //legacy
    
    protected ref map<ECharGender, ref array<string>> m_Characters = new map<ECharGender, ref array<string>>;
    
    protected ECharGender       m_CharGender;
    
    void IntroSceneCharacter()
    {
        m_CharacterId = GameConstants.DEFAULT_CHARACTER_MENU_ID;
