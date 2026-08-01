// ======================================================================
// File: jsonapistruct_8c_source.html
// Category: other
// ======================================================================

// -------------------------------------------------------------------------
// object which allow to parse upon generic JSON structure and format it back
//
//
class JsonApiStruct : Managed
{
void JsonApiStruct()
{
}
void ~JsonApiStruct()
{
}
void OnExpand()
{
}
void OnPack()
{
Print( "OnPack() ");
}
void OnBufferReady()
{
}
void OnSuccess( int errorCode )
{
// errorCode is EJsonApiError
}
void OnError( int errorCode )
{
// errorCode is EJsonApiError
}
void OnObject( string name )
{
Print( "OnObject: " + name );
}
void OnInteger( string name, int value )
{
Print( "OnInteger: " + value );
}
void OnFloat( string name, float value )
{
Print( "OnFloat: " + value );
}
void OnBoolean( string name, bool value )
{
Print( "OnBoolean: " + value );
}
void OnString( string name, string value )
{
Print( "OnString: " + value );
}
void OnVector( string name, vector value )
{
Print( "OnVector: " + value );
}
void OnStartArray( string name )
{
Print( "OnStartArray: " + name );
}
void OnEndArray( int itemCount )
{
Print( "OnEndArray: " + itemCount );
}
void OnItemObject( int index, string name )
{
Print( "OnItemObject: " + name );
}
void OnItemInteger( int index, int value )
{
Print( "OnItemInteger: " + value );
}
void OnItemFloat( int index, float value )
{
Print( "OnItemFloat: " + value );
}
void OnItemBoolean( int index, bool value )
{
Print( "OnItemBoolean: " + value );
}
void OnItemString( int index, string value )
{
Print( "OnItemString: " + value );
}
void OnItemVector( int index, vector value )
{
Print( "OnItemVector: " + value );
}
proto native void RegV( string name );
proto native void Push( JsonApiStruct obj );
proto native void StartObject( string name );
proto native void EndObject();
proto native void StoreObject( string name, JsonApiStruct obj );
proto native void StoreFloat( string name, float value );
proto native void StoreInteger( string name, int value );
proto native void StoreBoolean( string name, bool value );
proto native void StoreString( string name, string value );
proto native void StoreVector( string name, vector value );
proto native void StartArray( string name );
proto native void EndArray();
proto native void ItemObject( JsonApiStruct obj );
proto native void ItemFloat( float value );
proto native void ItemInteger( int value );
proto native void ItemBoolean( bool value );
proto native void ItemString( string value );
proto native void ItemVector( vector value );
proto native void SetDone();
proto native void SetFail();
proto native void Pack();
proto native void InstantPack();
proto native void ExpandFromRAW( string data );
proto native string AsString();
proto native bool PackToFile( string FileName );
proto native bool SaveToFile( string FileName );
proto native bool LoadFromFile( string FileName );
};