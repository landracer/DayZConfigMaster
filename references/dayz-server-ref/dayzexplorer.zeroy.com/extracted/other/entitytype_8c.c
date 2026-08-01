// ======================================================================
// File: entitytype_8c_source.html
// Category: other
// ======================================================================

class EntityType
{
protected void EntityType()
{
}
protected void ~EntityType()
{
}
proto native owned string GetName();
// config class API
proto string ConfigGetString(string entryName);
proto string ConfigGetStringRaw(string entryName);
proto int ConfigGetInt(string entryName);
bool ConfigGetBool(string entryName)
{
return (ConfigGetInt(entryName) == 1);
}
proto float ConfigGetFloat(string entryName);
proto vector ConfigGetVector(string entryName);
proto native void ConfigGetTextArray(string entryName, out TStringArray values);
proto native void ConfigGetTextArrayRaw(string entryName, out TStringArray values);
proto native void ConfigGetFloatArray(string entryName, out TFloatArray values);
proto native void ConfigGetIntArray(string entryName, out TIntArray values);
proto native bool ConfigIsExisting(string entryName);
};