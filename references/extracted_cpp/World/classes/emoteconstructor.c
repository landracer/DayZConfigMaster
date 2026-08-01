// Source: F:/Games/Dayz/scripts/4_world/classes/emoteconstructor.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class EmoteConstructor
{
    bool ConstructEmotes(PlayerBase player, out map<int, ref EmoteBase> emoteMap)
    {
        TTypenameArray emote_array_names = new TTypenameArray;
        RegisterEmotes(emote_array_names);
        Sort(emote_array_names,emote_array_names.Count());
        
        emoteMap = new map<int, ref EmoteBase>;
        ref EmoteBase new_emote;
        
        for (int i = 0; i < emote_array_names.Count(); i++)
        {
            new_emote = EmoteBase.Cast(emote_array_names[i].Spawn());
            if (new_emote)
            {
                if (new_emote.GetID() < 0)
                {
                    ErrorEx(&quot;Emote &quot; + emote_array_names[i].ToString() + &quot; has an invalid ID, registation failed!&quot;);
                    return false;
                }
                else if (emoteMap.Contains(new_emote.GetID()))
                {
                    ErrorEx(&quot;Emote &quot; + emote_array_names[i].ToString() + &quot; has a duplicate ID, registation failed!&quot;);
                    return false;
                }
                else
                {
                    new_emote.SetOwnerPlayer(player); 
                    emoteMap.Insert(new_emote.GetID(), new_emote);
                }
            }
        }
        return true;
