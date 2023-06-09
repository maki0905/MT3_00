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
	if (distance < 0) {
		distance *= -1;
	}
	if (distance <= sphere.radius) {
		return true;
	}
	return false;
}
