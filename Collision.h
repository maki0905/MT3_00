#pragma once
#include "Sphere.h"
#include "Plane.h"

bool IsCollision(const Sphere& sphere1, const Sphere& sphere2);

bool IsCollision(const Sphere& sphere, const Plane& plane);