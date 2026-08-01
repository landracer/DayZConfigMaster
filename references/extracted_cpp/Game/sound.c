// Source: F:/Games/Dayz/scripts/3_game/sound.c Source File
// Extracted from DayZ Code Explorer by Zeroy

enum WaveKind 
{
    WAVEEFFECT,
    WAVEEFFECTEX,
    WAVESPEECH,
    WAVEMUSIC,
    WAVESPEECHEX,
    WAVEENVIRONMENT,
    WAVEENVIRONMENTEX,
    WAVEWEAPONS,
    WAVEWEAPONSEX,
    WAVEATTALWAYS,
    WAVEUI
}
 

 
//----------------------------------------------
 
enum SoundControllerAction
{
    None,
    Limit,
    Overwrite
}

proto native void SetSoundControllerOverride(string controllerName, float value, SoundControllerAction action);

proto native void MuteAllSoundControllers();

proto native void ResetAllSoundControllers();

 
 
class AbstractSoundScene
{
    private void AbstractSoundScene() {}
    private void ~AbstractSoundScene() {}
    
    proto native AbstractWave Play2D(SoundObject soundObject, SoundObjectBuilder soundBuilder);
    proto native AbstractWave Play3D(SoundObject soundObject, SoundObjectBuilder soundBuilder);
    proto native SoundObject BuildSoundObject(SoundObjectBuilder soundObjectbuilder);
 
    proto native float GetRadioVolume();
    proto native void SetRadioVolume(float vol, float time);
 
    proto native float GetSpeechExVolume();
    proto native void SetSpeechExVolume(float vol, float time);
 
    proto native float GetMusicVolume();
    proto native void SetMusicVolume(float vol, float time);
 
    proto native float GetSoundVolume();
    proto native void SetSoundVolume(float vol, float time);
 
    proto native float GetVOIPVolume();
    proto native void SetVOIPVolume(float vol, float time);
    
    proto native float GetSilenceThreshold();
    proto native float GetAudioLevel();
