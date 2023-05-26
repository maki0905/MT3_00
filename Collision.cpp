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
