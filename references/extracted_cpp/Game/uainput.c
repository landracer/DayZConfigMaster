// Source: F:/Games/Dayz/scripts/3_game/inputapi/uainput.c Source File
// Extracted from DayZ Code Explorer by Zeroy


 
 
// constants for device (binding device) type determine:
//
//  EUAINPUT_DEVICE_KEYBOARD
//  EUAINPUT_DEVICE_MOUSE
//  EUAINPUT_DEVICE_KEYBOARDMOUSE
//  EUAINPUT_DEVICE_CONTROLLER
//  EUAINPUT_DEVICE_IR
 
// -------------------------------------------------------------------------
class UAIDWrapper
{
    private void UAIDWrapper() {}
    private void ~UAIDWrapper() {}
    
    proto native UAInput InputP();          // get input pointer
