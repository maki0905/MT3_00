#pragma once
#include "Sphere.h"
#include "Plane.h"
#include "Line.h"
#include "Triangle.h"

bool IsCollision(const Sphere& sphere1, const Sphere& sphere2);

bool IsCollision(const Sphere& sphere, const Plane& plane);

bool IsCollision(const Line& line, const Plane& plane);

bool IsCollision(const Ray& ray, const Plane& plane);

bool IsCollision(const Segment& segment, const Plane& plane);

bool IsCollision(const Triangle& triangle, const Segment& segment);