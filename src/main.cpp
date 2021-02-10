#include "config.hpp"
#include "lib.hpp"

#include "ray.hpp"
#include "camera.hpp"
#include "aabb.hpp"
#include "bvhnode.hpp"

#include "hittable.hpp"
#include "sphere.hpp"
#include "moving_sphere.hpp"
#include "aarectangle.hpp"
#include "cuboid.hpp"

#include "material.hpp"
#include "lambertian.hpp"
#include "metal.hpp"
#include "dielectric.hpp"
#include "diffuse_light.hpp"

#include "texture.hpp"
#include "solid_color.hpp"
#include "checker_texture.hpp"
#include "noise_texture.hpp"
#include "image_texture.hpp"

#include "transform.hpp"
#include "translation.hpp"

auto random_scene() -> HittableList {
    HittableList scene;

    // const auto ground_material = std::make_shared<Lambertian>(RGB(0.5, 0.5, 0.5));
    // scene.push(Sphere(p3d(0.0, -1000.0, 0.0), 1000.0, ground_material));

    const auto checker = std::make_shared<CheckerTexture>(RGB(0.2, 0.3, 0.1), RGB(0.9, 0.9, 0.9));
    scene.push(Sphere(p3d(0.0, -1000.0, 0.0), 1000.0, std::make_shared<Lambertian>(checker)));

    for (i32 a = -11; a < 11; ++a)
        for (i32 b = -11; b < 11; ++b) {
            const p3d center(a + 0.9 * random_f64(), 0.2, b + 0.9 * random_f64());

            if ((center - p3d(4.0, 0.2, 0.0)).length() > 0.9) {
                const f64 which_material = random_f64();

                if (which_material < 0.8) {
                    // diffuse
                    const auto albedo = RGB::random() * RGB::random();
                    const auto sphere_material = std::make_shared<Lambertian>(albedo);
                    const auto center2 = center + Vec3(0.0, random_f64(0.0, 0.5), 0.0);
                    scene.push(MovingSphere(center, center2, 0.0, 1.0, 0.2, sphere_material));
                } else if (which_material < 0.95) {
                    // metal
                    const auto albedo = RGB::random(0.5, 1.0);
                    const auto fuzz = random_f64(0.0, 0.5);
                    const auto sphere_material = std::make_shared<Metal>(albedo, fuzz);
                    scene.push(Sphere(center, 0.2, sphere_material));
                } else {
                    // glass
                    const auto sphere_material = std::make_shared<Dielectric>(1.5);
                    scene.push(Sphere(center, 0.2, sphere_material));
                }
            }
        }

    const auto material1 = std::make_shared<Dielectric>(1.5);
    scene.push(Sphere(p3d(0.0, 1.0, 0.0), 1.0, material1));

    const auto material2 = std::make_shared<Lambertian>(RGB(0.4, 0.2, 0.1));
    scene.push(Sphere(p3d(-4.0, 1.0, 0.0), 1.0, material2));

    const auto material3 = std::make_shared<Metal>(RGB(0.7, 0.6, 0.5), 0.0);
    scene.push(Sphere(p3d(4.0, 1.0, 0.0), 1.0, material3));

    return scene;
}

auto two_spheres() -> HittableList {
    HittableList hittables;

    const auto checker = std::make_shared<CheckerTexture>(RGB(0.2, 0.3, 0.1), RGB(0.9, 0.9, 0.9));
    hittables.push(Sphere(p3d(0.0, -10.0, 0.0), 10.0, std::make_shared<Lambertian>(checker)));
    hittables.push(Sphere(p3d(0.0,  10.0, 0.0), 10.0, std::make_shared<Lambertian>(checker)));

    return hittables;
}

auto two_perlin_spheres() -> HittableList {
    HittableList hittables;

    const auto pertext = std::make_shared<NoiseTexture>(4.0);
    hittables.push(Sphere(p3d(0.0, -1000.0, 0.0), 1000.0, std::make_shared<Lambertian>(pertext)));
    hittables.push(Sphere(p3d(0.0,     2.0, 0.0),    2.0, std::make_shared<Lambertian>(pertext)));

    return hittables;
}

auto earth() -> HittableList {
    HittableList hittables;

    const auto earth_texture = std::make_shared<ImageTexture>("earthmap.jpg");
    const auto earth_surface = std::make_shared<Lambertian>(earth_texture);
    hittables.push(Sphere(p3d(0.0, 0.0, 0.0), 2.0, earth_surface));

    return hittables;
}

auto simple_light() -> HittableList {
    HittableList hittables;

    const auto pertext = std::make_shared<NoiseTexture>(4.0);
    hittables.push(Sphere(p3d(0.0, -1000.0, 0.0), 1000.0, std::make_shared<Lambertian>(pertext)));
    hittables.push(Sphere(p3d(0.0,     2.0, 0.0),    2.0, std::make_shared<Lambertian>(pertext)));

    const auto difflight = std::make_shared<DiffuseLight>(RGB(4.0, 4.0, 4.0));
    hittables.push(XYAARectangle(3.0, 5.0, 1.0, 3.0, -2.0, difflight));

    return hittables;
}

