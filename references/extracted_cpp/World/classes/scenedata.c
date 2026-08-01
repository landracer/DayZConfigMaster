// Source: F:/Games/Dayz/scripts/4_world/classes/sceneeditor/scenedata.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class SceneData
{
    protected string    m_NameScene;
    protected string    m_NameMission;
    protected float     m_InitTime;
    protected int       m_DateInitYear;
    protected int       m_DateInitMonth;
    protected int       m_DateInitDay;
    protected int       m_DateInitHour;
    protected int       m_DateInitMinute;
    protected float     m_WeaterInitOvercast;
    protected float     m_WeaterInitRain;
    protected float     m_WeaterInitFog;
    protected float     m_WeaterInitWindForce;
    
    protected ref array<ref SceneObject>    m_Objects;
    protected ref array<ref ScenePlayer>    m_Players;
    protected ref array<ref SceneObject>    m_AllObjs;
    
    //========================================
    // Constructor -> SceneData
    //========================================
    void SceneData()
    {
        m_Objects = new array<ref SceneObject>;
        m_Players = new array<ref ScenePlayer>;
        m_AllObjs = new array<ref SceneObject>;
