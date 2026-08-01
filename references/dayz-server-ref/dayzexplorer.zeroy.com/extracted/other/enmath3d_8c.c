// ======================================================================
// File: enmath3d_8c_source.html
// Category: other
// ======================================================================

proto native vector Vector(float x, float y, float z);
enum ECurveType
{
CatmullRom,
NaturalCubic,
UniformCubic
};
class Math3D
{
private void Math3D() {}
private void ~Math3D() {}
//-----------------------------------------------------------------
proto static vector ClipLine(vector start, vector end, vector norm, float d);
proto static float IntersectRaySphere(vector raybase, vector raycos, vector center, float radius);
proto static float IntersectRayBox(vector start, vector end, vector mins, vector maxs);
proto static bool IntersectSphereBox(vector origin, float radius, vector mins, vector maxs);
proto static bool IntersectSphereCone(vector origin, float radius, vector conepos, vector axis, float angle);
proto static bool IntersectWholeSphereCone(vector origin, float radius, vector conepos, vector axis, float angle);
proto static bool IntersectCylinderOBB(vector mins, vector maxs, vector obbMat[4], vector cylMat[4], float cylinderRadius, float cylinderHeight);
proto static bool IntersectRayCylinder(vector rayStart, vector rayEnd, vector center, float radius, float height);
proto static int IntersectRayPlane(vector rayStart, vector rayEnd, vector planeNormal, float planeDist, out vector intersection);
proto static void YawPitchRollMatrix(vector ang, out vector mat[3]);
proto static void DirectionAndUpMatrix(vector dir, vector up, out vector mat[4]);
proto static void MatrixMultiply4(vector mat0[4], vector mat1[4], out vector res[4]);
proto static void MatrixMultiply3(vector mat0[3], vector mat1[3], out vector res[3]);
proto static void MatrixInvMultiply4(vector mat0[4], vector mat1[4], out vector res[4]);
proto static void MatrixInvMultiply3(vector mat0[3], vector mat1[3], out vector res[3]);
proto static void MatrixInverse4(vector mat[4]);
proto static void MatrixInverse3(vector mat[3]);
proto static void MatrixOrthogonalize4(vector mat[4]);
proto static void MatrixOrthogonalize3(vector mat[3]);
static void MatrixIdentity4(out vector mat[4])
{
mat[0] = "1 0 0";
mat[1] = "0 1 0";
mat[2] = "0 0 1";
mat[3] = vector.Zero;
}
static void MatrixIdentity3(out vector mat[3])
{
mat[0] = "1 0 0";
mat[1] = "0 1 0";
mat[2] = "0 0 1";
}
static void ScaleMatrix(float scale, out vector mat[3])
{
vector v0, v1, v2;
v0[0] = scale;
v1[1] = scale;
v2[2] = scale;
mat[0] = v0;
mat[1] = v1;
mat[2] = v2;
}
static void QuatIdentity(out float q[4])
{
q[0] = 0;
q[1] = 0;
q[2] = 0;
q[3] = 1;
}
static void QuatCopy(float s[4], out float d[4])
{
d[0] = s[0];
d[1] = s[1];
d[2] = s[2];
d[3] = s[3];
}
proto static void MatrixToQuat(vector mat[3], out float d[4]);
proto static void QuatToMatrix(float q[4], out vector mat[3]);
proto static vector MatrixToAngles(vector mat[3]);
proto static void QuatLerp(out float qout[4], float q1[4], float q2[4], float frac);
proto static void QuatMultiply(out float qout[4], float q1[4], float q2[4]);
proto static vector QuatToAngles(float q[4]);
proto static int CheckBoundBox(vector mins1, vector maxs1, vector mins2, vector maxs2);
static vector GetRandomDir()
{
float x = Math.RandomFloatInclusive(-1, 1);
float y = Math.RandomFloatInclusive(-1, 1);
float z = Math.RandomFloatInclusive(-1, 1);
return Vector(x, y, z).Normalized();
}
proto static native vector Curve(ECurveType type, float param, notnull array<vector> points);
proto static vector NearestPoint(vector beg, vector end, vector pos);
proto static float AngleFromPosition(vector origin, vector originDir, vector target);
proto static void ConePoints(vector origin, float length, float halfAngle, float angleOffset, out vector leftPoint, out vector rightPoint);
proto static void BlendCartesian(vector samplePosition, notnull array<vector> inPositions, notnull array<float> outWeights);
static vector BoxCenter(vector a, vector b)
{
return (a + b) * 0.5;
}
static vector BoxSize(vector a, vector b)
{
return Vector(Math.AbsFloat(a[0] - b[0]), Math.AbsFloat(a[1] - b[1]), Math.AbsFloat(a[2] - b[2]));
}
};