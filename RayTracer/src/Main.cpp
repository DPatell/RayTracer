#include "RayTracer.h"

#include "Camera.h"
#include "Color.h"
#include "HittableList.h"
#include "Sphere.h"

#include <iostream>

Color RayColor(const Ray& r, const Hittable& world, int depth)
{
	HitRecord record;

	// If the Ray Bounce limit is exceeded, no more light is generated
	if (depth <= 0)
	{
		return Color(0, 0, 0);
	}

	if (world.Hit(r, 0.001, infinity, record))
	{
		Point3 target = record.point + record.normal + RandomUnitVector();
		return 0.5 * RayColor(Ray(record.point, target - record.point), world, depth-1);
	}
	Vec3 unitDirection = UnitVector(r.Direction());
	auto t = 0.5*(unitDirection.y() + 1.0);
	return (1.0 - t)*Color(1.0, 1.0, 1.0) + t * Color(0.5, 0.7, 1.0);
}

int main() {

    // Image Resolution
	const auto aspectRatio = 16.0 / 9.0;
	const int imageWidth = 400;
	const int imageHeight = static_cast<int>(imageWidth / aspectRatio);
	const int samplePerPixel = 100;
	const int maxDepth = 50;

	// World
	HittableList world;
	world.add(std::make_shared<Sphere>(Point3(0, 0, -1), 0.5));
	world.add(std::make_shared<Sphere>(Point3(0, -100.5, -1), 100));

	// Camera
	Camera camera;

    // Render Image
    std::cout << "P3\n" << imageWidth << ' ' << imageHeight << "\n255\n";

    for (int j = imageHeight-1; j >= 0; --j) 
    {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < imageWidth; ++i) 
        {
			Color pixelColor(0, 0, 0);
			for (int s = 0; s < samplePerPixel; ++s)
			{
				auto u = (i + RandomDouble()) / (imageWidth - 1);
				auto v = (j + RandomDouble()) / (imageHeight - 1);
				Ray r = camera.GetRay(u, v);
				pixelColor += RayColor(r, world, maxDepth);
			}

			WriteColor(std::cout, pixelColor, samplePerPixel);
        }
    }

    std::cerr << "\nDone!\n";
}