#include "Collision.h"
#include "MathFunction.h"
#include <iostream>
#include <algorithm>
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

bool IsCollision(const Triangle& triangle, const Segment& segment)
{
	Vector3 v1 = Subtract(triangle.vertices[1], triangle.vertices[0]);
	Vector3 v2 = Subtract(triangle.vertices[2], triangle.vertices[1]);
	Vector3 normal = Normalize(Cross(v1, v2));
	/*float distance = (normal.x * triangle.vertices[0].x) + (normal.y * triangle.vertices[0].y) + (normal.z * triangle.vertices[0].z);*/
	float distance = Dot(triangle.vertices[0], normal);
	float dot = Dot(normal, segment.diff);
	// 垂直=平行であるので、衝突しているはずがない
	if (dot == 0.0f) {
		return false;
	}

	float t = (distance - Dot(segment.origin, normal)) / dot;

	if (t < 0.0f || t > 1.0f) {
		return false;
	}

	Vector3 p = Add(segment.origin, Multiply(t, segment.diff));

	Vector3 v01 = Subtract(triangle.vertices[1], triangle.vertices[0]);
	Vector3 v1p = Subtract(p, triangle.vertices[1]);
	Vector3 v12 = Subtract(triangle.vertices[2], triangle.vertices[1]);
	Vector3 v2p = Subtract(p, triangle.vertices[2]);
	Vector3 v20 = Subtract(triangle.vertices[0], triangle.vertices[2]);
	Vector3 v0p = Subtract(p, triangle.vertices[0]);
	
	// 各辺を結んだベクトルと、頂点と衝突点pを結んだベクトルのクロス積を取る
	Vector3 cross01 = Cross(v01, v1p);
	Vector3 cross12 = Cross(v12, v2p);
	Vector3 cross20 = Cross(v20, v0p);

	// すべての小三角形のクロス積と法線が同じ方向を向いていたら衝突
	if (Dot(cross01, normal) >= 0.0f && Dot(cross12, normal) >= 0.0f && Dot(cross20, normal) >= 0.0f) {
		return true;
	}

	return false;


}

bool IsCollision(const AABB& aabb1, const AABB& aabb2)
{
	if ((aabb1.min.x <= aabb2.max.x && aabb1.max.x >= aabb2.min.x) && // x軸
		(aabb1.min.y <= aabb2.max.y && aabb1.max.y >= aabb2.min.y) && // y軸
		(aabb1.min.z <= aabb2.max.z && aabb1.max.z >= aabb2.min.z)) { // z軸
		return true;
	}

	return false;
}

bool IsCollision(const Sphere& sphere, const AABB& aabb)
{
	// 最近接点を求める
	Vector3 closestPoint{ std::clamp(sphere.center.x, aabb.min.x, aabb.max.x), std::clamp(sphere.center.y, aabb.min.y, aabb.max.y), std::clamp(sphere.center.z, aabb.min.z, aabb.max.z) };
	// 最近接点と球の中心との距離を求める
	float distance = Length(Subtract(closestPoint , sphere.center));
	// 距離が半径よりも小さければ衝突
	if (distance <= sphere.radius) {
		return true;
	}
	return false;
}
