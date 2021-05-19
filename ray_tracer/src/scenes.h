#pragma once

#include "ray_tracer.h"

#include "hittable_list.h"
#include "material.h"
#include "sphere.h"

inline HittableList random_scene()
{
    HittableList world;

    std::shared_ptr<Material> ground_material = std::make_shared<Lambertian>(Color(0.5, 0.5, 0.5));
    world.add(std::make_shared<Sphere>(Point3(0, -1000, 0), 1000, ground_material));

    for (int a = -11; a < 11; ++a)
    {
        for (int b = -11; b < 11; ++b)
        {
            double choose_material = random_double();
            Point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 + random_double());

            if ((center - Point3(4.0, 0.2, 0.0)).magnitude() > 0.9)
            {
                std::shared_ptr<Material> sphere_material;

                if (choose_material < 0.8)
                {
                    // diffuse
                    Color albedo    = random_vector() * random_vector();
                    sphere_material = std::make_shared<Lambertian>(albedo);
                    world.add(std::make_shared<Sphere>(center, 0.2, sphere_material));
                }
                else if (choose_material < 0.95)
                {
                    // metal
                    Color  albedo    = random_vector(0.5, 1.0);
                    double fuzziness = random_double(0.0, 0.5);
                    sphere_material  = std::make_shared<Metal>(albedo, fuzziness);
                    world.add(std::make_shared<Sphere>(center, 0.2, sphere_material));
                }
                else
                {
                    // glass
                    sphere_material = std::make_shared<Dielectric>(1.5);
                    world.add(std::make_shared<Sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    std::shared_ptr<Material> material1 = std::make_shared<Dielectric>(1.5);
    world.add(std::make_shared<Sphere>(Point3(0, 1, 0), 1.0, material1));

    std::shared_ptr<Material> material2 = std::make_shared<Lambertian>(Color(0.4, 0.2, 0.1));
    world.add(std::make_shared<Sphere>(Point3(-4, 1, 0), 1.0, material2));

    std::shared_ptr<Material> material3 = std::make_shared<Metal>(Color(0.7, 0.6, 0.5), 0.0);
    world.add(std::make_shared<Sphere>(Point3(4, 1, 0), 1.0, material3));

    return world;
}
