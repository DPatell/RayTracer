#pragma once

#include "Vec3.h"

class Ray
{
public:
	Ray()
	{

	}

	Ray(const Point3& origin, const Vec3& direction) : orig(origin), dir(direction)
	{

	}

	Point3 Origin() const { return orig; }
	Vec3 Direction() const { return dir; }

	Point3 At(double t) const 
	{
		return orig + t*dir;
	}

public:
	Point3 orig;
	Vec3 dir;
};