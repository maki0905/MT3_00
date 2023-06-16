#include "Collision.h"
#include "MathFunction.h"

bool IsCollision(const Sphere& sphere1, const Sphere& sphere2)
{
	float distance;
	distance = Length(Subtract(sphere2.center, sphere1.center));
	if (distance <= sphere1.radius + sphere2.radius) {
		return true;
	}
	return false;
}

bool IsCollision(const Sphere& sphere, const Plane& plane)
{
	float distance = Dot(plane.normal, sphere.center) - plane.distance;
	// 絶対値にするためdistanceが0以下だったらマイナス1を掛ける
	if (distance < 0) {
		distance *= -1;
	}
	if (distance <= sphere.radius) {
		return true;
	}
	return false;
}

bool IsCollision(const Line& line, const Plane& plane)
{
	// 垂直判定を行うために、法線と線の内積を求める
	float dot = Dot(plane.normal, line.diff);

	// 垂直=平行であるので、衝突しているはずがない
	if (dot == 0.0f) {
		return false;
	}

	float t = (plane.distance - Dot(line.origin, plane.normal)) / dot;


	if (t >= -1.0f && t <= 1) {
		return true;
	}

	return false;
}

bool IsCollision(const Ray& ray, const Plane& plane)
{
	// 垂直判定を行うために、法線と線の内積を求める
	float dot = Dot(plane.normal, ray.diff);

	// 垂直=平行であるので、衝突しているはずがない
	if (dot == 0.0f) {
		return false;
	}

	float t = (plane.distance - Dot(ray.origin, plane.normal)) / dot;


	if (t > 0.0f) {
		return true;
	}

	return false;
}


bool IsCollision(const Segment& segment, const Plane& plane)
{
	// 垂直判定を行うために、法線と線の内積を求める
	float dot = Dot(plane.normal, segment.diff);

	// 垂直=平行であるので、衝突しているはずがない
	if (dot == 0.0f) {
		return false;
	}
	
	float t = (plane.distance - Dot(segment.origin, plane.normal)) / dot;
	

	if (t >= 0.0f && t <= 1.0f) {
		return true;
	}

	return false;
}