auto cornell_box() -> HittableList {
    HittableList hittables;

    const auto red   = std::make_shared<Lambertian>  (RGB( 0.65,  0.05,  0.05));
    const auto white = std::make_shared<Lambertian>  (RGB( 0.73,  0.73,  0.73));
    const auto green = std::make_shared<Lambertian>  (RGB( 0.12,  0.45,  0.15));
    const auto light = std::make_shared<DiffuseLight>(RGB(15.00, 15.00, 15.00));

    hittables.push(YZAARectangle(  0.0, 555.0,   0.0, 555.0, 555.0, green));
    hittables.push(YZAARectangle(  0.0, 555.0,   0.0, 555.0,   0.0,   red));
    hittables.push(ZXAARectangle(227.0, 332.0, 213.0, 343.0, 554.0, light));
    hittables.push(ZXAARectangle(  0.0, 555.0,   0.0, 555.0,   0.0, white));
    hittables.push(ZXAARectangle(  0.0, 555.0,   0.0, 555.0, 555.0, white));
    hittables.push(XYAARectangle(  0.0, 555.0,   0.0, 555.0, 555.0, white));

    hittables.push(Cuboid(p3d(130.0, 0.0,  65.0), p3d(295.0, 165.0, 230.0), white));
    hittables.push(Cuboid(p3d(265.0, 0.0, 295.0), p3d(430.0, 330.0, 460.0), white));

    return hittables;
}

auto main() -> i32 {
    // Image
    f64 aspect_ratio      = ASPECT_RATIO;
    i32 image_width       = IMAGE_WIDTH;
    i32 samples_per_pixel = SAMPLES_PER_PIXEL;

    // Scene
    HittableList scene;

    p3d lookfrom(13.0, 2.0, 3.0);
    p3d lookat(0.0, 0.0, 0.0);
    f64 vertical_field_of_view = 40.0;
    f64 aperture = 0.0;
    RGB background(0.0, 0.0, 0.0);

    switch (0) {
        case 1:
            scene                   = random_scene();
            background              = RGB( 0.7, 0.8, 1.0);
            lookfrom                = p3d(13.0, 2.0, 3.0);
            lookat                  = p3d( 0.0, 0.0, 0.0);
            vertical_field_of_view  = 20.0;
            aperture                = 0.1;
            break;
        case 2:
            scene                   = two_spheres();
            background              = RGB( 0.7, 0.8, 1.0);
            lookfrom                = p3d(13.0, 2.0, 3.0);
            lookat                  = p3d( 0.0, 0.0, 0.0);
            vertical_field_of_view  = 20.0;
            break;
        case 3:
            scene                   = two_perlin_spheres();
            background              = RGB( 0.7, 0.8, 1.0);
            lookfrom                = p3d(13.0, 2.0, 3.0);
            lookat                  = p3d( 0.0, 0.0, 0.0);
            vertical_field_of_view  = 20.0;
            break;
        case 4:
            scene                   = earth();
            background              = RGB( 0.7, 0.8, 1.0);
            lookfrom                = p3d(13.0, 2.0, 3.0);
            lookat                  = p3d( 0.0, 0.0, 0.0);
            vertical_field_of_view  = 20.0;
            break;
        case 5:
            scene                   = simple_light();
            samples_per_pixel       = 400;
            background              = RGB( 0.0, 0.0, 0.0);
            lookfrom                = p3d(26.0, 3.0, 6.0);
            lookat                  = p3d( 0.0, 2.0, 0.0);
            vertical_field_of_view  = 20.0;
            break;
        default:
        case 6:
            scene                   = cornell_box();
            aspect_ratio            = 1.0;
            image_width             = 600;
            samples_per_pixel       = 200;
            background              = RGB(  0.0,   0.0,    0.0);
            lookfrom                = p3d(278.0, 278.0, -800.0);
            lookat                  = p3d(278.0, 278.0,    0.0);
            vertical_field_of_view  = 40.0;
            break;
    }

    const i32 image_height = image_width * aspect_ratio;

    BVHNode bvh(scene, 0.0, 1.0);

    // Camera
    const Vec3 viewup(0.0, 1.0, 0.0);
    const f64 focus_distance = 10.0;

    const Camera camera(lookfrom, lookat, viewup, vertical_field_of_view, aspect_ratio, aperture, focus_distance, 0.0, 1.0);

    // Render
    std::cout.tie(0);
    std::cout << "P3\n";
    std::cout << image_width << ' ' << image_height << '\n';
    std::cout << "255\n";

    for (i32 j = image_height - 1; j >= 0; --j) {
        fprintf(stderr, "Rendering: %d lines remaining\n", j);
        for (i32 i = 0; i < image_width; ++i) {
            RGB pixel_color(0.0, 0.0, 0.0);
            for (i32 s = 0; s < samples_per_pixel; ++s) {
                const f64 u = (i + random_f64()) / (image_width - 1);
                const f64 v = (j + random_f64()) / (image_height - 1);
                pixel_color += camera.get_ray(u, v).color(bvh, background, MAX_DEPTH);
            }
            std::cout << pixel_color / samples_per_pixel << '\n';
        }
    }
    fprintf(stderr, "\nDone.\n");
    return 0;
}
