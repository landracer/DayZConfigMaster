// Source: F:/Games/Dayz/scripts/3_game/tools/inpututils.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class InputUtils
{
    //useful for console preset differentiation
    static const string PRESET_OLD          = &quot;#STR_UAPRESET_0&quot;;
    static const string PRESET_NEW          = &quot;#STR_UAPRESET_1&quot;;
    static const int VARIANT_OLD            = 0;
    static const int VARIANT_NEW            = 1;
    static int m_CurrentPresetIDConsole     = -1;
    //
    
    static protected ref map<int, ref array<int>> m_InputActionSortingMap; //<sorting_idx,<input_ID>>
    static protected ref array<int> m_UnsortedInputActions;
    
    static const float ICON_SCALE_NORMAL    = 1.21;
    static const float ICON_SCALE_TOOLBAR   = 1.81;
    
    static string GetButtonNameFromInput(string pInputName, int pInputDeviceType)
    {
        UAInput inp = GetUApi().GetInputByName(pInputName);
        for (int i = 0; i < inp.AlternativeCount(); i++)
        {
            inp.SelectAlternative(i);
            if (inp.CheckBindDevice(0, pInputDeviceType))
            {
                return GetUApi().GetButtonName(inp.GetBindKey(0));
            }
        }
        
        return &quot;&quot;;
