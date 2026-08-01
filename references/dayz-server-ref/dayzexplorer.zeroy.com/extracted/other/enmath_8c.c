// ======================================================================
// File: enmath_8c_source.html
// Category: other
// ======================================================================

class Math
{
private void Math() {}
private void ~Math() {}
static const float EULER = 2.7182818284590452353;
static const float PI = 3.14159265358979;
static const float PI2 = 6.28318530717958;
static const float PI_HALF = 1.570796326794;
static const float RAD2DEG = 57.2957795130823208768;
static const float DEG2RAD = 0.01745329251994329577;
proto static int GetNumberOfSetBits(int i);
proto static int GetNthBitSet(int value, int n);
proto static int RandomInt(int min, int max);
static int RandomIntInclusive(int min, int max)
{
return Math.RandomInt(min, max+1);
}
static bool RandomBool()
{
return RandomIntInclusive(0,1);
}
proto static float RandomFloat(float min, float max);
static float RandomFloatInclusive(float min, float max)
{
int max_range = Math.Pow(2, 30); //max range
int random_int = Math.RandomInt(0, max_range);
float rand_float = (float)random_int / (float)max_range;
float range = max - min;
return min + (rand_float * range); //rand float
}
static float RandomFloat01()
{
return RandomFloatInclusive(0, 1);
}
proto static int Randomize(int seed);
proto static float NormalizeAngle(float ang);
proto static float DiffAngle(float angle1, float angle2);
proto static float Pow(float v, float power);
proto static float ModFloat(float x, float y);
proto static float RemainderFloat(float x, float y);
proto static float AbsFloat(float f);
proto static int AbsInt(int i);
proto static float SignFloat(float f);
proto static int SignInt(int i);
proto static float SqrFloat(float f);
proto static int SqrInt(int i);
proto static float Sqrt(float val);
proto static float Log2(float x);
proto static float Sin(float angle);
proto static float Cos(float angle);
proto static float Tan(float angle);
proto static float Asin(float s);
proto static float Acos(float c);
proto static float Atan(float x);
proto static float Atan2(float y, float x);
proto static float Round(float f);
proto static float Floor(float f);
proto static float Ceil(float f);
proto static float WrapFloat(float f, float min, float max);
proto static float WrapFloatInclusive(float f, float min, float max);
proto static float WrapFloat0X(float f, float max);
proto static float WrapFloat0XInclusive(float f, float max);
proto static int WrapInt(int i, int min, int max);
proto static int WrapInt0X(int i, int max);
proto static float Clamp(float value, float min, float max);
proto static float Min(float x, float y);
proto static float Max(float x, float y);
proto static bool IsInRange(float v, float min, float max);
proto static bool IsInRangeInt(int v, int min, int max);
proto static float Lerp(float a, float b, float time);
proto static float InverseLerp(float a, float b, float value);
proto static float AreaOfRightTriangle(float s, float a);
proto static float HypotenuseOfRightTriangle(float s, float a);
proto static bool IsPointInCircle(vector c, float r, vector p);
proto static bool IsPointInRectangle(vector mi, vector ma, vector p);
//--------------------------------------------------------------------------
//-------------------------------- filters ---------------------------------
//--------------------------------------------------------------------------
proto static float SmoothCD(float val, float target, inout float velocity[], float smoothTime, float maxVelocity, float dt);
static float SmoothCDPI2PI(float val, float target, inout float velocity[], float smoothTime, float maxVelocity, float dt)
{
float diff = target - val;
if (diff < -Math.PI)
{
target += Math.PI2;
}
else if (diff > Math.PI)
{
target -= Math.PI2;
}
float retVal = SmoothCD(val, target, velocity, smoothTime, maxVelocity, dt);
while (retVal > Math.PI)
{
retVal -= Math.PI2;
}
while (retVal < -Math.PI)
{
retVal += Math.PI2;
}
return retVal;
}
static float Poisson(float mean, int occurences)
{
return Pow(mean, occurences) * Pow(EULER,-mean) / Factorial(occurences);
}
static int Factorial(int val)
{
if (val > 12)
{
ErrorEx("Values above &#39;12&#39; cause int overflow! Returning &#39;1&#39;",ErrorExSeverity.INFO);
return 1;
}
int res = 1;
while (val > 1)
{
res *= val--;
}
return res;
}
static float Remap(float inputMin, float inputMax, float outputMin, float outputMax, float inputValue, bool clampedOutput = true)
{
float tempValue = Math.InverseLerp(inputMin, inputMax, inputValue);
float remapped = Math.Lerp(outputMin, outputMax, tempValue);
if (clampedOutput)
return Math.Clamp(remapped, outputMin, outputMax);
return remapped;
}
static vector CenterOfRectangle(vector min, vector max)
{
float x = (min[0] + max[0]) * 0.5;
float z = (min[2] + max[2]) * 0.5;
return Vector(x, 0.0, z);
}
static bool VectorIsEqual(vector v1, vector v2, float tolerance)
{
return (Math.AbsFloat(v1[0] - v2[0]) <= tolerance && Math.AbsFloat(v1[1] - v2[1]) <= tolerance && Math.AbsFloat(v1[2] - v2[2]) <= tolerance);
}
}