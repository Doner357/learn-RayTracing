#include <iostream>
#include <cmath>
#include <string>
#include <cstdint>
#include <memory>
#include <chrono>

#include <stb/stb_image.h>
#include <stb/stb_image_write.h>

#include <rtw/rtw_stb_image.hpp>

#include "headers/camera.hpp"
#include "headers/hittable.hpp"
#include "headers/hittable_list.hpp"
#include "headers/material.hpp"
#include "headers/sphere.hpp"
#include "headers/bvh.hpp"
#include "headers/texture.hpp"

// Scenes could be rendered
void bouncing_spheres();
void checkered_spheres();
void earth();
void perlin_spheres();

int main() {
    switch (4)  {
    case 1: bouncing_spheres();  break;
    case 2: checkered_spheres(); break;
    case 3: earth();             break;
    case 4: perlin_spheres();    break;
    default:  break;
    }

    return 0;
}

void bouncing_spheres() {
    
    // -- World --
    hittable_list world;

    auto checker = std::make_shared<checker_texture>(0.32, color(0.2, 0.3, 0.1), color(0.9, 0.9, 0.9));
    world.add(std::make_shared<sphere>(point3(0,-1000,0), 1000, std::make_shared<lambertian>(checker)));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            point3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                std::shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = color::random() * color::random();
                    sphere_material = std::make_shared<lambertian>(albedo);
                    point3 center2 = center + vec3(0, random_double(0.0, 0.5) ,0.0);
                    world.add(std::make_shared<sphere>(center, center2, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = std::make_shared<metal>(albedo, fuzz);
                    world.add(std::make_shared<sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = std::make_shared<dielectric>(1.5);
                    world.add(std::make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = std::make_shared<dielectric>(1.5);
    world.add(std::make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = std::make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(std::make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = std::make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(std::make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

    world = hittable_list(std::make_shared<bvh_node>(world));

    Camera cam;

    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_width       = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth         = 50;

    cam.vfov     = 20;
    cam.lookfrom = point3(13,2,3);
    cam.lookat   = point3(0,0,0);
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 0.6;
    cam.focus_dist    = 10.0;

    // Timer
    std::chrono::time_point start_time = std::chrono::steady_clock::now();
    // Start rendering
    cam.render(world, "bouncing_spheres");
    // Stop timer
    std::chrono::time_point end_time = std::chrono::steady_clock::now();
    std::chrono::duration<double> duration = end_time - start_time;
    
    std::chrono::duration time = end_time - start_time;
    std::chrono::hours h = std::chrono::duration_cast<std::chrono::hours>(time);
    time -= h;
    std::chrono::minutes m = std::chrono::duration_cast<std::chrono::minutes>(time);
    time -= m;
    std::chrono::seconds s = std::chrono::duration_cast<std::chrono::seconds>(time);

    std::ofstream time_record;
    time_record.open("bouncing_spheres_time.txt");
    time_record << "Takes: " << h << ' ' << m << ' ' << s << std::endl;
    time_record.close();
}

void checkered_spheres() {
    
    // -- World --
    hittable_list world;

    auto checker = std::make_shared<checker_texture>(0.32, color(0.2, 0.3, 0.1), color(0.9, 0.9, 0.9));

    world.add(std::make_shared<sphere>(point3(0,-10, 0), 10, std::make_shared<lambertian>(checker)));
    world.add(std::make_shared<sphere>(point3(0, 10, 0), 10, std::make_shared<lambertian>(checker)));

    world = hittable_list(std::make_shared<bvh_node>(world));

    Camera cam;

    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_width       = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth         = 50;

    cam.vfov     = 20;
    cam.lookfrom = point3(13,2,3);
    cam.lookat   = point3(0,0,0);
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 0.6;
    cam.focus_dist    = 10.0;

    // Timer
    std::chrono::time_point start_time = std::chrono::steady_clock::now();
    // Start rendering
    cam.render(world, "checkered_spheres");
    // Stop timer
    std::chrono::time_point end_time = std::chrono::steady_clock::now();
    std::chrono::duration<double> duration = end_time - start_time;
    
    std::chrono::duration time = end_time - start_time;
    std::chrono::hours h = std::chrono::duration_cast<std::chrono::hours>(time);
    time -= h;
    std::chrono::minutes m = std::chrono::duration_cast<std::chrono::minutes>(time);
    time -= m;
    std::chrono::seconds s = std::chrono::duration_cast<std::chrono::seconds>(time);

    std::ofstream time_record;
    time_record.open("checkered_spheres_time.txt");
    time_record << "Takes: " << h << ' ' << m << ' ' << s << std::endl;
    time_record.close();
}

void earth() {
    std::shared_ptr<image_texture> earth_texture = std::make_shared<image_texture>("earthmap.jpg");
    std::shared_ptr<lambertian> earth_surface = std::make_shared<lambertian>(earth_texture);
    std::shared_ptr<sphere> globe = std::make_shared<sphere>(point3(0,0,0), 2, earth_surface);

    Camera cam;

    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_width       = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth         = 50;

    cam.vfov     = 20;
    cam.lookfrom = point3(0,0,12);
    cam.lookat   = point3(0,0,0);
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 0;

    // Timer
    std::chrono::time_point start_time = std::chrono::steady_clock::now();
    // Start rendering
    cam.render(hittable_list(globe), "earth-mapped_sphere");
    // Stop timer
    std::chrono::time_point end_time = std::chrono::steady_clock::now();
    std::chrono::duration<double> duration = end_time - start_time;
    
    std::chrono::duration time = end_time - start_time;
    std::chrono::hours h = std::chrono::duration_cast<std::chrono::hours>(time);
    time -= h;
    std::chrono::minutes m = std::chrono::duration_cast<std::chrono::minutes>(time);
    time -= m;
    std::chrono::seconds s = std::chrono::duration_cast<std::chrono::seconds>(time);

    std::ofstream time_record;
    time_record.open("earth-mapped_sphere.txt");
    time_record << "Takes: " << h << ' ' << m << ' ' << s << std::endl;
    time_record.close();
}

void perlin_spheres() {
    hittable_list world;

    auto pertext = std::make_shared<noise_texture>();
    world.add(std::make_shared<sphere>(point3(0,-1000,0), 1000, std::make_shared<lambertian>(pertext)));
    world.add(std::make_shared<sphere>(point3(0,2,0), 2, std::make_shared<lambertian>(pertext)));

    Camera cam;

    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_width       = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth         = 50;

    cam.vfov     = 20;
    cam.lookfrom = point3(13,2,3);
    cam.lookat   = point3(0,0,0);
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 0;

    cam.render(world, "perlin_sphere");
}