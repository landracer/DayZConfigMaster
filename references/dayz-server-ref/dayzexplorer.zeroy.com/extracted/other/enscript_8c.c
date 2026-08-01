// ======================================================================
// File: enscript_8c_source.html
// Category: other
// ======================================================================

class Class
{
proto native external bool IsInherited(typename type);
proto native owned external string ClassName();
string GetDebugName() { return ClassName(); }
proto native external typename Type();
proto external static typename StaticType();
static typename StaticGetType(typename t)
{
return t;
}
proto external string ToString();
proto static Class Cast(Class from);
proto static bool CastTo(out Class to, Class from);
private proto static bool SafeCastType(Class type, out Class to, Class from);
};
class Managed
{
};
class NonSerialized
{
};
typedef int[] TypeID;
class ScriptModule
{
private void ~ScriptModule();
proto volatile int Call(Class inst, string function, void parm);
proto volatile int CallFunction(Class inst, string function, out void returnVal, void parm);
proto volatile int CallFunctionParams(Class inst, string function, out void returnVal, Class parms);
proto native void Release();
static proto native ScriptModule LoadScript(ScriptModule parentModule, string scriptFile, bool listing);
};
//main script module (contains script.c and this file)
//ScriptModule g_Script;
class EnScript
{
private void EnScript() {}
private void ~EnScript() {}
static proto int GetClassVar(Class inst, string varname,int index, out void result);
static proto int SetClassVar(Class inst, string varname, int index, void input);
static proto int SetVar(out void var, string value);
static proto void Watch(void var, int flags);
};
proto void Sort(void param_array[], int num);
proto void reversearray(void param_array);
proto void copyarray(void destArray, void srcArray);
proto int ParseStringEx(inout string input, string token);
proto int ParseString(string input, out string tokens[]);
proto native int KillThread(Class owner, string name);
proto volatile void Idle();
proto owned string ThreadFunction(Class owner, string name, int backtrace, out int linenumber);
string String(string s)
{
return s;
}
class Obsolete: Managed
{
string m_Msg;
void Obsolete(string msg = "")
{
m_Msg = msg;
}
}
void PrintString(string s)
{
Print(s);
}
class array<Class T>
{
proto native int Count();
proto native void Clear();
proto void Set(int n, T value);
proto int Find(T value);
proto T Get(int n);
proto int Insert(T value);
proto int InsertAt(T value, int index);
void InsertAll(notnull array<T> from)
{
int nFrom = from.Count();
for ( int i = 0; i < nFrom; ++i )
{
Insert( from.Get(i) );
}
}
proto native void Remove(int index);
proto native void RemoveOrdered(int index);
proto native void Resize(int newSize);
proto native void Reserve(int newSize);
proto native void Swap(notnull array<T> other);
proto native void Sort(bool reverse = false);
proto int Copy(notnull array<T> from);
proto int Init(T init[]);
void RemoveItem(T value)
{
int remove_index = Find(value);
if ( remove_index >= 0 )
{
RemoveOrdered(remove_index);
}
}
void RemoveItemUnOrdered(T value)
{
int remove_index = Find(value);
if ( remove_index >= 0 )
{
Remove(remove_index);
}
}
bool IsValidIndex( int index )
{
return ( index > -1 && index < Count() );
}
/*
T GetChecked( int index )
{
if( IsValidIndex( index ) )
return Get( index );
else
return null;
}
*/
void Debug()
{
Print(string.Format("Array count: %1", Count()));
for (int i = 0; i < Count(); i++)
{
T item = Get(i);
Print(string.Format("[%1] => %2", i, item));
}
}
int GetRandomIndex()
{
if ( Count() > 0 )
{
return Math.RandomInt(0, Count());
}
return -1;
}
T GetRandomElement()
{
return Get(GetRandomIndex());
}
void SwapItems(int item1_index, int item2_index)
{
T item1 = Get(item1_index);
Set(item1_index, Get(item2_index));
Set(item2_index, item1);
}
void InsertArray(array<T> other)
{
int nOther = other.Count();
for (int i = 0; i < nOther; ++i)
{
T item = other.Get(i);
Insert(item);
}
}
void Invert()
{
int left = 0;
int right = Count() - 1;
if (right > 0)
{
while (left < right)
{
T temp = Get(left);
Set(left++, Get(right));
Set(right--, temp);
}
}
}
int MoveIndex(int curr_index, int move_number)
{
int count = Count();
int new_index = curr_index;
if ( move_number > 0 )
{
new_index = curr_index + move_number;
}
if ( move_number < 0 )
{
new_index = curr_index - move_number;
if ( new_index < 0 )
{
if ( new_index <= -count )
{
new_index = (new_index % count);
}
new_index = new_index + count;
}
}
if ( new_index >= count )
{
new_index = (new_index % count);
}
// move_number is 0
return new_index;
}
void ShuffleArray()
{
for (int i = 0; i < Count(); i++)
{
SwapItems(i,GetRandomIndex());
}
}
int DifferentAtPosition(array<T> pOtherArray)
{
if (Count() != pOtherArray.Count())
{
ErrorEx("arrays are not the same size");
return -1;
}
int nOther = pOtherArray.Count();
for (int i = 0; i < nOther; ++i)
{
if (Get(i) != pOtherArray.Get(i))
{
return i;
}
}
return -1;
}
};
//force these to compile so we can link C++ methods to them
typedef array<string> TStringArray;
typedef array<float> TFloatArray;
typedef array<int> TIntArray;
typedef array<bool> TBoolArray;
typedef array<Class> TClassArray;
typedef array<Managed> TManagedArray;
typedef array<ref Managed> TManagedRefArray;
typedef array<vector> TVectorArray;
typedef array<typename> TTypenameArray;
class set<Class T>
{
proto native int Count();
proto native void Clear();
proto int Find(T value);
proto T Get(int n);
proto int Insert(T value);
proto int InsertAt(T value, int index);
proto native void Remove(int index);
proto int Copy(set<T> from);
proto native void Swap(set<T> other);
proto int Init(T init[]);
void InsertSet(set<T> other)
{
int count = other.Count();
for (int i = 0; i < count; i++)
{
T item = other[i];
Insert(item);
}
}
void RemoveItem(T value)
{
int remove_index = Find(value);
if (remove_index >= 0)
{
Remove(remove_index);
}
}
void RemoveItems(set<T> other)
{
int count = other.Count();
for (int i = 0; i < count; i++)
{
T item = other[i];
RemoveItem(item);
}
}
void Debug()
{
Print(string.Format("Set count: %1", Count()));
for (int i = 0; i < Count(); i++)
{
T item = Get(i);
Print(string.Format("[%1] => %2", i, item));
}
}
};
//force these to compile so we can link C++ methods to them
typedef set<string> TStringSet;
typedef set<float> TFloatSet;
typedef set<int> TIntSet;
typedef set<Class> TClassSet;
typedef set<Managed> TManagedSet;
typedef set<ref Managed> TManagedRefSet;
typedef set<typename> TTypenameSet;
typedef int MapIterator;
class map<Class TKey,Class TValue>
{
proto native int Count();
proto native void Clear();
proto TValue Get(TKey key);
proto bool Find(TKey key, out TValue val);
proto TValue GetElement(int index);
proto TKey GetKey(int i);
proto void Set(TKey key, TValue value);
proto void Remove(TKey key);
proto void RemoveElement(int i);
proto bool Contains(TKey key);
proto bool Insert(TKey key, TValue value);
proto int Copy(map<TKey,TValue> from);
array<TKey> GetKeyArray()
{
array<TKey> keys = new array<TKey>();
for (int i = 0; i < Count(); ++i)
keys.Insert(GetKey(i));
return keys;
}
array<TValue> GetValueArray()
{
array<TValue> elements = new array<TValue>();
for (int i = 0; i < Count(); ++i)
elements.Insert(GetElement(i));
return elements;
}
bool ReplaceKey(TKey old_key, TKey new_key)
{
if (Contains(old_key))
{
Set(new_key, Get(old_key));
Remove(old_key);
return true;
}
return false;
}
TKey GetKeyByValue(TValue value)
{
TKey ret;
for (int i = 0; i < Count(); i++)
{
if (GetElement(i) == value)
{
ret = GetKey(i);
break;
}
}
return ret;
}
bool GetKeyByValueChecked(TValue value, out TKey key)
{
for (int i = 0; i < Count(); i++)
{
if (GetElement(i) == value)
{
key = GetKey(i);
return true;
}
}
return false;
}
proto native MapIterator Begin();
proto native MapIterator End();
proto native MapIterator Next(MapIterator it);
proto TKey GetIteratorKey(MapIterator it);
proto TValue GetIteratorElement(MapIterator it);
};
typedef map<int, float> TIntFloatMap;
typedef map<int, int> TIntIntMap;
typedef map<int, string> TIntStringMap;
typedef map<int, Class> TIntClassMap;
typedef map<int, Managed> TIntManagedMap;
typedef map<int, ref Managed> TIntManagedRefMap;
typedef map<int, typename> TIntTypenameMap;
typedef map<int, vector> TIntVectorMap;
typedef map<string, float> TStringFloatMap;
typedef map<string, int> TStringIntMap;
typedef map<string, string> TStringStringMap;
typedef map<string, Class> TStringClassMap;
typedef map<string, Managed> TStringManagedMap;
typedef map<string, ref Managed> TStringManagedRefMap;
typedef map<string, typename> TStringTypenameMap;
typedef map<string, vector> TStringVectorMap;
typedef map<Class, float> TClassFloatMap;
typedef map<Class, int> TClassIntMap;
typedef map<Class, string> TClassStringMap;
typedef map<Class, Class> TClassClassMap;
typedef map<Class, Managed> TClassManagedMap;
typedef map<Class, ref Managed> TClassManagedRefMap;
typedef map<Class, typename> TClassTypenameMap;
typedef map<Class, vector> TClassVectorMap;
typedef map<typename, float> TTypeNameFloatMap;
typedef map<typename, int> TTypeNameIntMap;
typedef map<typename, string> TTypeNameStringMap;
typedef map<typename, Class> TTypeNameClassMap;
typedef map<typename, Managed> TTypeNameManagedMap;
typedef map<typename, ref Managed> TTypeNameManagedRefMap;
typedef map<typename, typename> TTypeNameTypenameMap;
typedef map<typename, vector> TTypeNameVectorMap;
typedef map<Managed, float> TManagedFloatMap;
typedef map<Managed, int> TManagedIntMap;
typedef map<Managed, string> TManagedStringMap;
typedef map<Managed, Class> TManagedClassMap;
typedef map<Managed, Managed> TManagedManagedMap;
typedef map<Managed, ref Managed> TManagedManagedRefMap;
typedef map<Managed, typename> TManagedTypenameMap;
typedef map<Managed, vector> TManagedVectorMap;
typedef map<ref Managed, float> TManagedRefFloatMap;
typedef map<ref Managed, int> TManagedRefIntMap;
typedef map<ref Managed, string> TManagedRefStringMap;
typedef map<ref Managed, Class> TManagedRefClassMap;
typedef map<ref Managed, Managed> TManagedRefManagedMap;
typedef map<ref Managed, ref Managed> TManagedRefManagedRefMap;
typedef map<ref Managed, typename> TManagedRefTypenameMap;
typedef map<ref Managed, vector> TManagedRefVectorMap;