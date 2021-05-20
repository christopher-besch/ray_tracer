#pragma once

#include "ray_tracer.h"

#include "hittable_list.h"
#include "material.h"
#include "sphere.h"

// memory leak; don't use!
#if 0
inline void random_scene(HittableList& world, RandomGen& random_gen)
{
    Material* ground_material = new Lambertian(Color(0.5, 0.5, 0.5));
    world.add(new Sphere(Point3(0, -1000, 0), 1000, ground_material));

    for (int a = -11; a < 11; ++a)
    {
        for (int b = -11; b < 11; ++b)
        {
            double choose_material = random_gen.next_double();
            Point3 center(a + 0.9 * random_gen.next_double(), 0.2, b + 0.9 + random_gen.next_double());

            if ((center - Point3(4.0, 0.2, 0.0)).magnitude() > 0.9)
            {
                Material* sphere_material;

                if (choose_material < 0.8)
                {
                    // diffuse
                    Color albedo    = random_gen.next_double() * random_vector(random_gen);
                    sphere_material = new Lambertian(albedo);
                    world.add(new Sphere(center, 0.2, sphere_material));
                }
                else if (choose_material < 0.95)
                {
                    // metal
                    Color  albedo    = random_vector(0.5, 1.0, random_gen);
                    double fuzziness = random_gen.next_double(0.0, 0.5);
                    sphere_material  = new Metal(albedo, fuzziness);
                    world.add(new Sphere(center, 0.2, sphere_material));
                }
                else
                {
                    // glass
                    sphere_material = new Dielectric(1.5);
                    world.add(new Sphere(center, 0.2, sphere_material));
                }
            }
        }
    }

    Material* material1 = new Dielectric(1.5);
    world.add(new Sphere(Point3(0, 1, 0), 1.0, material1));

    Material* material2 = new Lambertian(Color(0.4, 0.2, 0.1));
    world.add(new Sphere(Point3(-4, 1, 0), 1.0, material2));

    Material* material3 = new Metal(Color(0.7, 0.6, 0.5), 0.0);
    world.add(new Sphere(Point3(4, 1, 0), 1.0, material3));
}
#endif
