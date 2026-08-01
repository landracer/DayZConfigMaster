// ======================================================================
// File: playerbaseclient_8c_source.html
// Category: other
// ======================================================================

class PlayerBaseClient extends PlayerBase
{
static ScriptedLightBase m_PersonalLight;
static bool m_PersonalLightEnabledOnCurrentServer = false; // "disablePersonalLight" in server.cfg decides if this is true or false
static bool m_PersonalLightDisabledByDebug = false;
static bool m_PersonalLightIsSwitchedOn = true;
static void CreatePersonalLight()
{
if (!m_PersonalLight && ( !g_Game.IsServer() || !g_Game.IsMultiplayer() ))
{
m_PersonalLight = ScriptedLightBase.CreateLight(PersonalLight, "0 0 0");
}
}
/*
override void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx)
{
super.OnRPC(sender, rpc_type, ctx);
switch( rpc_type )
{
case ERPCs.RPC_TOGGLE_PERSONAL_LIGHT:
{
Param1<bool> is_enabled = new Param1<bool>(false);
if (ctx.Read(is_enabled))
{
m_PersonalLightEnabledOnCurrentServer = is_enabled.param1;
UpdatePersonalLight();
}
break;
}
}
}*/
override void OnGameplayDataHandlerSync()
{
super.OnGameplayDataHandlerSync();
m_PersonalLightEnabledOnCurrentServer = !CfgGameplayHandler.GetDisablePersonalLight();
UpdatePersonalLight();
UpdateHitDirectionValues();
}
static void SwitchPersonalLight(bool state)
{
if ( !g_Game.IsServer() || !g_Game.IsMultiplayer() )
{
m_PersonalLightIsSwitchedOn = state;
UpdatePersonalLight();
}
}
static void UpdatePersonalLight()
{
string param;
CreatePersonalLight();
// Allow PL unless it&#39;s disabled by debug or client-side starting parameter
if ( !GetCLIParam("disablePersonalLight", param) && !m_PersonalLightDisabledByDebug && m_PersonalLightIsSwitchedOn )
{
m_PersonalLight.SetEnabled(m_PersonalLightEnabledOnCurrentServer);
}
else
{
m_PersonalLight.SetEnabled(false);
}
}
static void UpdateHitDirectionValues()
{
HitDirectionEffectBase.CheckValues();
}
}