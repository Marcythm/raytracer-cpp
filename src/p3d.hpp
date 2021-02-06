#pragma once

#include "config.hpp"
#include "vec3.hpp"

class p3d {
    using Self = p3d;

    f64 px, py, pz;

public:
    constexpr p3d(): px(0), py(0), pz(0) {}
    constexpr p3d(const Self &other): px(other.px), py(other.py), pz(other.pz) {}
    constexpr p3d(f64 e0, f64 e1, f64 e2): px(e0), py(e1), pz(e2) {}

    constexpr auto x() const -> f64 { return px; }
    constexpr auto y() const -> f64 { return py; }
    constexpr auto z() const -> f64 { return pz; }

    constexpr auto operator += (const Vec3 &rhs) -> Self& { px += rhs.x(); py += rhs.y(); pz += rhs.z(); return *this; }
    constexpr auto operator -= (const Vec3 &rhs) -> Self& { px -= rhs.x(); py -= rhs.y(); pz -= rhs.z(); return *this; }

public:
    friend constexpr auto operator + (const Self &lhs, const Vec3 &rhs) -> Self {
        return Self(lhs.px + rhs.x(), lhs.py + rhs.y(), lhs.pz + rhs.z());
    }
    friend constexpr auto operator - (const Self &lhs, const Vec3 &rhs) -> Self {
        return Self(lhs.px - rhs.x(), lhs.py - rhs.y(), lhs.pz - rhs.z());
    }
    friend constexpr auto operator - (const Self &lhs, const Self &rhs) -> Vec3 {
        return Vec3(lhs.px - rhs.px, lhs.py - rhs.py, lhs.pz - rhs.pz);
    }

    friend auto operator << (std::ostream &o, const Self &rhs) -> std::ostream& {
        return o << rhs.px << ' ' << rhs.py << ' ' << rhs.pz;
    }
};
