#pragma once

#include "vec3.hpp"
#include "rgb.hpp"
#include "ray.hpp"
#include "hittable.hpp"
#include "material.hpp"

class Lambertian: public Material {
    RGB albedo;

public:
    Lambertian(const RGB &_albedo): albedo(_albedo) {}

    auto scatter(const Ray &ray, const HitRecord &rec) const -> std::optional<std::pair<Ray, RGB>> override {
        if constexpr (constants::diffuse_render_method_type == diffuse_render_method::true_lambertian_reflection)
            return std::optional(std::make_pair(Ray(rec.p, rec.normal + Vec3::random_unit_vector()), albedo));
        else
            return std::optional(std::make_pair(Ray(rec.p, Vec3::random_in_hemisphere(rec.normal)), albedo));
    }
};
