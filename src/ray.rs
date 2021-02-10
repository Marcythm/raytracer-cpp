use crate::utilities::prelude::*;
use crate::hittable::prelude::*;

#[derive(Debug, Clone, Copy, Default)]
pub struct Ray {
    pub origin    : P3d,
    pub direction : Vec3,
}

impl Ray {
    pub fn new(origin: P3d, direction: Vec3) -> Self {
        Self { origin, direction }
    }
    pub fn from_to(from: P3d, to: P3d) -> Self {
        // generate a ray from origin to destination
        Self { origin: from, direction: to - from }
    }

    pub fn at(&self, t: f64) -> P3d {
        self.origin + t * self.direction
    }

    pub fn color<T: Hittable>(&self, world: &T, depth: i32, rng: &mut SmallRng) -> RGB {
        if depth <= 0 {
            return RGB::new(0.0, 0.0, 0.0);
        }
        if let Some(rec) = world.hit(&self, EPS, INFINITY) {
            if let Some((scattered, attenuation)) = rec.material.scatter(self, &rec, rng) {
                attenuation * scattered.color(world, depth - 1, rng)
            } else {
                RGB::new(0.0, 0.0, 0.0)
            }
        } else {
            let t = 0.5 * (self.direction.unit().y + 1.0);
            (1.0 - t) * RGB::new(1.0, 1.0, 1.0) + t * RGB::new(0.5, 0.7, 1.0)
        }
    }
}
