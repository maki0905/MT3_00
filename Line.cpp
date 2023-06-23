#include "Line.h"
#include "MathFunction.h"
#include "Novice.h"
void DrawSegment(const Segment& segment, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color)
{
	Vector3 screenPoint[2];
	Vector3 ndcVertex = Transform(segment.origin, viewProjectionMatrix);
	screenPoint[0] = Transform(ndcVertex, viewportMatrix);
	ndcVertex = Transform(Add(segment.origin, segment.diff), viewProjectionMatrix);
	screenPoint[1] = Transform(ndcVertex, viewportMatrix);
	Novice::DrawLine(int(screenPoint[0].x), int(screenPoint[0].y), int(screenPoint[1].x), int(screenPoint[1].y), color);

}
