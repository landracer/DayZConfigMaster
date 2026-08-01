// Source: F:/Games/Dayz/scripts/4_world/static/betasound.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class BetaSound
{
    static SoundOnVehicle SaySound(Object source, string  sound_name, float distance, bool looped)  
    {
        bool is_female = source.ConfigGetBool(&quot;woman&quot;);
        
        
        string path_class = &quot;CfgActionSounds &quot; + sound_name;
        string path_sound = path_class + &quot; sounds&quot;;
        //is the action sound class defined ? if not, try to play it as a regular sound
        if( GetDayZGame().ConfigIsExisting(path_sound) )
        {
            string path_sound_female = path_class + &quot;_female&quot; + &quot; sounds&quot;;
            
            if(is_female && GetDayZGame().ConfigIsExisting(path_sound_female))
            {
                path_sound = path_sound_female;
            }
            
            g_Game.ConfigGetTextArray(path_sound,CachedObjectsArrays.ARRAY_STRING);
            int rnd_index = Math.RandomInt(0,CachedObjectsArrays.ARRAY_STRING.Count());
            string sound_class = CachedObjectsArrays.ARRAY_STRING.Get(rnd_index);
            SoundOnVehicle sound_object = g_Game.CreateSoundOnObject(source, sound_class, distance, looped);
            return sound_object;
        }
        else
        {
            return g_Game.CreateSoundOnObject(source, sound_name, distance, looped);
        }
