// Source: F:/Games/Dayz/scripts/3_game/effects/effectsound.c Source File
// Extracted from DayZ Code Explorer by Zeroy


class EffectSound : Effect
{
    ref ScriptInvoker Event_OnSoundWaveStarted      = new ScriptInvoker();
    ref ScriptInvoker Event_OnSoundWaveEnded        = new ScriptInvoker();
    ref ScriptInvoker Event_OnSoundFadeInStopped    = new ScriptInvoker();
    ref ScriptInvoker Event_OnSoundFadeOutStarted   = new ScriptInvoker();
    
    protected ref SoundParams           m_SoundParams;
    protected ref SoundObjectBuilder    m_SoundObjectBuilder;
    protected ref SoundObject           m_SoundObject;
    protected AbstractWave              m_SoundWaveObject;
    
    protected WaveKind                  m_SoundWaveKind;
    protected string                    m_SoundSetName; 
    protected bool                      m_SoundLoop;
    protected bool                      m_SetEnvVariables;
    protected bool                      m_SoundAutodestroy;
    protected bool                      m_SoundWaveIsPlaying;
    protected float                     m_SoundWaveLenght;
    protected float                     m_SoundWaveVolume;
    protected float                     m_SoundWaveVolumeMax;
    protected float                     m_SoundWaveTime;
    protected int                       m_SoundDoppler;
    
    protected bool                      m_SoundWaveStarting;
    protected bool                      m_SoundWaveStopping;
    protected bool                      m_SoundFadedOut;
    
    protected float                     m_SoundFadeInDuration;
    
    protected float                     m_SoundFadeOutStartTime;
    protected float                     m_SoundFadeOutDuration;
    protected float                     m_SoundFadeOutInitVolume;
    
    
    
    void EffectSound()
    {
        m_SoundWaveKind = WaveKind.WAVEEFFECTEX;
        m_SoundWaveVolume = 0;
        m_SoundWaveVolumeMax = 1;
        m_SoundAutodestroy = false;
        m_SoundWaveStopping = false;
        m_SoundFadedOut = false;
        m_SoundDoppler = -1;
