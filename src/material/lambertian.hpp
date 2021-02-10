#pragma once

#include "material.hpp"
#include "solid_color.hpp"

struct Lambertian: Material {
    ptr<Texture> albedo;

public:
    Lambertian(const ptr<Texture> &_albedo): albedo(_albedo) {}
    Lambertian(const RGB &_color): albedo(std::make_shared<SolidColor>(_color)) {}

    auto scatter(const Ray &ray, const HitRecord &rec) const -> std::optional<std::pair<Ray, RGB>> override;
};
