// Source: F:/Games/Dayz/scripts/3_game/tools/keystouielements.c Source File
// Extracted from DayZ Code Explorer by Zeroy

//
// Keep this Object up-to-date with KeyCodes Enum in EnSystem.c
//
// KeysToUIElements is mapping of KeyCodes to its literal name or icon from ImageSet
// that will be displayed in floating widget for Default Actions - aka Action Target Selection
 
typedef Param2<string, bool> KeyToUIElement;
 
class KeysToUIElements
{
    static private ref map<int, ref KeyToUIElement> m_KeysToUIElements;
    
    static private void RegisterKeyToUIElement(int key_code, string name, bool is_image_set )
    {
        if ( m_KeysToUIElements == NULL )
        {
            m_KeysToUIElements = new map<int, ref KeyToUIElement>;
        }
        
        if ( m_KeysToUIElements.Contains(key_code) )
        {
            Debug.Log(&quot;Template ID: &quot;+string.ToString(key_code)+&quot; is alredy exist!&quot;, &quot;KeysToUIElements -> OnInit()&quot;, &quot;System&quot;, &quot;KeysToUIElements Registration&quot;, &quot;None&quot;);
        }
        else
        {
            KeyToUIElement params = new KeyToUIElement(name, is_image_set);
            m_KeysToUIElements.Set(key_code, params);
        }
