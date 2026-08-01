// Source: F:/Games/Dayz/scripts/4_world/classes/playersymptoms/statebase.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class SymptomBase
{
    const float MAX_TIME_ACTIVE_SAVEGUARD = 20;
    int m_Priority;
    SoundOnVehicle m_SoundObject;
    bool m_PlayedSound;
    bool m_IsActivated;
    PlayerBase m_Player;
    float m_ServerUpdateInterval = 1;
    float m_ServerUpdateDelta;
    bool m_IsTemplate = true;
    float m_ActivatedTime;
    int m_ID;//ID for the type of Symptom
    int m_UID;//unique ID
    bool m_IsClientOnly;
    bool m_DestroyOnAnimFinish = true;
    bool m_DestroyRequested = false;
    int m_SymptomType = -1;
    bool m_IsPersistent = false;
    SymptomManager m_Manager;
    bool m_SyncToClient = false;
    float m_Duration;
    bool m_AnimPlayRequested;
    int m_MaxCount = -1;//how many symptoms of this type can be queued up at the same time, &#39;-1&#39; for unlimited
 
    SymptomCB m_AnimCallback;
    
    ref array<Param> m_PersistentParams = new array<Param>;
    
    void SymptomBase()
    {
