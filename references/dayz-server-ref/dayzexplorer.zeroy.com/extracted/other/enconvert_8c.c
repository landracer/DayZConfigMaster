// ======================================================================
// File: enconvert_8c_source.html
// Category: other
// ======================================================================

class bool
{
string ToString()
{
if (value) return "true";
else return "false";
}
};
class func
{
private proto void SetInstance(Class inst);
};
enum EBool
{
NO = 0,
YES = 1
}
class int
{
protected const int ZERO_PAD_SIZE = 8;
protected static string m_ZeroPad[ZERO_PAD_SIZE] = {"", "0", "00", "000", "0000", "00000", "000000", "0000000"};
const int MAX = 2147483647;
const int MIN = -2147483648;
proto string ToString();
proto string AsciiToString();
string ToStringLen(int len)
{
string str = value.ToString();
int l = len - str.Length();
if (l > 0 && l < ZERO_PAD_SIZE )
return m_ZeroPad[l] + str;
return str;
}
proto string ToHex();
bool InRange( int min, int max, bool inclusive_min = true, bool inclusive_max = true )
{
if( ( !inclusive_min && value <= min ) || value < min )
return false;
if( ( !inclusive_max && value >= max ) || value > max )
return false;
return true;
}
};
class float
{
const float MIN = FLT_MIN;
const float MAX = FLT_MAX;
const float LOWEST = -FLT_MAX;
proto string ToString(bool simple = true);
};
class vector
{
static const vector Up = "0 1 0";
static const vector Aside = "1 0 0";
static const vector Forward = "0 0 1";
static const vector Zero = "0 0 0";
proto string ToString(bool beautify = true);
proto float Normalize();
proto vector Normalized();
proto native float Length();
proto native float LengthSq();
proto static native float Distance(vector v1, vector v2);
proto static native float DistanceSq(vector v1, vector v2);
vector Perpend()
{
return value * vector.Up;
}
static vector Direction(vector p1, vector p2)
{
vector dir_vec;
dir_vec[0] = p2[0] - p1[0];
dir_vec[1] = p2[1] - p1[1];
dir_vec[2] = p2[2] - p1[2];
return dir_vec;
}
static vector RandomDir()
{
return Vector(Math.RandomFloatInclusive(-1,1),Math.RandomFloatInclusive(-1,1),Math.RandomFloatInclusive(-1,1)).Normalized();
}
static vector RandomDir2D()
{
return Vector(Math.RandomFloatInclusive(-1,1),0,Math.RandomFloatInclusive(-1,1)).Normalized();
}
static float Dot(vector v1, vector v2)
{
return ((v1[0] * v2[0]) + (v1[1] * v2[1]) + (v1[2] * v2[2]));
}
vector GetRelAngles()
{
for(int i = 0; i < 3; i++) {
if(value[i] > 180)
value[i] = value[i] - 360;
if(value[i] < -180)
value[i] = value[i] + 360;
}
return value;
}
proto float VectorToYaw();
proto native static vector YawToVector(float yaw);
proto vector VectorToAngles();
proto vector AnglesToVector();
proto void RotationMatrixFromAngles(out vector mat[3]);
proto vector Multiply4(vector mat[4]);
proto vector Multiply3(vector mat[3]);
proto vector InvMultiply4(vector mat[4]);
proto vector InvMultiply3(vector mat[3]);
proto static native vector Lerp(vector v1, vector v2, float t);
static vector RotateAroundZeroDeg(vector vec, vector axis, float angle)
{
return (vec * Math.Cos(angle * Math.DEG2RAD)) + ((axis * vec) * Math.Sin(angle * Math.DEG2RAD)) + (axis * vector.Dot(axis, vec)) * (1 - Math.Cos(angle * Math.DEG2RAD));
}
static vector RotateAroundZeroRad(vector vec, vector axis, float angle)
{
return (vec * Math.Cos(angle)) + ((axis * vec) * Math.Sin(angle)) + (axis * vector.Dot(axis, vec)) * (1 - Math.Cos(angle));
}
static vector RotateAroundZero(vector pos, vector axis, float cosAngle, float sinAngle)
{
return (pos * cosAngle) + ((axis * pos) * sinAngle) + (axis * vector.Dot(axis, pos)) * (1 - cosAngle);
}
static vector RotateAroundPoint(vector point, vector pos, vector axis, float cosAngle, float sinAngle)
{
vector offsetPos = pos - point;
return RotateAroundZero(offsetPos, axis, cosAngle, sinAngle) + point;
}
static vector ArrayToVec(float arr[])
{
return Vector(arr[0], arr[1], arr[2]);
}
};
class typename
{
proto volatile Class Spawn();
proto owned string GetModule();
proto native owned string ToString();
proto native bool IsInherited(typename baseType);
proto native int GetVariableCount();
proto native owned string GetVariableName(int vIdx);
proto native typename GetVariableType(int vIdx);
proto bool GetVariableValue(Class var, int vIdx, out void val);
static string EnumToString(typename e, int enumValue)
{
int cnt = e.GetVariableCount();
int val;
for (int i = 0; i < cnt; i++)
{
if (e.GetVariableType(i) == int && e.GetVariableValue(null, i, val) && val == enumValue)
{
return e.GetVariableName(i);
}
}
return "unknown";
}
static int StringToEnum(typename e, string enumName)
{
int count = e.GetVariableCount();
int value;
for (int i = 0; i < count; i++)
{
if (e.GetVariableType(i) == int && e.GetVariableValue(null, i, value) && e.GetVariableName(i) == enumName)
{
return value;
}
}
return -1;
}
};
class EnumTools
{
private void EnumTools();
private void ~EnumTools();
static string EnumToString(typename e, int enumValue)
{
return typename.EnumToString(e, enumValue);
}
static int StringToEnum(typename e, string enumName)
{
return typename.StringToEnum(e, enumName);
}
static int GetEnumSize(typename e)
{
return e.GetVariableCount();
}
static int GetEnumValue(typename e, int idx)
{
int value;
e.GetVariableValue(null, idx, value);
return value;
}
static int GetLastEnumValue(typename e)
{
int lastValue;
e.GetVariableValue(null, e.GetVariableCount() - 1, lastValue);
return lastValue;
}
}