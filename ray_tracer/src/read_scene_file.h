#include "hittable_list.h"
#include "sphere.h"
#include "utils.h"

#include <fstream>
#include <istream>
#include <memory>
#include <sstream>
#include <vector>

#include "camera.h"
#include "material.h"
#include "ray.h"
#include "ray_tracer.h"
#include "vec3.h"

struct Scene
{
    double aspect_ratio;
    int    image_height;
    int    image_width;
    int    samples_per_pixel;
    int    max_depth;

    double vfov;
    Point3 lookfrom;
    Point3 lookat;
    Vec3   vup;
    double focus_dist;
    double aperture;

    std::vector<Material*> materials;
    HittableList           world;

    Scene() {}

    ~Scene()
    {
        for (Material* material : materials)
            delete material;
    }

    void calc_width()
    {
        image_width = static_cast<int>(image_height * aspect_ratio);
    }

    Camera get_cam()
    {
        return Camera(lookfrom, lookat, vup, vfov, aspect_ratio, aperture, focus_dist);
    }
};

// todo: horrible way of treating the user
inline int get_next_int(std::istream& input)
{
    std::string input_buffer;
    std::getline(input, input_buffer, ' ');
    return std::stoi(input_buffer);
}
inline double get_next_double(std::istream& input)
{
    std::string input_buffer;
    std::getline(input, input_buffer, ' ');
    return std::stod(input_buffer);
}
inline Vec3 get_next_vec3(std::istream& input)
{
    // prevent undefined
    double x = get_next_double(input);
    double y = get_next_double(input);
    double z = get_next_double(input);
    return Vec3(x, y, z);
}

// skip to next line after read
inline int get_int_from_line(std::istream& input)
{
    std::string input_buffer;
    std::getline(input, input_buffer, '=');
    std::getline(input, input_buffer);
    return std::stoi(input_buffer);
}
inline double get_double_from_line(std::istream& input)
{
    std::string input_buffer;
    std::getline(input, input_buffer, '=');
    std::getline(input, input_buffer);
    return std::stod(input_buffer);
}
inline Vec3 get_vec3_from_line(std::istream& input)
{
    std::string input_buffer;
    std::getline(input, input_buffer, '=');
    std::getline(input, input_buffer);
    std::stringstream input_buffer_ss(input_buffer);
    Vec3              vec3 = get_next_vec3(input_buffer_ss);
    return vec3;
}

inline void read_scene_file(std::string file_path, Scene& scene)
{
    try
    {
        std::fstream file;
        file.open(file_path);
        if (!file)
            raise_error("Can't open scene file " << file_path << "!");

        // read metadata
        scene.aspect_ratio      = get_double_from_line(file);
        scene.image_height      = get_int_from_line(file);
        scene.samples_per_pixel = get_int_from_line(file);
        scene.max_depth         = get_int_from_line(file);
        scene.vfov              = get_double_from_line(file);
        scene.lookfrom          = get_vec3_from_line(file);
        scene.lookat            = get_vec3_from_line(file);
        scene.vup               = get_vec3_from_line(file);
        scene.focus_dist        = get_double_from_line(file);
        scene.aperture          = get_double_from_line(file);
        scene.calc_width();

        std::string input_buffer;
        // skip comment
        std::getline(file, input_buffer);
        // materials
        while (std::getline(file, input_buffer))
        {
            std::stringstream input_buffer_ss(input_buffer);
            // split at spaces
            std::getline(input_buffer_ss, input_buffer, ' ');
            char type = input_buffer[0];

            Color  albedo;
            double fuzziness;
            double ior;
            switch (type)
            {
            case 'l':
                albedo = get_next_vec3(input_buffer_ss);
                scene.materials.push_back(new Lambertian(albedo));
                break;
            case 'd':
                ior = get_next_double(input_buffer_ss);
                scene.materials.push_back(new Dielectric(ior));
                break;
            case 'm':
                albedo    = get_next_vec3(input_buffer_ss);
                fuzziness = get_next_double(input_buffer_ss);
                scene.materials.push_back(new Metal(albedo, fuzziness));
                break;
            case '/':
                // comment
                break;
            default:
                // end of materials
                goto hittable;
            }
        }

    hittable:
        while (std::getline(file, input_buffer))
        {
            std::stringstream input_buffer_ss(input_buffer);
            // split at spaces
            std::getline(input_buffer_ss, input_buffer, ' ');

            char   type = input_buffer[0];
            Point3 position;
            double radius;
            int    material_idx;
            switch (type)
            {
            case 's':
                position     = get_next_vec3(input_buffer_ss);
                radius       = get_next_double(input_buffer_ss);
                material_idx = get_next_int(input_buffer_ss);
                scene.world.add(new Sphere(position, radius, scene.materials[material_idx]));
                break;
            // found more comments
            default:
                break;
            }
        }
        file.close();
    }
    catch (const std::exception& ex)
    {
        raise_error(ex.what() << " error parsing font file " << file_path << "!");
    }
}
