// ======================================================================
// File: proto_8c_source.html
// Category: other
// ======================================================================

/*
Function/method modifiers:
proto - prototyping of internal function (C++ side)
native - native call convention of internal function (C++ side)
volatile - internal function that may call back to script (hint for
compiler that context need to be saved on stack)
private - function may not be called from script
event - hint for tools that the function should be exposed as
Entity script event.
Variable modifiers:
owned - modifier for returing internal functions. Tells to script-VM,
that returning variable (string or array) must not be released
out - modifier for function parameters. It tells that variable will
be changed by function call (used mainly by internal functions)
inout - modifier for function parameters. It tells that variable will
be used and then changed by function call (used mainly by internal functions)
const - constants. May not be modified.
reference - hint for tools (Material editor), that the variable may be used
as parameter in material
*/
/*===================================================================*/
/* Enforce engine API */
/*===================================================================*/
//placeholder
class AnimEvent
{
int type;
int slot;
};
class SoundEvent
{
int type;
int handle;
};
typedef int[] vobject;
class vobject
{
proto native IEntitySource ToEntitySource();
}
#ifdef ENF_DONE
//------------------------------------------
// SOUND API
//------------------------------------------
//TODO:
typedef int[] HSOUND;
//flags
//SFX_ONCE
//SFX_MUSIC
//SFX_AMBIENT
//SFX_3D
//SFX_DISCARDABLE
//SFX_DIRECTIONAL
//SFX_DOPPLER
//SFX_STATIC
//SFX_NOTEST
//proto volatile HSOUND PlaySound(int soundScene, IEntity source, string sshader, int flags);
proto volatile native void EndSound(HSOUND snd);
//volume 0...1. Logaritmic scale
proto native int SetSoundVolume(HSOUND sound, float volume)
proto native int SetSoundFrequency(HSOUND sound, int freq)
//returns 0, if soundid is not valid
proto native int GetSoundLength(HSOUND sound)
//returns -1, if soundid is not valid
proto native int GetSoundPosition(HSOUND sound)
//defaultne se pouziva EAX prostredi nadefinovane v mape, ale lze ho prebit touto fci.
// Mohou se michat dve ruzna prostredi v pomeru danem hodnotou fade (fade==0 -> 100% env1, fade==1 -> 100% env2).
//pokud chceme michat aktualni prostredi s nejakym jinym, lze pouzit zastupny nazev "$current".
//Tim lze dosahnout nafadovani vlastniho prostredi, kdyz fci postupne volame napr. takto:
//SetEAXEnvironment("$current", "Drugged", fade)
//pricemz hodnota fade postupne narusta od nuly do jedne
//proto native bool SetEAXEnvironment(string env1, string env2, float fade)
#endif
class PacketOutputAdapter
{
proto native void WriteBool(bool value); //size: 1 byte
proto native void WriteInt(int value); //size: 4 bytes
proto native void WriteFloat(float value); //size: 4 bytes
proto native void WriteString(string value); //size: n bytes (string length)
proto native void WriteVector(vector value); //size: 12 bytes
proto native void WriteMatrixAsQuaternionVector(vector mat[4]); //size: 28 bytes
proto native void WriteIntAsByte(int value); //write int in range <-128, 127> size: 1 byte
proto native void WriteIntAsUByte(int value); //write int in range <0, 255> size: 1 byte
proto native void WriteIntAsHalf(int value); //write int in range <-32768, 32768> size: 2 bytes
proto native void WriteIntAsUHalf(int value); //write int in range <0, 65535> size: 2 bytes
proto native void WriteFloatAsByte(float value, float min, float max); // size: 1 byte
proto native void WriteFloatAsHalf(float value, float min, float max); // size: 2 bytes
};
class PacketInputAdapter
{
proto native bool ReadBool();
proto native int ReadInt();
proto native float ReadFloat();
proto string ReadString();
proto native vector ReadVector();
proto void ReadMatrixAsQuaternionVector(vector mat[4]);
proto native int ReadIntAsByte();
proto native int ReadIntAsUByte();
proto native int ReadIntAsHalf();
proto native int ReadIntAsUHalf();
proto native float ReadFloatAsByte(float min, float max);
proto native float ReadFloatAsHalf(float min, float max);
};
proto native void MakeScreenshot(string name);
proto native int GetFPS();
//----------------------------------------------
proto native int LoadSkyPresets(string presetsFile);
proto native int InitSky(string presetName);
proto native int SetSkyPreset(string presetName, float stormy, float dayTime);
proto native int LerpSkyPreset(string presetName1, string presetName2, float dayTime, float stormy1, float stormy2, float lerpVal);
proto native int LerpSkyPreset3(string presetName1, string presetName2, string presetName3, float dayTime, float stormy1, float stormy2, float stormy3, float w1, float w2, float w3);
proto native void SetSkyUserPlanets(bool enabled);
proto native bool SetSkyPlanet(int index, float azimuthDeg, float zenithDeg);
proto native bool SetSkyPlanetSize(int index, float angleDeg);
proto native void SetStarsObserverTime(int year, int month, int day, int hour, int minute, float sec, int offsetSec);
proto native void SetStarsObserverPosition(float latitudeDeg, float longitudeDeg);
proto native void SetRealStarAutoUpdate(bool update);
proto native void SetNightLayerRotMatrix(vector mat[3]);
proto native void SetStarsRotMatrix(vector mat[3]);
//----------------------------------------------
typedef int[] Material;
class Material
{
proto bool SetParam(string propertyName, void value);
proto native void ResetParam(string propertyName);
proto native int GetParamIndex(string paramName);
proto void SetParamByIndex(int paramIndex, void value);
};
int VectortoRGBA( vector vec, float h)
{
float x,y,z;
int r,g,b,a,rgba;
x = vec[0];
y = vec[1];
z = vec[2];
x = x * 127.0 + 128.0;
y = y * 127.0 + 128.0;
z = z * 127.0 + 128.0;
h = h * 255.0;
a = (int)h << 24;
r = (int)x << 16;
g = (int)y << 8;
b = z;
return r | g | b | a;
}
//-----------------------------------------------------------------
int ARGB(int a, int r, int g, int b)
{
a = a << 24;
r = r << 16;
g = g << 8;
return a | r | g | b;
}
//-----------------------------------------------------------------
int ARGBF(float fa, float fr, float fg, float fb)
{
return ARGB((float)(fa * 255.0), (float)(fr * 255.0), (float)(fg * 255.0), (float)(fb * 255.0));
}
//-----------------------------------------------------------------
int AWHITE(int a)
{
return a << 24 | 0xffffff;
}
//-----------------------------------------------------------------
int LerpARGB(int c1, int c2)
{
int cb1, cb2;
const int cmask = 0x00ff00ff;
cb1 = c1 >> 8 & cmask;
cb2 = c2 >> 8 & cmask;
cb1 = cb1 + cb2 >> 1;
c1 = c1 & cmask;
c2 = c2 & cmask;
c1 = c1 + c2 >> 1;
return cb1 << 8 | c1;
}
//-------------------------------------------------------------------------
class Link<Class T>
{
proto private native void Init(T init);
proto private native Object Get();
void Release()
{
T obj = Get();
if(obj)
obj.Release();
}
void Link(T init)
{
Init(init);
}
T Ptr()
{
return Get();
}
bool IsNull()
{
if(!Get())
return true;
return false;
}
};