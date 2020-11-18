#pragma once

#include "RayTracer.h"

struct HitRecord;

class Material
{
public:
	virtual bool Scatter(const Ray& rayIn, const HitRecord& record, Color& attenuation, Ray& scattered) const = 0;
};

class Lambertian : public Material
{
public:
	Lambertian(const Color& a) : albedo(a)
	{

	}

	virtual bool Scatter(const Ray& rayIn, const HitRecord& record, Color& attenuation, Ray& scattered) const override
	{
		auto scatterDirection = record.normal + RandomUnitVector();
		
		// Catch degenrate scatter direction
		if (scatterDirection.NearZero())
		{
			scatterDirection = record.normal;
		}

		scattered = Ray(record.point, scatterDirection);
		attenuation = albedo;
		return true;
	}

public:
	Color albedo;
};

class Metal : public Material
{
public:
	Metal(const Color& a, double f) : albedo(a), fuzziness(f < 1 ? f : 1)
	{

	}

	virtual bool Scatter(const Ray& rayIn, const HitRecord& record, Color& attenuation, Ray& scattered) const override
	{
		Vec3 reflected = Reflect(UnitVector(rayIn.Direction()), record.normal);
		scattered = Ray(record.point, reflected + fuzziness*RandomInUnitSphere());
		attenuation = albedo;
		return (Dot(scattered.Direction(), record.normal) > 0);
	}

public:
	Color albedo;
	double fuzziness;
};