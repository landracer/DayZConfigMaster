// Source: F:/Games/Dayz/scripts/3_game/effects/backlit/backlit.c Source File
// Extracted from DayZ Code Explorer by Zeroy

 
// Purpose of this file is to encapsulate Backlit Effects
// Basic implementatoin is for Razer Chroma devices, but API is made to remain universal for future purposes
//
// for specific method - watch UAInputAPI/ UAInput class
//
//
 
 
//-----------------------------------------------------------------------------
const int EUABLAYER_ALL             = 0;    // all layers
const int EUABLAYER_HINTKEY         = 1;    // showing hints
const int EUABLAYER_VISKEY          = 2;    // visualisation
const int EUABLAYER_3               = 3;    // 
const int EUABLAYER_CAR             = 4;    // 
const int EUABLAYER_HUMAN           = 5;    // 
const int EUABLAYER_6               = 6;    // 
const int EUABLAYER_CUSTOM          = 7;    // custom
const int EUABLAYER_LOADING         = 8;    // keys during loading sequence
const int EUABLAYER_MENU            = 9;    // keys during menu
 
 
//-----------------------------------------------------------------------------
const int EUABACKLIT_NONE               = 0;    // turn off
const int EUABACKLIT_ON                 = 1;    // permanent ilumination
const int EUABACKLIT_2                  = 2;
const int EUABACKLIT_3                  = 3;
const int EUABACKLIT_FADEOUT_SLOW       = 4;    // slow fadeout
const int EUABACKLIT_FADEOUT_FAST       = 5;    // fast fadeout
const int EUABACKLIT_FADEIN_SLOW        = 6;    // slow in -> then stay on
const int EUABACKLIT_FADEIN_FAST        = 7;    // fast in -> then stay on
 
 
// note: there should be states like:
//
//
//      LOADING/ MAIN MENU/ CONNECTING/ KEYBINDING/ OPTIONS - continuous
//      CHARACTER/ CAR/ &quot;HELI&quot;/ SPECTATOR/ INFECTED - continuous BASE
//      SWIMMING/ CRAWLING/ SPRINTING/ FIGHTING/ FLARE/ FLASHLIGHT - continuous CHARACTER
//
//  DoorOpen/ HitTaken/ CarCrashed/ etc. - event based
//
 
 
const int EUAB_OFF                      = 0;    // all off
 
// logos :: anims
const int EUAB_LOGO_DAYZ                = 10;
const int EUAB_LOGO_CONNECTING          = 11;
 
// menus :: anims
const int EUAB_MENU_BROWSER             = 20;
const int EUAB_MENU_MAIN                = 21;
const int EUAB_MENU_KEYBINDING          = 22;
const int EUAB_MENU_OPTIONS             = 23;
 
// car :: basic background colour + overlay + transition anims
const int EUAB_CAR_OFF                  = 100;  // sitting in car with engine off
const int EUAB_CAR_ON_NOLIGHTS          = 101;  // driving w/o lights (day)
const int EUAB_CAR_ON_LIGHTS            = 102;  // driving with headlights (night)
const int EUAB_CAR_STARTING             = 103;  // short starting animation
const int EUAB_CAR_ENTER                = 104;  // entering car
const int EUAB_CAR_LEAVE                = 105;  // leaving car
const int EUAB_CAR_CRASH                = 106;  // crashed
 
// character :: basic background colour + overlay + transition anims
const int EUAB_PLR_WALK                 = 200;
const int EUAB_PLR_RUN                  = 201;
const int EUAB_PLR_SPRINT               = 202;
const int EUAB_PLR_SWIM                 = 203;
const int EUAB_PLR_HITBY                = 204;
const int EUAB_PLR_CONSUME              = 205;
const int EUAB_PLR_CRAFTING             = 206;
const int EUAB_PLR_EMOTE                = 207;  // playing emote
const int EUAB_PLR_UNCONSCIOUS          = 208;
const int EUAB_PLR_DEAD                 = 209;
 
// modes :: these are set of background colours
const int EUAB_MODE_NIGHT               = 300;
const int EUAB_MODE_MORNING_BAD         = 301;
const int EUAB_MODE_MORNING_GOOD        = 302;
const int EUAB_MODE_DAY_FOGGY           = 303;
const int EUAB_MODE_DAY_OVERCAST        = 304;
const int EUAB_MODE_DAY_NICE            = 305;
const int EUAB_MODE_EVENING_BAD         = 306;
const int EUAB_MODE_EVENING_GOOD        = 307;
 
const int EUAB_MODE_FLASHLIGHT          = 320;
const int EUAB_MODE_CHEMLIGHT           = 321;
 
 
// overlay types
const int EUAB_OVERLAY_NONE             = 400;  // no overlay
const int EUAB_OVERLAY_CONTROLS         = 401;  // highlighted controls
const int EUAB_OVERLAY_STATUS           = 402;  // numpad + mouse used for health/ blood level visualisation
const int EUAB_OVERLAY_VON              = 403;  // VON status
 
 
//-----------------------------------------------------------------------------
 
class Backlit
{
//  private void ~Backlit(); // raist todo: turn lights off there?
 
    bool m_BacklitActive;
 
    void OnInit( DayZGame game )
    {
        // enable only on client/ standalone!
        if( game.IsClient() || !game.IsMultiplayer() )
            m_BacklitActive = true;
            
        if( m_BacklitActive )
            Print(&quot;... Backlit Effects Enabled&quot;);
