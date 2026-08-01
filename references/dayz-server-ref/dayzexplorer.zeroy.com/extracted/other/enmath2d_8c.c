// ======================================================================
// File: enmath2d_8c_source.html
// Category: other
// ======================================================================

//-----------------------------------------------------------------------------
enum WindingOrder
{
CounterClockwise, // 0
Clockwise, // 1
Invalid // 2
}
class Math2D
{
private void Math2D();
private void ~Math2D();
static proto bool IsPointInPolygonXZ(notnull array<vector> polygon, vector point);
static proto bool IsPointInTriangleXZ(vector p1, vector p2, vector p3, vector point);
static proto bool IsPointInPolygon(notnull array<float> polygon, float x, float y);
static proto WindingOrder TriangleWindingXZ(vector a, vector b, vector c);
}