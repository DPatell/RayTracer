#pragma once

#include "RayTracer.h"

#include "Hittable.h"

class Sphere : public Hittable
{
public:
	Sphere()
	{

	}

	Sphere(Point3 cen, double r) : center(cen), radius(r)
	{

	}

	virtual bool Hit(const Ray& r, double tMin, double tMax, HitRecord& rec) const override;

public:
	Point3 center;
	double radius;
};

bool Sphere::Hit(const Ray& r, double tMin, double tMax, HitRecord& record) const
{
	Vec3 oc = r.Origin() - center;
	auto a = r.Direction().LengthSquared();
	auto halfB = Dot(oc, r.Direction());
	auto c = oc.LengthSquared() - radius * radius;

	auto discriminant = halfB * halfB - a * c;
	if (discriminant < 0) return false;
	auto sqrtd = sqrt(discriminant);

	// Find the nearest root that lies in the acceptable range
	auto root = (-halfB - sqrtd) / a;
	if (root < tMin || tMax < root)
	{
		root = (-halfB + sqrtd) / a;
		if (root < tMin || tMax < root)
		{
			return false;
		}
	}

	record.t = root;
	record.point = r.At(record.t);
	Vec3 outwardNormal = (record.point - center) / radius;
	record.setFaceNormal(r, outwardNormal);

	return true;
}