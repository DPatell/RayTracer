#pragma once

#include "RayTracer.h"

#include "Hittable.h"

#include <memory>
#include <vector>

class HittableList : public Hittable
{
public:
	HittableList()
	{

	}

	HittableList(std::shared_ptr<Hittable> object)
	{
		add(object);
	}

	void clear()
	{
		objects.clear();
	}

	void add(std::shared_ptr<Hittable> object)
	{
		objects.push_back(object);
	}

	virtual bool Hit(const Ray& r, double tMin, double tMax, HitRecord& record) const override;

public:
	std::vector<std::shared_ptr<Hittable>> objects;
};

bool HittableList::Hit(const Ray& r, double tMin, double tMax, HitRecord& record) const
{
	HitRecord tempRecord;
	bool hitAnything = false;
	auto closestSoFar = tMax;

	for (const auto& object : objects)
	{
		if (object->Hit(r, tMin, closestSoFar, tempRecord))
		{
			hitAnything = true;
			closestSoFar = tempRecord.t;
			record = tempRecord;
		}
	}

	return hitAnything;
}