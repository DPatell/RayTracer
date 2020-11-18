#include "RayTracer.h"

#include "Camera.h"
#include "Color.h"
#include "HittableList.h"
#include "Sphere.h"
#include "Material.h"

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
		Ray scattered;
		Color attenuation;
		if (record.materialPtr->Scatter(r, record, attenuation, scattered))
		{
			return attenuation * RayColor(scattered, world, depth - 1);
		}
		return Color(0, 0, 0);
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
	auto materialGround = std::make_shared<Lambertian>(Color(0.8, 0.8, 0.0));
	auto materialCenter = std::make_shared<Lambertian>(Color(0.7, 0.3, 0.3));
	auto materialLeft = std::make_shared<Metal>(Color(0.8, 0.8, 0.8), 0.0);
	auto materialRight = std::make_shared<Metal>(Color(0.8, 0.6, 0.2), 1.0);

	world.add(std::make_shared<Sphere>(Point3(0.0, -100.5, -1.0), 100.0, materialGround));
	world.add(std::make_shared<Sphere>(Point3(0.0, 0.0, -1.0), 0.5, materialCenter));
	world.add(std::make_shared<Sphere>(Point3(-1.0, 0.0, -1.0), 0.5, materialLeft));
	world.add(std::make_shared<Sphere>(Point3(1.0, 0.0, -1.0), 0.5, materialRight));

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