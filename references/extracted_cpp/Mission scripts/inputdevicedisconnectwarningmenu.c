// Source: F:/Games/Dayz/scripts/5_mission/gui/inputdevicedisconnectwarningmenu.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class InputDeviceDisconnectWarningMenu extends UIScriptedMenu
{
    private ref array<int> m_DisconnectedDevices;
    private ref array<int> m_DeviceOrder = {EUAINPUT_DEVICE_CONTROLLER,EUAINPUT_DEVICE_MOUSE,EUAINPUT_DEVICE_KEYBOARD};
    private ref map<int,string> m_DeviceMessages;
    private bool m_CanClose;
    private bool m_Initialized;
    private string m_DevicesText;
    private string m_CaptionText;
    private string m_ConfirmationText;
    
    private WrapSpacerWidget    m_WrapperWidget;
    private TextWidget          m_CaptionWidget;
    private RichTextWidget      m_DeviceListWidget;
    private RichTextWidget      m_ActionTextWidget;
    
    void InputDeviceDisconnectWarningMenu()
    {
        g_Game.GetMission().AddActiveInputExcludes({&quot;gamepaddisconnect&quot;});
        
        m_DisconnectedDevices = new array<int>;
        m_CanClose = false;
        
        BuildMessages();
